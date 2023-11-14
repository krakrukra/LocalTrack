#include "../cmsis/stm32f0xx.h"
#include "peripherals.h"
#include "../fatfs/ff.h"
#include "../fatfs/diskio.h"
#include "../usb/usb.h"

//regular state check functions
static void sleepmodeCheck();
static void diskmodeCheck();
static void voltageCheck();

//functions for processing of NMEA data
static void processNMEA();
static void processGPRMC();
static void processGPGGA();
static void processGPGSA();
static char checkKeyword(char* referenceString);
static unsigned int readValue(unsigned int factor);
static void skipAfter(char symbol);

//functions for creating GPX output
static void startNewTrack();
static void stopCurrentTrack();
static void addTrackPoint();
static void appendGPXtext(char* text);
static void appendGPXvalue(int value, unsigned int divisor, unsigned int skipLimit);

//functions to write appropriate configuration values
static void readConfigFile();
static void setFixInterval(unsigned int newInterval);
static void setDateTime();

//other functions
static void MCUsleep();
static void verifyGPXfiles();
static void enterBootloader();

//FATFS related global variables
static FATFS FATFSinfo;
static FIL FILinfo;
static FILINFO FILINFOinfo;
static unsigned int temp;//for storing general temporary values

//GPS logger related global variables
unsigned char NMEAbuffer[1024];//buffer for NMEA sentences received from SIM28
unsigned char GPXbuffer[1024];//buffer to temporarily store GPX data to be written in a file

//array to convert month of the year into number of days it contains (change 28 to 29 for leap years)
static unsigned char MonthToDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

LoggerInfo_TypeDef LoggerInfo =
  {
    .RMCdata =
    {
      .time = 0,
      .latitude = 0,
      .longitude = 0,
      .speed = 0,
      .course = 0,
      .date = 0,
      .dataStatus = 0
    },
    
    .GGAdata =
    {
      .time = 0,
      .latitude = 0,
      .longitude = 0,
      .altitude = 0,
      .hdop = 5000,
      .numSatUsed = 0,
      .dataStatus = 0
    },
    
    .GSAdata =
    {
      .pdop = 5000,
      .hdop = 5000,
      .vdop = 5000,
      .fixType = 1 
    },
    
    .SupplyVoltage = 0,
    .BatteryVoltage = 0,
    .TrackFileOffset = 0,
    .NMEApointer = &NMEAbuffer[0],
    .UTCoffset = 0,
    .GPXsize = 0,
    .FileName  = {0},
    .TrackName = {0},
    .FixInterval = "PMTK220,1000",
    .ConfigFlags = 0
  };

//------------------------------------------------------------------------------------

int main()
{
  //clocks configuration  
  RCC->CFGR3 |= (1<<4);//I2C1 runs on system clock
  RCC->AHBENR |= (1<<19)|(1<<18)|(1<<17)|(1<<0);//enable GPIOA, GPOIB, GPIOC, DMA1 clocks
  RCC->APB1ENR |= (1<<28)|(1<<21)|(1<<5)|(1<<1)|(1<<0);//enable PWR, I2C1, TIM2, TIM3, TIM7 clocks
  RCC->APB2ENR |= (1<<14)|(1<<12)|(1<<9)|(1<<0);//enable USART1, SPI1, ADC_interface, SYSCFG clocks
  ADC1->CFGR2 = (1<<30);//ADC1 clock = PCLK / 2
  RCC->CSR |= (1<<0);//enable LSI clock
  
  //SCB configuration
  SCB->SCR |= (1<<2);//set stop mode as default low power mode
  
  //ports configuration
  GPIOB->MODER |= (1<<19)|(1<<17)|(1<<15)|(1<<13)|(1<<3)|(1<<2);//PB6, PB7, PB8, PB9 are in alternate function mode, PB1 is analog input
  GPIOB->AFR[1] |= (1<<4)|(1<<0);//PB8, PB9 are I2C1; PB6, PB7 are USART1
  SYSCFG->CFGR1 |= (1<<10)|(1<<9);//remap USART1 DMA requests to channels 4 and 5
  
  //EXTI configuration
  SYSCFG->EXTICR[3] = (1<<13);//EXTI line 15 is connected to GPIOC
  EXTI->IMR = (1<<20)|(1<<15);//enable EXTI lines 20, 15
  EXTI->RTSR = (1<<20)|(1<<15);//EXTI lines 20, 15 are triggered at rising edge
  NVIC_EnableIRQ(7);//enable ADXL345 activity interrupt
  
  //USART1 configuration
  //1 start bit, 8 bit transfer (least significant bit first), 1 stop bit; oversampling by 16;
  USART1->CR3 = (1<<12);//disable overrun detection, disable DMA requests
  USART1->BRR = 833;//set baudrate to 9600
  USART1->CR1 = (1<<3)|(1<<2)|(1<<0);//enable USART1; send idle character
  
  //I2C1 configuration
  I2C1->TIMINGR = (0x0<<28)|(0x3<<20)|(0x1<<16)|(0x3<<8)|(0x9<<0);//PRESC=0x0, SCLDEL=0x3, SDADEL=0x1, SCLH=0x3, SCLL=0x9 (400kHz clock)
  I2C1->CR1 |= (1<<0);//enable I2C1
  
  //ADC1 configuration
  ADC1->CR = (1<<31);//start ADC1 calibration
  while(ADC1->CR & (1<<31));//wait until calibration is complete
  __DSB();//make sure all outstanding memory transfers are over
  ADC1->CR = (1<<0);//enable ADC1
  while( !(ADC1->ISR & (1<<0)) );//wait until ADC1 is ready
  ADC1->CFGR1 = (1<<16)|(1<<2);//discontinuous conversion mode, keep old data on overrun, scan backwards
  ADC1->SMPR = (1<<2)|(1<<1)|(1<<0);//use 239.5 ADC cycles of sample time (59.875us at PCLK = 8Mhz)
  ADC->CCR = (1<<22);//enable reference voltage for ADC1
  ADC1->CHSELR = (1<<17)|(1<<9);//enable Vref and ADC_IN9 channels for ADC1
  ADC1->CR = (1<<2)|(1<<0);//start new ADC conversion
  
  //RTC configuration
  while( !(RCC->CSR & (1<<1)) );//wait until LSI is ready
  PWR->CR |= (1<<8);//enable access to RTC
  RCC->BDCR |= (1<<16);//reset RTC peripheral
  RCC->BDCR &= ~(1<<16);//deassert RTC reset
  RCC->BDCR |= (1<<15)|(1<<9);//enable clock for RTC peripheral, use LSI
  RTC->WPR = 0xCA;//disable write-protection of RTC registers
  RTC->WPR = 0x53;//disable write-protection of RTC registers
  RTC->ISR |= (1<<7);//enter RTC initialization mode
  while( !(RTC->ISR & (1<<6)) );//wait until RTC configuration can be changed
  RTC->PRER = (124<<16)|(319<<0);//PREDIV_A + 1 = 125, PREDIV_S + 1 = 320; 40kHz clock expected
  RTC->ISR &= ~(1<<7);//exit RTC initialization mode
  NVIC_EnableIRQ(2);//enable RTC wakeup timer interrupt
  
  __enable_irq();//enable interrupts globally
  
//------------------------------------------------------------------------------------
  
  restart_tim2(5);//have a startup delay of 5ms for W25N01GVZEIG
  while(TIM2->CR1 & (1<<0)) voltageCheck();//while timer runs out, keep measuring and supply voltage
  
  //if battery is discharged, stop and wait until battery is recharged up to minimum working level
  if(LoggerInfo.SupplyVoltage < 290000) while(LoggerInfo.SupplyVoltage < 300000) voltageCheck();
  
  if( f_mount(&FATFSinfo, "0:", 1) == FR_OK )//if f_mount() was successful
    { 
      verifyGPXfiles();//append GPX track and file terminations to all GPX files that do not have it (eg. because of poweroff)
      readConfigFile();//run configuration commands from config.txt
      sim28_init();//configure SIM28, ensure it is in a known state
    }
  else//if no valid FAT was found
    {
      disk_initialize(0);//initialize the flash storage for MSD use
      sim28_init();//configure SIM28, ensure it is in a known state
      sim28_sleep();//tell SIM28 to enter sleepmode, since there is nowhere to store the data
    }
  
  restart_tim3(300);//set sleepmode timer to 5 minutes
  adxl_init();//initialize ADXL345
  
  while(1)
    { 
      voltageCheck();//measure battery and supply voltages, save results in RAM
      diskmodeCheck();//if USB cable is connected go to diskmode
      sleepmodeCheck();//if sleepmode timer has run out, go to sleepmode
      processNMEA();//if new 512 byte block of NMEA data is received, process messages inside
      garbage_collect();//if some block in flash became invalid, erase it
    }
  
  NVIC_SystemReset();
  return 0;
}

//------------------------------------------------------------------------------------

//if necessary, enter sleep mode (or low battery mode)
static void sleepmodeCheck()
{
  if(GPIOC->IDR & (1<<15))//if activity was detected
    {
      restart_tim3(120);//reset sleepmode timer to 2 minutes
      adxl_clear();//clear activity signal
    }
  
  //if sleepmode timer had ran out (no activity or speed detected), or if battery is discharged
  if( !(TIM3->CR1 & (1<<0)) || (LoggerInfo.SupplyVoltage < 290000) )
    {
      stopCurrentTrack();//if some track file was being written, add track termination
      sim28_sleep();//tell SIM28 to enter sleepmode
      adxl_clear();//clear activity signal, so that MCU can wake up on new activity detection
      MCUsleep();//enter MCU sleep mode
      while(LoggerInfo.SupplyVoltage < 300000) voltageCheck();//wait until battery is recharged up to minimum working level
      
      if( f_mount(&FATFSinfo, "0:", 1) == FR_OK )//if valid filesystem is available
	{
	  sim28_init();//wake up SIM28
	  restart_tim3(300);//set sleepmode timer to 5 minutes
	}
      else//if filesystem can not be mounted
	{
	  //keep SIM28 in sleep mode
	  restart_tim3(60);//set sleepmode timer to 1 minute
	}
    }
  
  return;
}

//if USB cable is connected, ignore incoming NMEA data and only act as USB disk
static void diskmodeCheck()
{
  if(GPIOB->IDR & (1<<13))//if VBUS voltage is detected
    { 
      stopCurrentTrack();//if some track file was being written, add track and file termination
      sim28_sleep();//tell SIM28 to enter sleepmode
      
      FLASH->ACR = (1<<4)|(1<<0);//enable prefetch buffer, insert 1 wait state for flash read access (needed because SYSCLK will be 48MHz)
      
      RCC->CFGR |= (1<<20)|(1<<16);//derive PLL clock from HSE, multiply HSE by 6 (8MHz * 6 = 48MHz)
      RCC->CR |= (1<<24);//enable PLL
      while( !(RCC->CR & (1<<25)) );//wait until PLL is ready
      RCC->CFGR = (1<<20)|(1<<16)|(1<<1);//set PLL as system clock
      while( !((RCC->CFGR & 0x0F) == 0b1010) );//wait until PLL is used as system clock
      
      if(LoggerInfo.ConfigFlags & (1<<2)) enterBootloader();
      else usb_init();//initialize USB peripheral
      while(GPIOB->IDR & (1<<13)) usb_handler();//while USB cable is plugged in, act as flash disk only
      
      //once USB cable was disconnected, decrease current draw to 83mA, disable USB peripheral
      GPIOA->MODER &= ~((1<<18)|(1<<16));//disconnect chg1 and chg2 lines from ground
      RCC->APB1RSTR |=  (1<<23);//reset USB peripheral
      
      RCC->CFGR = (1<<20)|(1<<16)|(1<<0);//set HSE as system clock
      while( !((RCC->CFGR & 0x0F) == 0b0101) );//wait until HSE is used as system clock
      RCC->CR &= ~(1<<24);//disable PLL
      
      FLASH->ACR = 0;//disable prefetch buffer, have no wait state for flash read access (because SYSCLK will be 8MHz)
      
      if( f_mount(&FATFSinfo, "0:", 1) == FR_OK )//reinitialize FATFSinfo structure, in case the filesystem was altered during USB diskmode
	{	
	  readConfigFile();//load new configuration from config.txt
	  sim28_init();//wake up SIM28
	  restart_tim3(300);//set sleepmode timer to 5 minutes
	}
      else//if filesystem can not be mounted
	{
	  //keep SIM28 in sleepmode
	  restart_tim3(60);//set sleepmode timer to 1 minute  
	}
      
    }
  
  return;
}

//measure battery and supply voltages, save results in RAM
static void voltageCheck()
{
  if(ADC1->ISR & (1<<2))//if ADC conversion is complete
    {
      //if ADC_IN9 channel was converted (end of sequence), compute battery voltage
      if(ADC1->ISR & (1<<3)) LoggerInfo.BatteryVoltage = ( LoggerInfo.SupplyVoltage * ADC1->DR ) / 1365;// Vbat / 3 = Vdd / 2^12 * Vmeas       
      //if Vref channel was converted (sequence not over yet), compute MCU supply voltage
      else LoggerInfo.SupplyVoltage = ( 330000 * *((unsigned short*) 0x1FFFF7BA) ) / ADC1->DR;// Vdd = 3.3V * VREFINT_CAL / VREFINT_DATA
      
      ADC1->ISR = (1<<3)|(1<<2)|(1<<1);//clear EOSEQ, EOC, EOSMP flags
      ADC1->CR = (1<<2)|(1<<0);//start new ADC conversion
    }
  
  return;
}

//------------------------------------------------------------------------------------

//process a newly received 512 byte block of NMEA data
static void processNMEA()
{
  unsigned char* whereToStop; //used to find messageCount value
  unsigned char* whereToCheck;//used to find messageCount value
  unsigned char  messageCount = 0;//holds how many NMEA messages to process
  
  //if a complete 512 byte block was received, clear appropriate DMA flag and remember where the end of this block is
       if(DMA1->ISR & (1<<18)) {DMA1->IFCR = (1<<18); whereToStop = &NMEAbuffer[512];}
  else if(DMA1->ISR & (1<<17)) {DMA1->IFCR = (1<<17); whereToStop = &NMEAbuffer[0];}  
  else return;//if there is no new block to process, do nothing and exit
  
  //find how many messages are to be processed
  whereToCheck = LoggerInfo.NMEApointer;//start searching for message terminations at NMEApointer
  while(whereToCheck != whereToStop)//keep going until the end of newly received block is reached
    {
      if(*whereToCheck == 0x0A) messageCount++;
      
      //move to next character in NMEAbuffer[]
      if(whereToCheck < &NMEAbuffer[1023]) whereToCheck++;
      else whereToCheck = &NMEAbuffer[0];
    }
  
  
  LoggerInfo.GPXsize = 0;//start writing new processed data at the beginning of GPXbuffer[]
  while(messageCount)//keep going until all messages in the buffer were processed
    {
           if( checkKeyword("$GPRMC") ) processGPRMC();//interpret the message and write correct values into RMCdata structure
      else if( checkKeyword("$GPGGA") ) processGPGGA();//interpret the message and write correct values into GGAdata structure
      else if( checkKeyword("$GPGSA") ) processGPGSA();//interpret the message and write correct values into GSAdata structure
      else                              skipAfter(0x0A);//if message is not recognized, skip to the next one
      
      //if last received GPRMC and GPGGA messages are valid and have the same timestamp
      if( ((LoggerInfo.RMCdata).dataStatus = 1) && ((LoggerInfo.GGAdata).dataStatus == 1) && ((LoggerInfo.RMCdata).time == (LoggerInfo.GGAdata).time) )
	{
	  if(LoggerInfo.TrackName[0] == 0) startNewTrack();//if there is no current track, start a new one
	  //add new trackpoint to GPXbuffer[], unless it has 0,0 coordinates
	  if( (LoggerInfo.RMCdata).longitude || (LoggerInfo.RMCdata).latitude ) addTrackPoint();
	  
	  (LoggerInfo.RMCdata).dataStatus = 0;//set RMC dataStatus to invalid, so the message is not used again
	  (LoggerInfo.GGAdata).dataStatus = 0;//set GGA dataStatus to invalid, so the message is not used again
	}
      
      messageCount--;//one more NMEA message was processed
    }
  
  
  if(LoggerInfo.GPXsize)//if there is a need to save some data into a file
    {
      //save all the new processed data to a GPX file
      if( f_open(&FILinfo, (char*) &LoggerInfo.FileName, FA_OPEN_APPEND | FA_WRITE) == FR_OK)
	{
	  f_write(&FILinfo, (void*) &GPXbuffer, LoggerInfo.GPXsize, &temp);
	  f_close(&FILinfo);
	}
    }
  
  return;
}

//fill RMCdata structure in RAM from GPRMC message
static void processGPRMC()
{
  skipAfter(',');//move to UTC time field, read the value
  (LoggerInfo.RMCdata).time = readValue(1000);
  
  skipAfter(',');//move to data status field, 'A' means data valid
  if(*LoggerInfo.NMEApointer == 'A') (LoggerInfo.RMCdata).dataStatus = 1;
  else                               (LoggerInfo.RMCdata).dataStatus = 0;
  
  skipAfter(',');//move to latitude field, read the value, convert it to decimal degrees
  (LoggerInfo.RMCdata).latitude = readValue(100000);//value was read in this format DDMMMMMMM (in units of degree, minute/10^5)
  (LoggerInfo.RMCdata).latitude = ((LoggerInfo.RMCdata).latitude / 10000000) * 10000000 + (((LoggerInfo.RMCdata).latitude % 10000000) * 10) / 6;
  
  skipAfter(',');//move to north/south field, add negative sign in case of south latitude
  if(*LoggerInfo.NMEApointer == 'S') (LoggerInfo.RMCdata).latitude = -(LoggerInfo.RMCdata).latitude;
  
  skipAfter(',');//move to longitude field, read the value, convert it to decimal degrees
  (LoggerInfo.RMCdata).longitude = readValue(100000);//value was read in this format DDDMMMMMMM (in units of degree, minute/10^5)
  (LoggerInfo.RMCdata).longitude = ((LoggerInfo.RMCdata).longitude / 10000000) * 10000000 + (((LoggerInfo.RMCdata).longitude % 10000000) * 10) / 6;
  
  skipAfter(',');//move to east/west field, add negative sign in case of west longitude
  if(*LoggerInfo.NMEApointer == 'W') (LoggerInfo.RMCdata).longitude = -(LoggerInfo.RMCdata).longitude;
  
  skipAfter(',');//move to speed over ground field, read the value, convert it to meters per second
  (LoggerInfo.RMCdata).speed = readValue(1000);//value was read in this format NNNNNNNNN (in units of knot/10^3)
  (LoggerInfo.RMCdata).speed = ((LoggerInfo.RMCdata).speed * 514) / 1000;
  
  skipAfter(',');//move to course over ground field, read the value
  (LoggerInfo.RMCdata).course = readValue(100);
  
  skipAfter(',');//move to date field, read the value
  (LoggerInfo.RMCdata).date = readValue(1);
  
  skipAfter(0x0A);//skip to the next command
  return;
}

//fill GGAdata structure in RAM from GPGGA message
static void processGPGGA()
{
  skipAfter(',');//move to UTC time field, read the value
  (LoggerInfo.GGAdata).time = readValue(1000);
  
  skipAfter(',');//move to latitude field, read the value, convert it to decimal degrees
  (LoggerInfo.GGAdata).latitude = readValue(100000);//value was read in this format DDMMMMMMM (in units of degree, minute/10^5)
  (LoggerInfo.GGAdata).latitude = ((LoggerInfo.GGAdata).latitude / 10000000) * 10000000 + (((LoggerInfo.GGAdata).latitude % 10000000) * 10) / 6;
  
  skipAfter(',');//move to north/south field, add negative sign in case of south latitude
  if(*LoggerInfo.NMEApointer == 'S') (LoggerInfo.GGAdata).latitude = -(LoggerInfo.GGAdata).latitude;
  
  skipAfter(',');//move to longitude field, read the value, convert it to decimal degrees
  (LoggerInfo.GGAdata).longitude = readValue(100000);//value was read in this format DDDMMMMMMM (in units of degree, minute/10^5)
  (LoggerInfo.GGAdata).longitude = ((LoggerInfo.GGAdata).longitude / 10000000) * 10000000 + (((LoggerInfo.GGAdata).longitude % 10000000) * 10) / 6;
  
  skipAfter(',');//move to east/west field, add negative sign in case of west longitude
  if(*LoggerInfo.NMEApointer == 'W') (LoggerInfo.GGAdata).longitude = -(LoggerInfo.GGAdata).longitude;
  
  skipAfter(',');//move to data status field; '1', '2' or '6' means data valid
       if(*LoggerInfo.NMEApointer == '1') (LoggerInfo.GGAdata).dataStatus = 1;
  else if(*LoggerInfo.NMEApointer == '2') (LoggerInfo.GGAdata).dataStatus = 1;
  else if(*LoggerInfo.NMEApointer == '6') (LoggerInfo.GGAdata).dataStatus = 1;
  else                                    (LoggerInfo.GGAdata).dataStatus = 0;
  
  skipAfter(',');//move to sattelites used field, read the value
  (LoggerInfo.GGAdata).numSatUsed = readValue(1);
  
  skipAfter(',');//move to hdop field, read the value
  (LoggerInfo.GGAdata).hdop = readValue(100);
  
  skipAfter(',');//move to MSL altitude field, read the value
  (LoggerInfo.GGAdata).altitude = readValue(1000);
  
  skipAfter(0x0A);//skip to the next command
  return;
}

//fill GSAdata structure in RAM from GPGSA message
static void processGPGSA()
{
  skipAfter(',');//move to mode 1 field, do nothing
  
  skipAfter(',');//move to mode 2 field; '2' means 2D fix, '3' means 3D fix
       if(*LoggerInfo.NMEApointer == '2') (LoggerInfo.GSAdata).fixType = 2;
  else if(*LoggerInfo.NMEApointer == '3') (LoggerInfo.GSAdata).fixType = 3;
  else                                    (LoggerInfo.GSAdata).fixType = 1;
  
  skipAfter(',');//move to sattelite used field  1, do nothing
  skipAfter(',');//move to sattelite used field  2, do nothing
  skipAfter(',');//move to sattelite used field  3, do nothing
  skipAfter(',');//move to sattelite used field  4, do nothing
  skipAfter(',');//move to sattelite used field  5, do nothing
  skipAfter(',');//move to sattelite used field  6, do nothing
  skipAfter(',');//move to sattelite used field  7, do nothing
  skipAfter(',');//move to sattelite used field  8, do nothing
  skipAfter(',');//move to sattelite used field  9, do nothing
  skipAfter(',');//move to sattelite used field 10, do nothing
  skipAfter(',');//move to sattelite used field 11, do nothing
  skipAfter(',');//move to sattelite used field 12, do nothing

  skipAfter(',');//move to pdop field, read the value
  (LoggerInfo.GSAdata).pdop = readValue(100);
  
  skipAfter(',');//move to hdop field, read the value
  (LoggerInfo.GSAdata).hdop = readValue(100);
  
  skipAfter(',');//move to vdop field, read the value
  (LoggerInfo.GSAdata).vdop = readValue(100);
  
  skipAfter(0x0A);//skip to the next command
  return;
}

//if referenceString is found at LoggerInfo.NMEApointer, return 1 and move to the next keyword
static char checkKeyword(char* referenceString)
{
  unsigned char* whereToCheck = LoggerInfo.NMEApointer;
  
  //keep comparing characters until the end of reference string
  while(*referenceString)
    {
      //if a mismatch is found, stop the function and return 0
      if( *whereToCheck != *referenceString ) return 0;
      referenceString++;//move to next character in keyword
      
      //move to next character in NMEAbuffer[]
      if(whereToCheck < &NMEAbuffer[1023]) whereToCheck++;
      else whereToCheck = &NMEAbuffer[0];
    }
  
  //if requested keyword was actually found, move NMEApointer to next keyword
  LoggerInfo.NMEApointer = whereToCheck;
  return 1;
}

//convert decimal ASCII string to integer, this string can have a dot in the
//middle; in this case value before the dot is multiplied by specified factor,
//value after the dot is evaluated as a fraction of specified factor
static unsigned int readValue(unsigned int factor)
{
  unsigned int beforeDot = 0;//holds conversion result for number before the dot
  unsigned int  afterDot = 0;//holds conversion result for number  after the dot
  
  //keep comparing until some non-digit symbol is found
  while( (*LoggerInfo.NMEApointer > 47) && (*LoggerInfo.NMEApointer < 58) )
    {
      //convert ASCII symbol to integer
      beforeDot = beforeDot * 10 + (*LoggerInfo.NMEApointer - 48);
      
      //move to next character in NMEAbuffer[]
      if(LoggerInfo.NMEApointer < &NMEAbuffer[1023]) LoggerInfo.NMEApointer++;
      else LoggerInfo.NMEApointer = &NMEAbuffer[0];
    }
  
  if(*LoggerInfo.NMEApointer == '.')//if there is a dot in the middle of value
    {
      beforeDot = beforeDot * factor;//multiply value before dot by specified factor
      
      //move to next character in NMEAbuffer[]
      if(LoggerInfo.NMEApointer < &NMEAbuffer[1023]) LoggerInfo.NMEApointer++;
      else LoggerInfo.NMEApointer = &NMEAbuffer[0];
    }
  
  //keep comparing until some non-digit symbol is found
  while( (*LoggerInfo.NMEApointer > 47) && (*LoggerInfo.NMEApointer < 58) )
    {
      //convert ASCII symbol to integer
      factor = factor / 10;
      afterDot = afterDot + (*LoggerInfo.NMEApointer - 48) * factor;      
      
      //move to next character in NMEAbuffer[]
      if(LoggerInfo.NMEApointer < &NMEAbuffer[1023]) LoggerInfo.NMEApointer++;
      else LoggerInfo.NMEApointer = &NMEAbuffer[0];
    }
  
  return beforeDot + afterDot;
}

//move NMEApointer to the next character after specified symbol
static void skipAfter(char symbol)
{
  //keep inceasing NMEApointer until specified symbol is found
  while( *LoggerInfo.NMEApointer != symbol )
    {
      //move to next character in NMEAbuffer[]
      if(LoggerInfo.NMEApointer < &NMEAbuffer[1023]) LoggerInfo.NMEApointer++;
      else LoggerInfo.NMEApointer = &NMEAbuffer[0];
    }
  
  //move to next character in NMEAbuffer[]
  if(LoggerInfo.NMEApointer < &NMEAbuffer[1023]) LoggerInfo.NMEApointer++;
  else LoggerInfo.NMEApointer = &NMEAbuffer[0];
  
  return;
}

//------------------------------------------------------------------------------------

//append new GPX track header to GPXbuffer[]; if needed, create a new GPX file header as well
static void startNewTrack()
{
  FRESULT result;//holds return value of some fatfs related function
  
  setDateTime();//fill FileName[] and TrackName[] arrays based on current date and time  
  restart_tim2(180000);//set TIM2 to count for 3 minutes (if track will be stopped before TIM2 is done, the track is discarded)
  
  restart_tim3(120);//reset sleepmode timer to 2 minutes
  adxl_clear();//clear activity signal  
  
  result = f_stat((char*) LoggerInfo.FileName, &FILINFOinfo);//find if a file with target FileName[] already exists
  if(result == FR_OK)//if target track file does exist
    {
      LoggerInfo.TrackFileOffset = FILINFOinfo.fsize - 7;//remember where in the file current track will be written
      
      //erase previous GPX file termination
      if( f_open(&FILinfo, (char*) &LoggerInfo.FileName[0], FA_READ | FA_WRITE) == FR_OK )
	{ 
	  f_lseek(&FILinfo, FILINFOinfo.fsize - 7);//move read/write pointer back to where GPX file termination should be
	  f_truncate(&FILinfo);//delete all data past current read/write pointer
	  f_close(&FILinfo);
	}
    }
  else if(result == FR_NO_FILE)//if target track file is not found
    {
      LoggerInfo.TrackFileOffset = 0;//set TrackFileOffset to 0, to indicate that there is only one track in the GPX file
      
      //append GPX file header; use current date for GPX name, in this format: YYYY-MM-DD (year-month-day)
      appendGPXtext("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
      appendGPXtext("<gpx version=\"1.1\" creator=\"LocalTrack\" xmlns=\"http://www.topografix.com/GPX/1/1\">\n");
      appendGPXtext("<metadata>\n");
      appendGPXtext("<name>20");
      appendGPXtext((char*) LoggerInfo.FileName);
      LoggerInfo.GPXsize = LoggerInfo.GPXsize - 4;
      appendGPXtext("</name>\n");
      appendGPXtext("</metadata>\n");      
    }
  
  //append new track header
  appendGPXtext("<trk>\n");
  appendGPXtext("<name>");
  appendGPXtext((char*) LoggerInfo.TrackName);
  appendGPXtext("</name>\n");
  appendGPXtext("<cmt>Battery Voltage: ");
  appendGPXvalue(LoggerInfo.BatteryVoltage, 100000, 100000);
  LoggerInfo.GPXsize = LoggerInfo.GPXsize - 3;
  appendGPXtext("V ");
       if(LoggerInfo.BatteryVoltage >= 420000) appendGPXtext("100%");
  else if(LoggerInfo.BatteryVoltage <= 330000) appendGPXtext("0%");
  else
    {
      appendGPXvalue( (LoggerInfo.BatteryVoltage - 330000) / 9, 100, 100 );
      LoggerInfo.GPXsize = LoggerInfo.GPXsize - 3;
      appendGPXtext("%");
    }
  appendGPXtext("</cmt>\n");
  appendGPXtext("<trkseg>\n");
  
  return;
}

//append GPX track termination to GPXbuffer[]; if last track was too short, delete it completely
static void stopCurrentTrack()
{ 
  if(LoggerInfo.TrackName[0])//if some track was being written to file
    {
      LoggerInfo.GPXsize = 0;//start writing data at the beginning of GPXbuffer[]
      
      //if current track is shorter than 3 minutes and ShortTrackFlag is not set
      if( (TIM2->CR1 & (1<<0)) && !(LoggerInfo.ConfigFlags & (1<<0)) )
	{
	  //if there is only one track in GPX file, delete the whole file
	  if(LoggerInfo.TrackFileOffset == 0) f_unlink((char*) &LoggerInfo.FileName);
	  //if there are multiple tracks, delete only the current track
	  else if( f_open(&FILinfo, (char*) &LoggerInfo.FileName[0], FA_READ | FA_WRITE) == FR_OK )
	    { 
	      f_lseek(&FILinfo, LoggerInfo.TrackFileOffset);//move read/write pointer back to where current track started
	      f_truncate(&FILinfo);//remove current track data
	      
	      //put GPX file termination back in place
	      appendGPXtext("</gpx>\n");	  
	      f_write(&FILinfo, (void*) &GPXbuffer, LoggerInfo.GPXsize, &temp);
	      f_close(&FILinfo);
	    }
	}
      
      else//if current track is at least 3 minutes long or ShortTrackFlag is set
	{ 
	  //save all the new processed data to a GPX file
	  if( f_open(&FILinfo, (char*) &LoggerInfo.FileName, FA_OPEN_APPEND | FA_WRITE) == FR_OK)
	    { 
	      //append GPX track and file terminations
	      appendGPXtext("</trkseg>\n");
	      appendGPXtext("</trk>\n");
	      appendGPXtext("</gpx>\n");
	      f_write(&FILinfo, (void*) &GPXbuffer, LoggerInfo.GPXsize, &temp);
	      f_close(&FILinfo);
	    }
	}
      
    }
  
  LoggerInfo.TrackName[0] = 0;//remember that there is no current track anymore  
  return;
}

//append trackpoint information into GPXbuffer[]
static void addTrackPoint()
{ 
  //add string in this form: <trkpt lat="-61.6317250" lon="132.2348600">
  appendGPXtext("<trkpt lat=\"");
  appendGPXvalue( (LoggerInfo.RMCdata).latitude,  10000000, 10000000 );
  appendGPXtext("\" lon=\"");
  appendGPXvalue( (LoggerInfo.RMCdata).longitude, 10000000, 10000000 );
  appendGPXtext("\">\n");
  
  //add string in this form: <ele>146.125</ele>
  appendGPXtext("  <ele>");
  appendGPXvalue( (LoggerInfo.GGAdata).altitude, 1000, 1000 );
  appendGPXtext("</ele>\n");
  
  //add string in this form: <time>2020-07-31T12:49:32.945Z</time>
  appendGPXtext("  <time>20");
  appendGPXvalue( (LoggerInfo.RMCdata).date % 100, 1, 10 );
  appendGPXtext("-");
  appendGPXvalue( ((LoggerInfo.RMCdata).date / 100) % 100, 1, 10 );
  appendGPXtext("-");
  appendGPXvalue( ((LoggerInfo.RMCdata).date / 10000) % 100, 1, 10 );
  appendGPXtext("T");
  appendGPXvalue( ((LoggerInfo.RMCdata).time / 10000000) % 100, 1, 10 );
  appendGPXtext(":");
  appendGPXvalue( ((LoggerInfo.RMCdata).time / 100000) % 100, 1, 10 );
  appendGPXtext(":");
  appendGPXvalue( (LoggerInfo.RMCdata).time % 100000, 1000, 10000 );
  appendGPXtext("Z</time>\n");
  
  if(LoggerInfo.ConfigFlags & (1<<4))//if REPORT_SPEED command was used
    {
      //add string in this form: <speed>57.341</speed>
      appendGPXtext("  <speed>");
      appendGPXvalue( (LoggerInfo.RMCdata).speed, 1000, 1000 );
      appendGPXtext("</speed>\n");
    }
  
  if(LoggerInfo.ConfigFlags & (1<<5))//if REPORT_COURSE command was used
    {
      //add string in this form: <course>132.87</course>
      appendGPXtext("  <course>");
      appendGPXvalue( (LoggerInfo.RMCdata).course, 100, 100 );
      appendGPXtext("</course>\n");
    }
  
  if(LoggerInfo.ConfigFlags & (1<<6))//if REPORT_NUMSAT command was used
    {
      //add string in this form: <sat>12</sat>
      appendGPXtext("  <sat>");
      appendGPXvalue( (LoggerInfo.GGAdata).numSatUsed, 1, 1 );
      appendGPXtext("</sat>\n");
    }
  
  if(LoggerInfo.ConfigFlags & (1<<7))//if REPORT_FIXTYPE command was used
    {
      //add string in this form: <fix>2d</fix>, but only if fix is available
      if( (LoggerInfo.GSAdata).fixType == 2 ) appendGPXtext("  <fix>2d</fix>\n");
      if( (LoggerInfo.GSAdata).fixType == 3 ) appendGPXtext("  <fix>3d</fix>\n");
    }
  
  //add string in this form: </trkpt>
  appendGPXtext("</trkpt>\n");
  
  //if SIM28 reported speed above 5 km/h (1.389 m/s) reset sleepmode timer to 2 minutes
  if( ((LoggerInfo.RMCdata).speed) >= 1389 ) restart_tim3(120);
  
  return;
}

//append specified text to the previous data in GPXbuffer[], increment GPXsize accordingly
static void appendGPXtext(char* text)
{
  while(*text)//keep writing until end of string is detected
    {
      if(LoggerInfo.GPXsize >= 1024) return;//do not write outside of GPXbuffer[]
      GPXbuffer[LoggerInfo.GPXsize] = *text;//copy character into GPXbuffer[]
      
      text++;//move to the next character in data string
      LoggerInfo.GPXsize++;//one more byte was saved in GPXbuffer[]
    }
  
  return;
}

//convert integer to a string and append it to the previous data in GPXbuffer[], increment GPXsize accordingly;
//value will be divided by a specified divisor, in order to place a decimal point separator in the right place;
//leading zero digits with significance bigger than skipLimit will be omitted, nonzero digits are never omitted;
//divisor and skipLimit must be numbers that are a power of 10, from 10^0 to 10^9; skipLimit should not be less than divisor
static void appendGPXvalue(int value, unsigned int divisor, unsigned int skipLimit)
{ 
  unsigned int  divideBy = 1000000000;//used to step through the digits in specified decimal value
  unsigned char symbol = 0;//contains ASCII code for current digit
  
  if(value < 0)//if specified value is negative
    {
      if(LoggerInfo.GPXsize >= 1024) return;//do not write outside of GPXbuffer[]
      GPXbuffer[LoggerInfo.GPXsize] = '-';//add minus sign at the start
      LoggerInfo.GPXsize++;//one more byte was saved in GPXbuffer[]
      
      value = -value;//make the value positive
    }
  
  //skip leading zeroes, but make sure not to go over skipLimit; eg. skipLimit = divisor * 10
  //means leave 2 symbols before the dot, even if both of them are zeroes
  while( ((value / divideBy) == 0) && (divideBy > skipLimit) ) divideBy = divideBy / 10;
  
  while(divideBy)//keep converting until all digits are processed
    {
      symbol = (value / divideBy) % 10 + 48;//find the next digit to add in the string
      
      if(LoggerInfo.GPXsize >= 1024) return;//do not write outside of GPXbuffer[]
      GPXbuffer[LoggerInfo.GPXsize] = symbol;//add one more digit in the string
      LoggerInfo.GPXsize++;//one more byte was saved in GPXbuffer[]
      
      //if last digit before the dot was just now processed, and there are still digits left to place after the dot
      if((divideBy == divisor) && (divisor != 1))
	{
	  if(LoggerInfo.GPXsize >= 1024) return;//do not write outside of GPXbuffer[]
	  GPXbuffer[LoggerInfo.GPXsize] = '.';//add dot separator character
	  LoggerInfo.GPXsize++;//one more byte was saved in GPXbuffer[]
	}
      
      divideBy = divideBy / 10;//move on to convert the next digit
    }
  
  return;
}

//------------------------------------------------------------------------------------

//write configuration values/strings according to config.txt (or default settings)
static void readConfigFile()
{
  unsigned char* whereToCheck = &NMEAbuffer[0];//used to count how many commands to process
  unsigned char  messageCount = 0;//used to count how many commands to process
  
  //reset fix interval to 1000ms
  LoggerInfo.FixInterval[8]  = '1';
  LoggerInfo.FixInterval[9]  = '0';
  LoggerInfo.FixInterval[10] = '0';
  LoggerInfo.FixInterval[11] = '0';
  LoggerInfo.FixInterval[12] = 0x00;
  LoggerInfo.FixInterval[13] = 0x00;
  
  //reset UTC offset to 0 minutes, clear all config flags
  LoggerInfo.UTCoffset = 0;
  LoggerInfo.ConfigFlags = 0;
  
  //try to open config.txt and load the settings from there
  if( f_open(&FILinfo, "0:/config.txt", FA_READ) == FR_OK )
    { 
      f_read(&FILinfo, (void*) &NMEAbuffer[0], 512, &temp);
      f_close(&FILinfo);
      NMEAbuffer[temp] = 0x0A;//force append a newline character at the end (in case user did not put it there)
      
      while(whereToCheck != &NMEAbuffer[temp + 1])//find how many messages are to be processed
	{
	  if(*whereToCheck == 0x0A) messageCount++;
	  whereToCheck++;
	}
      
      LoggerInfo.NMEApointer = &NMEAbuffer[0];//start interpreting commands from the start of NMEAbuffer[]      
      while(messageCount)//keep processing commands until no more left
	{
	       if( checkKeyword("FIX_INTERVAL ") )      setFixInterval( readValue(1) );
	  else if( checkKeyword("UTC_OFFSET +") )       LoggerInfo.UTCoffset =  readValue(1);
	  else if( checkKeyword("UTC_OFFSET -") )       LoggerInfo.UTCoffset = -readValue(1);
	  else if( checkKeyword("ALLOW_SHORT_TRACKS") ) LoggerInfo.ConfigFlags |= (1<<0);
	  else if( checkKeyword("DISABLE_1PPS_LED") )   LoggerInfo.ConfigFlags |= (1<<1);
	  else if( checkKeyword("ENTER_DFU_MODE") )     LoggerInfo.ConfigFlags |= (1<<2);
	  else if( checkKeyword("REPORT_SPEED") )       LoggerInfo.ConfigFlags |= (1<<4);
	  else if( checkKeyword("REPORT_COURSE") )      LoggerInfo.ConfigFlags |= (1<<5);
	  else if( checkKeyword("REPORT_NUMSAT") )      LoggerInfo.ConfigFlags |= (1<<6);
	  else if( checkKeyword("REPORT_FIXTYPE") )     LoggerInfo.ConfigFlags |= (1<<7);
	  else if( checkKeyword("MASS_ERASE") )        {mass_erase(); NVIC_SystemReset();}	  
	  
	  
	  skipAfter(0x0A);//stop if no more configuration commands are found
	  messageCount--;//one more command was processed
	}
      
      //make sure UTC offset is between -24 hours and +24 hours
      if(LoggerInfo.UTCoffset >  1440) LoggerInfo.UTCoffset =  1440;
      if(LoggerInfo.UTCoffset < -1440) LoggerInfo.UTCoffset = -1440;      
    }
  
  return;
}

//sets time interval between position fixes
static void setFixInterval(unsigned int newInterval)
{ 
  unsigned short divideBy = 10000;
  unsigned char i = 8;
  
  //make sure newInterval value is between 200 and 10000
  if(newInterval < 200)   newInterval = 200;
  if(newInterval > 10000) newInterval = 10000;
  
  //convert newInterval integer to ASCII string, have no zeroes in the prefix (eg. have "235" instead of "00235" )
  while(divideBy)
    {
      LoggerInfo.FixInterval[i] = 48 + (newInterval / divideBy) % 10;
      divideBy = divideBy / 10;
      if(LoggerInfo.FixInterval[8] != 48) i++;
    }
  LoggerInfo.FixInterval[i] = 0x00;//append string terminator at the end
    
  return;
}

//take current date and time, apply UTC offset to it, then fill FileName[] and TrackName[] based on the result
static void setDateTime()
{
  //start with local date equal to UTC date
  unsigned char localYear  = ((LoggerInfo.RMCdata).date % 100);
  unsigned char localMonth = ((LoggerInfo.RMCdata).date / 100) % 100;
  unsigned char localDay   = ((LoggerInfo.RMCdata).date / 10000);
  short localMinutes = 0;//used to store time of the day, in units of 1 minute
  
  //change last day number in february to 29 in case of a leap year
  if( (localYear % 4) == 0 ) MonthToDays[1] = 29;
  else                       MonthToDays[1] = 28;
  
  //calculate local time from UTC time (convert hours to minutes)
  localMinutes = ((LoggerInfo.RMCdata).time / 10000000) * 60 + ((LoggerInfo.RMCdata).time / 100000) % 100 + LoggerInfo.UTCoffset;
  
  if(localMinutes > 1440)//if necessary, move date 1 day forward
    {      
      if(localDay < MonthToDays[localMonth - 1]) localDay++;//if there is no need to change month
      else//if next day will be in a new month, switch to a new month
	{ 
	  if(localMonth < 12) localMonth++;//if there is no need to change year
	  else//if next month will be in a new year, switch to a new year
	    {
	      localMonth = 1;
	      localYear++;
	    }
	  
	  localDay = 1;
	}
      
      localMinutes = localMinutes - 1440;//take date change into account
    }
  
  else if (localMinutes < 0)//if necessary, move date 1 day backward
    {
      if(localDay > 1) localDay--;//if there is no need to change month
      else//if previous day will be in a previous month, switch to previous month
	{	  
	  if(localMonth > 1) localMonth--;//if there is no need to change year
	  else//if previous month will be in a previous year, switch to previous year
	    {
	      localMonth = 12;
	      localYear--;
	    }
	  
	  localDay = MonthToDays[localMonth - 1];
	}
      
      localMinutes = localMinutes + 1440;//take date change into account
    }
  
  //write local year, month and day in FileName[]
  LoggerInfo.FileName[0]  = (localYear / 10) + 48;
  LoggerInfo.FileName[1]  = (localYear % 10) + 48;
  LoggerInfo.FileName[2]  = '-';
  LoggerInfo.FileName[3]  = (localMonth / 10) + 48;
  LoggerInfo.FileName[4]  = (localMonth % 10) + 48;
  LoggerInfo.FileName[5]  = '-';
  LoggerInfo.FileName[6]  = (localDay / 10) + 48;
  LoggerInfo.FileName[7]  = (localDay % 10) + 48;
  LoggerInfo.FileName[8]  = '.';
  LoggerInfo.FileName[9]  = 'G';
  LoggerInfo.FileName[10] = 'P';
  LoggerInfo.FileName[11] = 'X';
  LoggerInfo.FileName[12] = 0;
  
  //write local hour, minute and second in TrackName[]
  LoggerInfo.TrackName[0] = (localMinutes / 60) / 10 + 48;
  LoggerInfo.TrackName[1] = (localMinutes / 60) % 10 + 48;
  LoggerInfo.TrackName[2] = ':';
  LoggerInfo.TrackName[3] = (localMinutes % 60) / 10 + 48;
  LoggerInfo.TrackName[4] = (localMinutes % 60) % 10 + 48;
  LoggerInfo.TrackName[5] = ':';
  LoggerInfo.TrackName[6] = ((LoggerInfo.RMCdata).time / 10000) % 10 + 48;
  LoggerInfo.TrackName[7] = ((LoggerInfo.RMCdata).time /  1000) % 10 + 48;
  LoggerInfo.TrackName[8] = 0; 
  
  return;
}

//------------------------------------------------------------------------------------

//enter MCU sleep mode, wake up on interrupt
static void MCUsleep()
{
  ADC1->CR = (1<<4)|(1<<0);//stop current ADC1 conversion
  while(ADC1->CR & (1<<4));//wait until ADC1 conversion is stopped
  ADC1->CR = (1<<1)|(1<<0);//disable ADC1
  while(ADC1->CR & (1<<0));//wait until ADC1 is completely disabled
  ADC1->ISR = 0x9F;//clear all ADC1 flags
  
  //reset voltage measurements to zero, to avoid using old values after wakeup
  LoggerInfo.SupplyVoltage = 0;
  LoggerInfo.BatteryVoltage = 0;
  
  RCC->CR |= (1<<0);//enable HSI clock
  while( !(RCC->CR & (1<<1)) );//wait until HSI is ready
  RCC->CFGR = 0;//use HSI as system clock
  while( !((RCC->CFGR & 0x0F) == 0b0000) );//wait until HSI is used as system clock
  RCC->CR &= ~(1<<16);//disable HSE clock
  
  while ( !(RTC->ISR & (1<<2)) );//wait until wakeup timer can be configured
  RTC->WUTR = 10799;//make sure that MCU wakes up at least once every 3 hours
  RTC->CR = (1<<14)|(1<<10)|(1<<2);//start wakeup timer, enable wakeup interrupt, use 1Hz clock
  
  __WFI();//MCU enters deep sleep mode (stop)
  
  RTC->CR &= ~(1<<10);//disable wakeup timer
  RTC->ISR &= ~(1<<10);//clear wakeup timer flag  
  
  //MCU continues execution here after activity detection
  RCC->CR |= (1<<19)|(1<<16);//enable HSE clock, clock security system
  while( !(RCC->CR & (1<<17)) );//wait until HSE is ready
  RCC->CFGR = (1<<0);//set HSE as system clock
  while( !((RCC->CFGR & 0x0F) == 0b0101) );//wait until HSE is used as system clock
  RCC->CR &= ~(1<<0);//disable HSI
  
  //enable ADC1 again
  ADC1->CR = (1<<31);//start ADC1 calibration
  while(ADC1->CR & (1<<31));//wait until calibration is complete
  __DSB();//make sure all outstanding memory transfers are over
  ADC1->CR = (1<<0);//enable ADC1
  while( !(ADC1->ISR & (1<<0)) );//wait until ADC1 is ready
  ADC1->CFGR1 = (1<<16)|(1<<2);//discontinuous conversion mode, keep old data on overrun, scan backwards
  ADC1->SMPR = (1<<2)|(1<<1)|(1<<0);//use 239.5 ADC cycles of sample time (59.875us at PCLK = 8Mhz)
  ADC->CCR = (1<<22);//enable reference voltage for ADC1
  ADC1->CHSELR = (1<<17)|(1<<9);//enable Vref and ADC_IN9 channels for ADC1
  ADC1->CR = (1<<2)|(1<<0);//start new ADC conversion
  
  return;
}

//search through available GPX files and append track and file terminations to all files that do not have it
static void verifyGPXfiles()
{
  FRESULT result;//holds return value of some fatfs related function
  DIR DIRinfo;
  
  result = f_findfirst(&DIRinfo, &FILINFOinfo, "0:/", "*.GPX");//find if there are any existing tracks in flash memory  
  while( (result == FR_OK) && FILINFOinfo.fname[0] )//keep searching until all tracks are verified
    {
      if( f_open(&FILinfo, &FILINFOinfo.fname[0], FA_READ | FA_WRITE) == FR_OK)//try to open the track file
	{ 
	  f_lseek(&FILinfo, FILINFOinfo.fsize - 7);//move read/write pointer back to where GPX file termination should be
	  f_read(&FILinfo, (void*) &NMEAbuffer[0], 7, &temp);//read last 7 bytes from the file into NMEAbuffer[]
	  
	  LoggerInfo.NMEApointer = &NMEAbuffer[0];//move NMEApointer to the beginning of NMEAbuffer[]
	  if( !checkKeyword("</gpx>") )//if "</gpx>" string was not found
	    {
	      //append GPX track and file terminations
	      LoggerInfo.GPXsize = 0;
	      appendGPXtext("</trkseg>\n");
	      appendGPXtext("</trk>\n");
	      appendGPXtext("</gpx>\n");
	      
	      f_lseek(&FILinfo, FILINFOinfo.fsize);//move read/write pointer to the end of file
	      f_write(&FILinfo, (void*) &GPXbuffer, LoggerInfo.GPXsize, &temp);//write GPX data to file
	    }
	  f_close(&FILinfo);
	}
      result = f_findnext(&DIRinfo, &FILINFOinfo);
    }
  f_closedir(&DIRinfo);
  
  return;
}

//transfer control over MCU to factory-programmed bootloader (for firmware updates)
static void enterBootloader()
{
  //set function pointer to a value specified in the vector table of System Memory
  void (*bootloader)(void) = (void (*)(void)) ( *((unsigned int*) (0x1FFFC804U)) );
  
  if( f_unlink("0:/config.txt") != FR_OK) return;
  
  //disable all interrupts in the NVIC
  NVIC_DisableIRQ(2);
  NVIC_DisableIRQ(7);
  NVIC_DisableIRQ(10);
  
  //clear all pending interrupts
  NVIC_ClearPendingIRQ(2);
  NVIC_ClearPendingIRQ(7);
  NVIC_ClearPendingIRQ(10);
  
  //reset all peripherals  
  RCC->AHBRSTR  = 0x017E0000;
  RCC->APB1RSTR = 0x7AFE4933;
  RCC->APB2RSTR = 0x00475AE1;
  RCC->BDCR |= (1<<16);
  RCC->AHBRSTR  = 0x00000000;
  RCC->APB1RSTR = 0x00000000;
  RCC->APB2RSTR = 0x00000000;
  RCC->BDCR &= ~(1<<16);
  
  //disable all peripheral clocks
  RCC->AHBENR  = 0x00000014;
  RCC->APB1ENR = 0x00000000;
  RCC->APB2ENR = 0x00000000;
  RCC->CFGR2   = 0x00000000;
  RCC->CFGR3   = 0x00000000;
  RCC->CR2     = 0x00000080;
  RCC->CSR     = 0x00000000;
  while(RCC->CSR & (1<<1));//wait until LSI is turned off
  
  RCC->CR |= (1<<0);//enable HSI clock
  while( !(RCC->CR & (1<<1)) );//wait until HSI is ready
  RCC->CFGR = 0;//set HSI as system clock
  while( !((RCC->CFGR & 0x0F) == 0b0000) );//wait until HSI is used as system clock  
  RCC->CR = 0x0083;//disable PLL, HSE clocks; disable CSS
  
  //clear all clock ready flags
  RCC->CIR = 0x00FF0000;
  
  __DSB();//make sure all outstanding memory transfers are over before changing MSP value
  __set_MSP(0x20003FFC);//move main stack pointer back to the top
  __ISB();//make sure the effect of changing MSP value is visible immediately
  bootloader();//jump to bootloader code
  
  NVIC_SystemReset();//reset the system if CPU ever returns
  return;
}
