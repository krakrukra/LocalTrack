/*-----------------------------------------------------------------------------/
/ Copyright (C) 2018, krakrukra, all rights reserved.
/
/ This is an open source software. Redistribution and use of it in
/ source and binary forms, with or without modification, are permitted
/ provided that the following condition is met:
/
/ 1. Redistributions of source code must retain the above copyright notice,
/    this condition and the following disclaimer.
/
/ This software is provided by the copyright holder and contributors "AS IS"
/ and any warranties related to this software are DISCLAIMED.
/ The copyright holder or contributors SHALL NOT BE LIABLE for any damages
/ caused by use of this software.
/-----------------------------------------------------------------------------*/

#include "../cmsis/stm32f0xx.h"
#include "../fatfs/ff.h"
#include <stdio.h>
#include <stdlib.h>

volatile unsigned char need_sleepcheck = 0;//set to 1 by ISR at every 1PPS falling edge

static void adxl_init();//initialize ADXL345
static void adxl_clear();//clear ADXL345 activity/inactivity signals (set lines to 0)
static void adxl_sleepcheck();//check if ADXL345 activity/inactivity signals are present, react accordingly if they are
static void adxl_standby();//tell ADXL345 to stop measuremetns and enter standby mode
static void nmea_send(char* cmd);//send data to SIM28 over USART1
static void number_update();//update TrackNumber so it is equal to the highest existing one (on SD card)
static unsigned char adc_battcheck();//check if supply voltage is too low (return 1 in this case)

static unsigned char NMEA_buffer[1024];//buffer for NMEA sentences received from SIM28
static unsigned short TrackNumber;//number of the current track file
static char FileName[13];//name of the file to write the current track to

static FATFS SD_fs;
static FIL SD_file;
static DIR SD_dir;
static FILINFO SD_fileInfo;
static unsigned int temp;//for storing general temporary values

int main()
{
  //clocks configuration
  RCC->AHBENR |= (1<<22)|(1<<19)|(1<<18)|(1<<17)|(1<<0);//enable GPIOA, GPOIB, GPIOC, GPIOF, DMA clocks
  RCC->CFGR3 |= (1<<4);//I2C1 uses SYSCLK
  RCC->APB1ENR |= (1<<21)|(1<<11);//enable I2C1, WWDG clocks
  RCC->APB2ENR |= (1<<14)|(1<<12)|(1<<9)|(1<<0);//enable USART1, SPI1, ADC_interface, SYSCFG clocks

  //SCB configuration
  SCB->SCR |= (1<<2);//set stop mode as default low power mode

  //PORTS, EXTI configuration
  GPIOA->MODER |= (1<<21)|(1<<19)|(1<<15)|(1<<13)|(1<<11);//PA5, PA6, PA7;  PA9, PA10 are in alternate function mode ( SPI1; USART1 )
  GPIOA->AFR[1] |= (1<<8)|(1<<4);//PA9, PA10 are USART1
  GPIOB->MODER |= (1<<20)|(1<<15)|(1<<13);//PB10 is output; PB6, PB7 are in alternate function mode
  GPIOB->AFR[0] |= (1<<28)|(1<<24);//PB6, PB7 are I2C1
  SYSCFG->EXTICR[3] |= (1<<9);//EXTI line 14 is connected to GPIOC
  SYSCFG->EXTICR[2] |= (1<<9);//EXTI line 10 is connected to GPIOC
  EXTI->IMR |= (1<<14)|(1<<10);//enable EXTI lines 14, 10
  EXTI->RTSR |= (1<<14);//EXTI line 14 is triggered at rising edge
  EXTI->FTSR |= (1<<10);//EXTI line 10 is triggered at falling edge

  //DMA1 configuration
  DMA1_Channel3->CNDTR = 1024;//transfer size = 1024 bytes
  DMA1_Channel3->CPAR = (unsigned int) &(USART1->RDR);//set peripheral start address
  DMA1_Channel3->CMAR = (unsigned int) NMEA_buffer;//set memory start address
  DMA1_Channel3->CCR = (1<<7)|(1<<5)|(1<<0);//increment memory address, circular mode, enable DMA channel 3

  //I2C1 configuration
  I2C1->TIMINGR = (0x3<<20)|(0x1<<16)|(0x3<<8)|(0x9<<0);//SCLDEL=0x3, SDADEL=0x1, SCLH=0x3, SCLL=0x9 (400kHz clock)
  I2C1->CR1 |= (1<<0);//enable I2C1
  NVIC_EnableIRQ(7);//enable ADXL345 activity interrupt
  
  //SPI1 configuration
  GPIOB->BSRR = (1<<10);//set CS pin high
  SPI1->CR1 |= (1<<9)|(1<<8)|(1<<3)|(1<<2);//always in master mode, 2MHz clock
  SPI1->CR2 |= (1<<12)|(1<<10)|(1<<9)|(1<<8);//generate RXNE event after 8 bits received, set 8 bit frame format
  SPI1->CR1 |= (1<<6);//enable SPI1

  //USART1 configuration
  //1 start bit, 8 bit transfer (least significant bit first), 1 stop bit; oversampling by 16
  USART1->BRR = 833;//BRR = 69 corresponds to SIM28 default baud rate 115200. use BRR = 833 for 9600 baudrate
  USART1->CR1 = (1<<0);//enable USART1
  USART1->CR3 = (1<<6);//enable DMA for reception
  USART1->CR1 |= (1<<3)|(1<<2);//enable USART1 transmitter, receiver; send idle character

  //ADC1 configuration
  ADC1->CR = (1<<31);//start ADC1 calibration
  while(ADC1->CR & (1<<31));//wait until calibration is complete
  ADC1->CFGR2 = (1<<30);//ADC1 clock = PCLK / 2
  ADC1->CR = (1<<0);//enable ADC1
  while( !(ADC1->ISR & (1<<0)) );//wait until ADC1 is ready
  ADC1->CFGR1 = (1<<12);//overwrite on overrun
  ADC1->SMPR = (1<<1)|(1<<0);//use 28.5 ADC cycles of sample time (7.125us)
  ADC->CCR = (1<<22);//enable reference voltage for ADC1
  ADC1->CHSELR = (1<<17);//enable only Vref channel for ADC1
  ADC1->CR |= (1<<1);//disable ADC1
  
  if( f_mount(&SD_fs, "0:", 1) ) NVIC_SystemReset();//initialize SD card

  if( adc_battcheck() )//check for insufficient battery voltage (for correct SIM28 operation)
    {
      adxl_standby();//tell adxl345 to enter standby mode
      
      while( !(USART1->ISR & (1<<5)) );//wait until SIM28 starts sending something (to make sure that it is ready to receive)
      nmea_send("$PMTK161,0*28");//tell SIM28 to enter sleep mode
      
      RCC->CR |= (1<<0);//enable HSI clock
      while( !(RCC->CR & (1<<1)) );//wait until HSI is ready
      RCC->CFGR &= ~( (1<<1)|(1<<0) );//use HSI as system clock
      while( !((RCC->CFGR & 0x0F) == 0b0000) );//wait until HSI is used as system clock
      RCC->CR &= ~(1<<16);//disable HSE clock
      
      __WFI();//MCU enters deep sleep mode (stop)
      
      //at this point processor should be stuck in __WFI() until the power supply is turned off
      while(1);
    }
  
  while( !(USART1->ISR & (1<<5)) );//wait until SIM28 starts sending something (to make sure that it is ready to receive)
  nmea_send("$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28");//tell SIM28 to only send GPGGA and GPRMC messages
  
  //WWDG configuration
  WWDG->CFR = 0x1FF;//window watchdog prescaler = 8, window = 0x7F 
  WWDG->CR = 0xDF;//start WWDG, set timer value to 0x3F (131.072ms)

  adxl_init();//initialize ADXL345
  
  //start saving new data to the highest numberered file that exists on SD card
  number_update();
  sprintf((char*) FileName, "TRK%u.txt", TrackNumber);

  __enable_irq();//enable interrupts globally

  
  while(1)
    {
      WWDG->CR = 0xDF;//refresh WWDG timer value

      if(need_sleepcheck)//at every 1PPS falling edge
	{ 
	  if( adc_battcheck() )//check for insufficient battery voltage
	    {
	      __disable_irq();
	      adxl_standby();//tell adxl345 to enter standby mode
	      
	      nmea_send("$PMTK161,0*28");//tell SIM28 to enter sleep mode
	      
	      RCC->CR |= (1<<0);//enable HSI clock
	      while( !(RCC->CR & (1<<1)) );//wait until HSI is ready
	      RCC->CFGR &= ~( (1<<1)|(1<<0) );//use HSI as system clock
	      while( !((RCC->CFGR & 0x0F) == 0b0000) );//wait until HSI is used as system clock
	      RCC->CR &= ~(1<<16);//disable HSE clock
	      
	      __WFI();//MCU enters deep sleep mode (stop)
	      
	      //at this point processor should be stuck in __WFI() until the power supply is turned off
	      while(1);
	    }

	  adxl_sleepcheck();//check for ADXL345 activity/inactivity signals
	  
	  need_sleepcheck = 0;
	}
      
      if(DMA1->ISR & (1<<10))//when a complete 512 byte block is received
	{ 
	  //write first block to SD card (while that happends new received bytes are saved to the second block in NMEA_buffer)
	  if( f_open(&SD_file, (char*) FileName, FA_OPEN_APPEND | FA_WRITE) ) NVIC_SystemReset();
	  if( f_write(&SD_file, (void*) NMEA_buffer, 512, &temp) ) NVIC_SystemReset();
	  if( f_close(&SD_file) ) NVIC_SystemReset();

	  DMA1->IFCR = (1<<11)|(1<<10)|(1<<9)|(1<<8);//clear all DMA1_Channel3 flags
	}
      
      if(DMA1->ISR & (1<<9))//when the second block is completely received
	{ 
	  //write second block to SD card
	  if( f_open(&SD_file, (char*) FileName, FA_OPEN_APPEND | FA_WRITE) ) NVIC_SystemReset();
	  if( f_write(&SD_file, (void*) NMEA_buffer + 512, 512, &temp) ) NVIC_SystemReset();
	  if( f_close(&SD_file) ) NVIC_SystemReset();

	  DMA1->IFCR = (1<<11)|(1<<10)|(1<<9)|(1<<8);//clear all DMA1_Channel3 flags
	}
      
    }
  
  NVIC_SystemReset();
  return 0;
}


static void adxl_init()
{
  adxl_standby();//this to ensure ADXL345 is ready to be reconfigured
  
  I2C1->CR2 = (0x05<<16)|(1<<13)|(0x53<<1);//set NBYTES=5, send START, 0x53+write
  I2C1->TXDR = 0x24;//register address 0x24
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->TXDR = 32;//2g activity thereshold
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->TXDR = 20;//1.25g inactivity thereshold
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->TXDR = 60;//60 sec inactivity time
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->TXDR = 0b01110111;//dc operation, all axes enabled (both for ACT and INACT)
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->CR2 |= (1<<14);//send STOP
  
  I2C1->CR2 = (0x02<<16)|(1<<13)|(0x53<<1);//set NBYTES=2, send START, 0x53+write
  I2C1->TXDR = 0x2F;//register address 0x2F
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->TXDR = 0b00001000;//activity - INT1, inactivity - INT2
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->CR2 |= (1<<14);//send STOP
 
  I2C1->CR2 = (0x02<<16)|(1<<13)|(0x53<<1);//set NBYTES=2, send START, 0x53+write
  I2C1->TXDR = 0x31;//register address 0x31
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->TXDR = 0b00001011;//full resolution, 16g range
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->CR2 |= (1<<14);//send STOP
  
  I2C1->CR2 = (0x02<<16)|(1<<13)|(0x53<<1);//set NBYTES=2, send START, 0x53+write
  I2C1->TXDR = 0x38;//register address 0x38
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->TXDR = 0x00;//FIFO bypass mode
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->CR2 |= (1<<14);//send STOP

  I2C1->CR2 = (0x02<<16)|(1<<13)|(0x53<<1);//set NBYTES=2, send START, 0x53+write
  I2C1->TXDR = 0x2E;//register address 0x2E
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->TXDR = 0b00011000;//enable activity, inactivity interrupts
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->CR2 |= (1<<14);//send STOP
  
  I2C1->CR2 = (0x02<<16)|(1<<13)|(0x53<<1);//set NBYTES=2, send START, 0x53+write
  I2C1->TXDR = 0x2D;//register address 0x2D
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->TXDR = 0b00101000;//use link mode, start measurements
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->CR2 |= (1<<14);//send STOP

  return;
}

static void adxl_clear()
{
  //read ADXL345 INT_SOURCE register to reset INT lines to 0
  I2C1->CR2 = (0x01<<16)|(1<<13)|(0x53<<1);//set NBYTES=1, send START, 0x53+write
  I2C1->TXDR = (0x30);//register address 0x30
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->CR2 = (0x01<<16)|(1<<13)|(1<<10)|(0x53<<1);//set NBYTES=1, send reSTART, 0x53+read
  while( !(I2C1->ISR & (1<<2)) );
  I2C1->RXDR;//read RXDR register
  I2C1->CR2 |= (1<<15)|(1<<14);//send NACK, STOP

  return;
}

static void adxl_sleepcheck()
{
  if(GPIOC->IDR & (1<<13))//if inactivity was detected
    {
      adxl_clear();//reset ADXL345 interrupt lines

      nmea_send("$PMTK161,0*28");//tell SIM28 to enter sleep mode

      DMA1_Channel3->CCR = 0;//disable DMA channel 3
      
      RCC->CR |= (1<<0);//enable HSI clock
      while( !(RCC->CR & (1<<1)) );//wait until HSI is ready
      RCC->CFGR &= ~( (1<<1)|(1<<0) );//use HSI as system clock
      while( !((RCC->CFGR & 0x0F) == 0b0000) );//wait until HSI is used as system clock
      RCC->CR &= ~(1<<16);//disable HSE clock
      
      __WFI();//MCU enters deep sleep mode (stop)

      //MCU continues to here after activity detection
      RCC->CR |= (1<<16);//enable HSE clock
      while( !(RCC->CR & (1<<17)) );//wait until HSE is ready  
      RCC->CFGR |= (1<<0);//use HSE as system clock
      while( !((RCC->CFGR & 0x0F) == 0b0101) );//wait until HSE is used as system clock
      RCC->CR &= ~(1<<0);//disable HSI clock
      
      //recalibrate ADC1
      ADC1->CR = (1<<31);
      while(ADC1->CR & (1<<31));

      if( adc_battcheck() )//check for insufficient battery voltage
	{
	  __disable_irq();//prevent ISR execution
	  adxl_standby();//tell adxl345 to enter standby mode
	  
	  RCC->CR |= (1<<0);//enable HSI clock
	  while( !(RCC->CR & (1<<1)) );//wait until HSI is ready
	  RCC->CFGR &= ~( (1<<1)|(1<<0) );//use HSI as system clock
	  while( !((RCC->CFGR & 0x0F) == 0b0000) );//wait until HSI is used as system clock
	  RCC->CR &= ~(1<<16);//disable HSE clock
      
	  __WFI();//MCU enters deep sleep mode (stop)
	  
	  //at this point processor should be stuck in __WFI() until the power supply is turned off
	  while(1);
	}
      
      //enable DMA channel 3
      DMA1_Channel3->CNDTR = 1024;
      DMA1_Channel3->CPAR = (unsigned int) &(USART1->RDR);
      DMA1_Channel3->CMAR = (unsigned int) NMEA_buffer;
      DMA1_Channel3->CCR = (1<<7)|(1<<5)|(1<<0);
    }
  
  if(GPIOC->IDR & (1<<14))//if activity was detected
    {
      adxl_clear();//reset ADXL345 interrupt lines
      
      nmea_send("$PMTK101*32");//wake up SIM28
      
      //start saving incoming data in a new file
      if(TrackNumber < 65535) TrackNumber++;
      sprintf((char*) FileName, "TRK%u.txt", TrackNumber);
    }
  
  return;
}

static void adxl_standby()
{
  I2C1->CR2 = (0x02<<16)|(1<<13)|(0x53<<1);//set NBYTES=2, send START, 0x53+write
  I2C1->TXDR = 0x2D;//register address 0x2D
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->TXDR = 0x00;//enter standby mode
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->CR2 |= (1<<14);//send STOP

  return;
}

static void nmea_send(char* cmd)
{
  while(*cmd != 0x00)//send all symbols of a command before the null termination
    {
      while( !(USART1->ISR & (1<<7)) );
      USART1->TDR = *cmd;
      cmd++;
    }
  
  while( !(USART1->ISR & (1<<7)) );
  USART1->TDR = 0x0D;//append carriage return symbol
  while( !(USART1->ISR & (1<<7)) );
  USART1->TDR = 0x0A;//append line feed symbol

  while( !(USART1->ISR & (1<<6)) );//wait until transfer is complete

  return;
}

static void number_update()
{
  if( f_findfirst(&SD_dir, &SD_fileInfo, "0:/", "TRK*") ) NVIC_SystemReset();//find if there are any existing tracks on SD card
  
  while( SD_fileInfo.fname[0] )//if some track file is found keep searching
    {
      temp = atoi( SD_fileInfo.fname + 3 );
      if(temp > TrackNumber) TrackNumber = temp;//set current tracknumber to the highest existing number found yet
      if( f_findnext(&SD_dir, &SD_fileInfo) ) NVIC_SystemReset();
    }

  if( f_closedir(&SD_dir) ) NVIC_SystemReset();
  
  return;
}

static unsigned char adc_battcheck()
{
  unsigned int Vdd_meas = 0;//measured supply voltage in units of 10uV
  unsigned int ADC_Data = 0;
  unsigned short i;

  ADC1->ISR = 0x9F;//clear all ADC flags
  ADC1->CR |= (1<<0);//enable ADC1
  while( !(ADC1->ISR & (1<<0)) );//wait until ADC1 is ready
  
  for(i=0; i<5; i++)
    {
      ADC1->CR |= (1<<2);//start conversion
      while( !(ADC1->ISR & (1<<3)) );//wait until conversion sequence is completed
      ADC_Data = ADC_Data + ADC1->DR;//add new ADC measurement to the sum of previous ones
      ADC1->ISR = 0x9E;//clear all ADC flags, except ADRDY
    }

  ADC1->CR |= (1<<1);//disable ADC1
  
  ADC_Data = ADC_Data / 5;//get an average value for Vref ADC measurement

  Vdd_meas = (330000 * *((unsigned short*) 0x1FFFF7BA) ) / ADC_Data;// Vdd = 3.3V * VREFINT_CAL / VREFINT_DATA
  
  if(Vdd_meas < 282000 ) return 1;//if measured battery voltage is below 2.82V return 1
  else return 0;
}
