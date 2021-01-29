#include "../cmsis/stm32f0xx.h"
#include "peripherals.h"

extern LoggerInfo_TypeDef LoggerInfo;
extern unsigned char NMEAbuffer[1024];

static char sim28_active = 1;//1 means SIM28 is in acquisition mode, 0 means sleepmode

//initialize ADXL345
void adxl_init()
{
  adxl_standby();//this is to ensure ADXL345 is ready to be reconfigured
  adxl_clear();//set the interrupt lines to 0
  
  I2C1->CR2 = (0x05<<16)|(1<<13)|(0x53<<1);//set NBYTES=5, send START, 0x53+write
  I2C1->TXDR = 0x24;//register address 0x24
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->TXDR = 32;//2g activity thereshold
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->TXDR = 20;//1.25g inactivity thereshold
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->TXDR = 180;//set inactivity time of 180 seconds
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->TXDR = 0b01110000;//dc operation, all axes enabled for ACT, INACT is disabled
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->CR2 = (1<<14);//send STOP
  
  I2C1->CR2 = (0x02<<16)|(1<<13)|(0x53<<1);//set NBYTES=2, send START, 0x53+write
  I2C1->TXDR = 0x2F;//register address 0x2F
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->TXDR = 0b00001000;//activity - INT1, inactivity - INT2
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->CR2 = (1<<14);//send STOP
  
  I2C1->CR2 = (0x02<<16)|(1<<13)|(0x53<<1);//set NBYTES=2, send START, 0x53+write
  I2C1->TXDR = 0x31;//register address 0x31
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->TXDR = 0b00001011;//full resolution, 16g range
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->CR2 = (1<<14);//send STOP
  
  I2C1->CR2 = (0x02<<16)|(1<<13)|(0x53<<1);//set NBYTES=2, send START, 0x53+write
  I2C1->TXDR = 0x38;//register address 0x38
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->TXDR = 0x00;//FIFO bypass mode
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->CR2 = (1<<14);//send STOP

  I2C1->CR2 = (0x02<<16)|(1<<13)|(0x53<<1);//set NBYTES=2, send START, 0x53+write
  I2C1->TXDR = 0x2E;//register address 0x2E
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->TXDR = 0b00010000;//enable only activity interrupt
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->CR2 = (1<<14);//send STOP
  
  I2C1->CR2 = (0x02<<16)|(1<<13)|(0x53<<1);//set NBYTES=2, send START, 0x53+write
  I2C1->TXDR = 0x2D;//register address 0x2D
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->TXDR = 0b00001000;//no link mode, start measurements
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->CR2 = (1<<14);//send STOP
  
  return;
}

//clear ADXL345 activity/inactivity signals (set lines to 0)
void adxl_clear()
{
  //read ADXL345 INT_SOURCE register to reset INT lines to 0
  I2C1->CR2 = (0x01<<16)|(1<<13)|(0x53<<1);//set NBYTES=1, send START, 0x53+write
  I2C1->TXDR = (0x30);//register address 0x30
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->CR2 = (0x01<<16)|(1<<13)|(1<<10)|(0x53<<1);//set NBYTES=1, send reSTART, 0x53+read
  while( !(I2C1->ISR & (1<<2)) );
  I2C1->RXDR;//read RXDR register
  I2C1->CR2 = (1<<14);//send NACK, STOP
  
  return;
}

//tell ADXL345 to stop measurements and enter standby mode
void adxl_standby()
{
  I2C1->CR2 = (0x02<<16)|(1<<13)|(0x53<<1);//set NBYTES=2, send START, 0x53+write
  I2C1->TXDR = 0x2D;//register address 0x2D
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->TXDR = 0x00;//enter standby mode
  while( !(I2C1->ISR & (1<<0)) );
  I2C1->CR2 = (1<<14);//send STOP
  
  return;
}

//------------------------------------------------------------------------------------

//send the configuration commands to SIM28; should be run at startup or when waking up SIM28 from sleep
//this command will also enable DMA channel for USART1 and start filling NMEAbuffer[] with new messages
void sim28_init()
{
  //unless configured otherwise, allow 1PPS signal to light up the status LED
  if(LoggerInfo.ConfigFlags & (1<<1)) GPIOA->MODER &= ~(1<<0);
  else                                GPIOA->MODER |=  (1<<0);
  
  do
    {      
      //restart USART1 with default baudrate for SIM28
      USART1->CR1 = 0;//disable USART1
      USART1->BRR = 833;//change USART1 baudrate to 9600
      USART1->CR1 = (1<<3)|(1<<2)|(1<<0);//enable USART1; send idle character 
      
      sim28_send("PMTK251,115200");//tell SIM28 to use baudrate of 115200
      
      restart_tim2(5);//give SIM28 5ms to finish with baudrate switch
      while(TIM2->CR1 & (1<<0));//wait until timer runs out
      
      //restart USART1 with new baudrate
      USART1->CR1 = 0;//disable USART1
      USART1->BRR = 69;//change USART1 baudrate to 115200
      USART1->CR1 = (1<<3)|(1<<2)|(1<<0);//enable USART1; send idle character
      
      sim28_send((char*) &LoggerInfo.FixInterval);//tell SIM28 the fix interval in milliseconds
    }
  while( !sim28_receive("PMTK001,220,3") );
  
  //tell SIM28 to output GPRMC, GPGGA, GPGSA messages on every position fix
  do sim28_send("PMTK314,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0");
  while( !sim28_receive("PMTK001,314,3") );
  
  //enable DMA channel 5
  DMA1_Channel5->CNDTR = 1024;//destination buffer size = 1024 bytes
  DMA1_Channel5->CPAR = (unsigned int) &(USART1->RDR);//receive data from USART1 RDR
  DMA1_Channel5->CMAR = (unsigned int) &NMEAbuffer[0];//save data to NMEAbuffer[]
  DMA1_Channel5->CCR = (1<<12)|(1<<7)|(1<<5)|(1<<0);//byte access, memory increment mode, circular mode; set higher priority than SPI1
  
  USART1->RQR = (1<<3);//clear RXNE flag
  USART1->CR3 |= (1<<6);//enable DMA requests for USART1 RXNE event
  LoggerInfo.NMEApointer = &NMEAbuffer[0];//start processing NMEA messages from the beginning of NMEAbuffer[]
  
  return;
}

//tell SIM28 to enter sleepmode, disable DMA for USART1
void sim28_sleep()
{  
  if(!sim28_active) return;//if SIM28 is already in sleepmode, return immediately
  
  USART1->CR3 &= ~(1<<6);//disable DMA requests for USART1 RXNE event  
  DMA1_Channel5->CCR = 0;//disable DMA channel 5 (so after re-enabling it later the old data in NMEAbuffer[] is discarded)
  DMA1->IFCR = (1<<19)|(1<<18)|(1<<17)|(1<<16);//clear all DMA1_Channel5 flags
  
  //wake up SIM28 in case it was in sleepmode, make sure it outputs only 1 GPRMC message every 200ms
  do sim28_send("PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0");
  while( !sim28_receive("PMTK001,314,3") );
  do sim28_send("PMTK220,200");
  while( !sim28_receive("PMTK001,220,3") );
  
  do
    {
      sim28_send("PMTK161,0");//tell SIM28 to enter stop mode
      restart_tim2(250);//set a timer for 250ms
      while(TIM2->CR1 & (1<<0));//wait until timer runs out
      
      USART1->RQR = (1<<3);//clear RXNE flag
      restart_tim2(250);//set a timer for 250ms
      while( !(USART1->ISR & (1<<5)) && (TIM2->CR1 & (1<<0)) );//wait until timer runs out or some new character is received
    }
  while(USART1->ISR & (1<<5));//repeat sleepmode entry sequence if SIM28 keeps sending something over USART1
  
  GPIOA->MODER &= ~(1<<0);//turn off the 1PPS status LED
  sim28_active = 0;//remember that SIM28 is in sleepmode
  
  return;
}

//reformat string into NMEA message, send the message to SIM28 over USART1
//$ symbol at the start and * with checksum at the end will be appended
void sim28_send(char* cmd)
{
  unsigned char checksum = 0;
  unsigned char digit = 0;
  
  while( !(USART1->ISR & (1<<7)) );//wait until a new symbol can be sent
  USART1->TDR = 0x24;//place $ symbol at the start
  
  while(*cmd != 0x00)//send all symbols of a command before the null termination
    {
      checksum = (*cmd) ^ checksum;//compute the NMEA checksum
      while( !(USART1->ISR & (1<<7)) );//wait until a new symbol can be sent
      USART1->TDR = *cmd;//send the data byte
      cmd++;//move to the next symbol
    }
  
  while( !(USART1->ISR & (1<<7)) );//wait until a new symbol can be sent
  USART1->TDR = 0x2A;//append * symbol
  
  //convert 4 most significant bits of the checksum byte into ASCII symbol
  if( (checksum >> 4) < 10 ) digit = 48 + (checksum >> 4);
  else                       digit = 65 + (checksum >> 4) - 10;
  while( !(USART1->ISR & (1<<7)) );//wait until a new symbol can be sent
  USART1->TDR = digit;//append first hex checksum digit
  
  //convert 4 least significant bits of the checksum byte into ASCII symbol
  if( (checksum & 0x0F) < 10 ) digit = 48 + (checksum & 0x0F);
  else                         digit = 65 + (checksum & 0x0F) - 10;
  while( !(USART1->ISR & (1<<7)) );//wait until a new symbol can be sent
  USART1->TDR = digit;//append second hex checksum digit
  
  while( !(USART1->ISR & (1<<7)) );//wait until a new symbol can be sent
  USART1->TDR = 0x0D;//append carriage return symbol
  while( !(USART1->ISR & (1<<7)) );//wait until a new symbol can be sent
  USART1->TDR = 0x0A;//append line feed symbol
  
  while( !(USART1->ISR & (1<<6)) );//wait until transfer is complete
  sim28_active = 1;//data sent over USART wakes up SIM28
  
  return;
}

//wait 250ms for specified referenceString to be received from SIM28
//if expected referenceString was found, return 1; otherwise return 0
//USART1 DMA requests should be disabled when calling this
unsigned char sim28_receive(char* referenceString)
{
  char* whereToCheck = referenceString;//used to compare received message with expected one
  char  symbol;//holds last symbol received over USART1

  if(!sim28_active) return 0;//if SIM28 is in sleepmode return 0 immediately  
  restart_tim2(250);//wait no more than 250ms for expected string to be completely received
  
  while(TIM2->CR1 & (1<<0))//keep checking until expected message arrives or 100ms timer runs out
    { 
      if(USART1->ISR & (1<<5))//if a new character was received
	{
	  symbol = USART1->RDR;//read last data received by USART1
	  
	  //if new received symbol matches expected symbol, move to next expected character from referenceString
               if(symbol == *whereToCheck)    whereToCheck++;
	  else if(symbol == *referenceString) whereToCheck = referenceString + 1;
	  else                                whereToCheck = referenceString;
	}
      
      if(*whereToCheck == 0x00) return 1;//if referenceString was actually found somewhere in the byte stream
    }

  return 0;
}

//------------------------------------------------------------------------------------

//run TIM2 for a specified number of milliseconds
void restart_tim2(unsigned int time)
{
  if(time == 0) return;//do nothing if 0ms time was specified
  
  TIM2->CR1 = (1<<7)|(1<<3);//disable TIM2 (in case it was running)
  TIM2->PSC = 999;//TIM2 prescaler = 1000
  TIM2->ARR = time * 8 - 1;//set TIM2 reload value
  TIM2->EGR = (1<<0);//generate update event
  TIM2->SR = 0;//clear overflow flag
  TIM2->CR1 = (1<<7)|(1<<3)|(1<<0);//ARR is buffered, one pulse mode, start upcounting
  
  return;
}

//run TIM3 for the specified time (in seconds, max argument is 327)
void restart_tim3(unsigned short time)
{
  if(time == 0) return;//do nothing if 0ms time was specified
  
  TIM3->CR1 = (1<<7)|(1<<3);//disable TIM3 (in case it was running)
  TIM3->PSC = 39999;//TIM3 prescaler = 40000
  TIM3->ARR = time * 200 - 1;//set TIM3 reload value
  TIM3->EGR = (1<<0);//generate update event
  TIM3->SR = 0;//clear overflow flag
  TIM3->CR1 = (1<<7)|(1<<3)|(1<<0);//ARR is buffered, one pulse mode, start upcounting
  
  return;
}
