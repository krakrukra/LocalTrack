#include "../cmsis/stm32f0xx.h"
#include "usb.h"
#include "usb_rodata.h"

//general usb functions
static void processControlTransaction();
static void processStandardRequest();
static void processClassRequest();
static void processGetDescriptorRequest();
static void processGetStatusRequest();
static void processSetAddressRequest();
static void processClearFeatureRequest();
static void processGetConfigurationRequest();
static void processSetConfigurationRequest();
static void processINtransaction_EP0();
static void processOUTtransaction_EP0();

//MSD class specific functions, variables
static void processBulkOnlyResetRequest();
static void processGetMaxLunRequest();
extern MSDinfo_TypeDef MSDinfo;
extern void processMSDtransaction();

//generic functions
static void restart_tim6(unsigned short time, unsigned short prescaler);

//this data structure is needed for control transfers
ControlInfo_TypeDef ControlInfo;

//----------------------------------------------------------------------------------------------------------------------

//this function must be called before any use of USB 
void usb_init()
{
  RCC->CFGR3 |= (1<<7);//USB uses PLL clock
  RCC->APB1ENR |= (1<<23)|(1<<4);//enable USB interface clock, TIM6 clock
  RCC->APB1RSTR |= (1<<23);//reset USB registers
  RCC->APB1RSTR &= ~(1<<23);//deassert USB macrocell reset signal
  
  USB->CNTR = (1<<0);//enable USB power supply, keep USB reset
  restart_tim6(1, 0);//set TIM6 to count for 1us
  while(TIM6->CR1 & (1<<0));//wait until TIM6 has finished counting
  USB->CNTR = 0;//clear USB reset
  USB->ISTR = 0;//clear any interrupt flags
  
  //EP0 IN buffer start address = 0x0040, size = 64 bytes
  BTABLE->ADDR0_TX  = 0x0040;
  BTABLE->COUNT0_TX = 0x0000;
  //EP0 OUT buffer start address = 0x0080, size = 64 bytes
  BTABLE->ADDR0_RX  = 0x0080;
  BTABLE->COUNT0_RX = (1<<15)|(1<<10);
  //EP2 OUT_0 buffer start address = 0x00C0, size = 64 bytes
  BTABLE->ADDR2_TX  = 0x00C0;
  BTABLE->COUNT2_TX = (1<<15)|(1<<10);
  //EP2 OUT_1 buffer start address = 0x0100, size = 64 bytes
  BTABLE->ADDR2_RX  = 0x0100;
  BTABLE->COUNT2_RX = (1<<15)|(1<<10);
  //EP3 IN_0 buffer start address = 0x0140, size = 64 bytes
  BTABLE->ADDR3_TX  = 0x0140;
  BTABLE->COUNT3_TX = 0x0000;
  //EP3 IN_1 buffer start address = 0x0180, size = 64 bytes
  BTABLE->ADDR3_RX  = 0x0180;
  BTABLE->COUNT3_RX = 0x0000;
  
  usb_reset();//reinitialize endpoints, USB state machines
  
  //perform charging port detection
  USB->BCDR = (1<<1)|(1<<0);//enable Data Contact Detection
  restart_tim6(20, 999);//set TIM6 to count for 20ms
  while(TIM6->CR1 & (1<<0));//wait until 20ms have passed
  restart_tim6(480, 999);//set TIM6 to count for 480ms
  while( !(USB->BCDR & (1<<4)) && (TIM6->CR1 & (1<<0)) );//wait until DCD is completed or TIM6 runs out
  
  USB->BCDR = (1<<2)|(1<<0);//enable Primary Detection
  restart_tim6(50, 999);//set TIM6 to count for 50ms
  while(TIM6->CR1 & (1<<0));//wait until 50ms have passed
  restart_tim6(50, 999);//set TIM6 to count for 50ms
  while( !(USB->BCDR & (1<<5)) && (TIM6->CR1 & (1<<0)) );//wait until PD is completed or TIM6 runs out
  if(USB->BCDR & (1<<5)) GPIOA->MODER |= (1<<18)|(1<<16);//if charging port was detected, increase current draw to 1A
  
  USB->BCDR = (1<<15);//disable charging port detection, enable internal pullup at D+ line
  
  //at this point the only thing left to enable USB transaction handling is to set EF bit in USB->DADDR. the host has to send a RESET signal for that to happend
  return;
}

void usb_reset()
{
  USB->CNTR = 0;//clear USB reset
  USB->ISTR = 0;//clear any interrupt flags
  
  //reinitialize endpoints
  USB->EP0R ^= (1<<15)|(1<<7);       //disable EP0.    set DTOG_RX=0, set DTOG_TX=0, ignore IN/OUT packets, clear both CTR flags; keep EP_TYPE, EP_KIND, EA untouched
  USB->EP2R ^= (1<<15)|(1<<7)|(1<<6);//disable EP2_RX. set DTOG_RX=0, set DTOG_TX=1, ignore IN/OUT packets, clear both CTR flags; keep EP_TYPE, EP_KIND, EA untouched
  USB->EP3R ^= (1<<15)|(1<<7);       //disable EP3_TX. set DTOG_RX=0, set DTOG_TX=0, ignore IN/OUT packets, clear both CTR flags; keep EP_TYPE, EP_KIND, EA untouched
  
  USB->EP0R = (1<<12)|(1<<9)|(1<<6)|(1<<4);//EP0 enabled, assigned CONTROL type, EA = 0. respond with STALL to IN/OUT packets
  USB->EP2R = (1<<8)|(2<<0);//EP2 disabled, assigned BULK type, double-buffered, EA = 2
  USB->EP3R = (1<<8)|(3<<0);//EP3 disabled, assigned BULK type, double-buffered, EA = 3  
  
  //initialize Control state machine related registers
  ControlInfo.DataPointer = 0;
  ControlInfo.BytesLeft = 0;  
  ControlInfo.NewAddress = 0;
  ControlInfo.ConfigurationNumber = 0;
  ControlInfo.ZLPneeded = 0;
  ControlInfo.DeviceState = DEFAULT;
  ControlInfo.TransferStage = IDLE;
  
  //initialize MSD state machine related registers
  MSDinfo.ActiveBuffer = 0;
  MSDinfo.TargetFlag = 0;
  MSDinfo.MSDstage = READY;
  MSDinfo.DataPointer = 0;
  MSDinfo.BytesLeft = 0;
  (MSDinfo.CSW).dCSWSignature = 0x53425355;
  (MSDinfo.CSW).dCSWTag = 0;
  (MSDinfo.CSW).dCSWDataResidue = 0;
  (MSDinfo.CSW).bCSWStatus = 0;
  bufferCopy( (unsigned short*) &(MSDinfo.CSW), (unsigned short*) (BTABLE_BaseAddr + BTABLE->ADDR3_TX), 13 );
  BTABLE->COUNT3_TX = 13;
  
  return;
}

//this function should be used as an ISR for USB interrupt request
void usb_handler()
{
  if(USB->ISTR & (1<<15))//CTR    
    {
      switch(USB->ISTR & 0x0F)//check what endpoint id caused CTR interrupt
	{
	case 0://EP0 CTR
	  processControlTransaction();
	  break;
	  
	case 2://EP2 CTR
	  processMSDtransaction();
	  break;
	  
	case 3://EP3 CTR
	  processMSDtransaction();
	  break;
	  
	  //add case 1:, case 2:, etc, if it is necessary to handle transactions for other endpoints as well
	  
	default:
	  *( (unsigned int*) (USB_BaseAddr + 4*(USB->ISTR & 0x0F)) ) &= 0x070F;//clear CTR_RX, CTR_TX flags in EPnR, where n is number of endpoint specified in ISTR
	  break;
	  
	}
    }
  
  else if(USB->ISTR & (1<<12))//WKUP
    {
      USB->ISTR = 0xEFFF;//clear WKUP flag
    }

  else if(USB->ISTR & (1<<11))//SUSP
    {
      USB->ISTR = 0xF7FF;//clear SUSP flag
    }

  else if(USB->ISTR & (1<<10))//RESET
    {
      usb_reset();//reinitialize USB, clear all interrupt flags
      USB->DADDR = (1<<7);//enable USB transaction handling, set device address to 0
    }

  return;
}

//function for copying data between RAM and PMA buffers (PMA cannot be accessed by word access, so half-word access is used)
void bufferCopy(unsigned short* whereFrom, unsigned short* whereTo, unsigned short byteCount)
{  
  while(byteCount > 1)
    {
      *whereTo = *whereFrom;
      whereFrom++;     
      whereTo++;
      byteCount -= 2;      
    }

  //copy last byte in case ByteCount was an odd number
  if(byteCount)
    {
      *((unsigned char*) whereTo) = *((unsigned char*) whereFrom);
    }
  
  return;
}

//----------------------------------------------------------------------------------------------------------------------

static void processControlTransaction()
{
  if(USB->ISTR & (1<<4))//if OUT or SETUP transaction happened
    {
      if(USB->EP0R & (1<<11))//in case of SETUP transaction
	{	  
	  //copy 8 bytes (which contain control request) from EP0_RX buffer in PMA to ControlInfo structure in RAM
	  //that is needed because buffer in PMA can be overwritten by next transactions and then request data would be lost
	  bufferCopy( (unsigned short*) (BTABLE_BaseAddr + BTABLE->ADDR0_RX), (unsigned short*) &ControlInfo.ControlRequest, 8 );
	  
	  //reinitialize variables needed for control transfer handling
	  ControlInfo.DataPointer = 0;
	  ControlInfo.BytesLeft = 0;	  
	  ControlInfo.ZLPneeded = 0;
	  ControlInfo.TransferStage = IDLE;

	  //only process standard and class specific requests. respond with STALL to any other bmRequestType
	       if( ((ControlInfo.ControlRequest).bmRequestType & 0x60) == (0<<5) ) processStandardRequest();	 
	  else if( ((ControlInfo.ControlRequest).bmRequestType & 0x60) == (1<<5) ) processClassRequest();
	  else USB->EP0R = (1<<13)|(1<<12)|(1<<9)|(1<<5)|(1<<4);//respond with STALL to IN/OUT packets, clear both CTR flags	    
	}
      else//in case of OUT transaction
	processOUTtransaction_EP0();
    }
  else//in case of IN transaction
    processINtransaction_EP0();
  
  return;
}

static void processStandardRequest()
{
  if(ControlInfo.DeviceState == DEFAULT)
    {
      switch((ControlInfo.ControlRequest).bRequest)
	{
	case GET_DESCRIPTOR:
	  processGetDescriptorRequest();
	  break;
	  
	case SET_ADDRESS:		  
	  processSetAddressRequest();
	  break;
	  
	default: //if request not recognized or is not available in current device state
	  USB->EP0R = (1<<13)|(1<<12)|(1<<9)|(1<<5)|(1<<4);//respond with STALL to IN/OUT packets, clear both CTR flags
	  break;
	}
    }
  
  else if(ControlInfo.DeviceState == ADDRESS)
    {
      switch((ControlInfo.ControlRequest).bRequest)
	{ 
	case GET_CONFIGURATION:
	  processGetConfigurationRequest();
	  break;
	  
	case GET_DESCRIPTOR:
	  processGetDescriptorRequest();
	  break;
	  
	case GET_STATUS:
	  processGetStatusRequest();
	  break;
	  
	case SET_ADDRESS:
	  processSetAddressRequest();
	  break;
	  
	case SET_CONFIGURATION:
	  processSetConfigurationRequest(); 
	  break;
	  
	default: //if request not recognized or is not available in current device state
	  USB->EP0R = (1<<13)|(1<<12)|(1<<9)|(1<<5)|(1<<4);//respond with STALL to IN/OUT packets, clear both CTR flags
	  break;
	}
    }
  
  else if(ControlInfo.DeviceState == CONFIGURED)
    {
      switch((ControlInfo.ControlRequest).bRequest)
	{
	case CLEAR_FEATURE:
	  processClearFeatureRequest();
	  break;
	  
	case GET_CONFIGURATION:
	  processGetConfigurationRequest();
	  break;
	  
	case GET_DESCRIPTOR:
	  processGetDescriptorRequest();
	  break;
	  
	case GET_STATUS:
	  processGetStatusRequest();
	  break;
	  
	case SET_CONFIGURATION:
	  processSetConfigurationRequest();
	  break;
	  
	default: //if request not recognized or is not available in current device state
	  USB->EP0R = (1<<13)|(1<<12)|(1<<9)|(1<<5)|(1<<4);//respond with STALL to IN/OUT packets, clear both CTR flags
	  break;
	}
    }
  
  return;
}

static void processClassRequest()
{
  if(ControlInfo.DeviceState == CONFIGURED)
    {
      //if target interface is MSD interface
      if( (ControlInfo.ControlRequest).wIndex == 0 )
	{
	  switch((ControlInfo.ControlRequest).bRequest)
	    {
	    case 0xFE://GET_maxLUN request
	      processGetMaxLunRequest();
	      break;
	      
	    case 0xFF://BULK-ONLY RESET request
	      processBulkOnlyResetRequest();
	      break;
	      
	    default: //if request is not recognized
	      USB->EP0R = (1<<13)|(1<<12)|(1<<9)|(1<<5)|(1<<4);//respond with STALL to IN/OUT packets, clear both CTR flags
	      break;
	    }
	}
      
      //if target interface can not be determined
      else
	{
	  USB->EP0R = (1<<13)|(1<<12)|(1<<9)|(1<<5)|(1<<4);//respond with STALL to IN/OUT packets, clear both CTR flags
	}
    }
  else//in case of DEFAULT or ADDRESS states
    {
      USB->EP0R = (1<<13)|(1<<12)|(1<<9)|(1<<5)|(1<<4);//respond with STALL to IN/OUT packets, clear both CTR flags
    }
  
  return;
}

static void processGetDescriptorRequest()
{
  void* descriptorAddress;
  unsigned short descriptorSize;
  
  switch( (ControlInfo.ControlRequest).wValue )
    {
    case 0x0100://get DEVICE descriptor request
      descriptorAddress = &DeviceDescriptor;
      descriptorSize = sizeof(DeviceDescriptor_TypeDef);
      break;
      
    case 0x0200://get CONFIGURATION descriptor request, index = 0
      descriptorAddress = &GetConfigResponse;
      descriptorSize = sizeof(GetConfigResponse_TypeDef);
      break;
      
    case 0x0300://get STRING descriptor request, index = 0
      descriptorAddress = &StringDescriptor_0;
      descriptorSize = sizeof(StringDescriptor_0);
      break;
      
    case 0x0301://get STRING descriptor request, index = 1
      descriptorAddress = &StringDescriptor_1;
      descriptorSize = sizeof(StringDescriptor_1);
      break;
      
      //add other descriptor codes here if necessary
      
    default://if requested descriptor type is not one of these: DEVICE, CONFIGURATION, STRING
      USB->EP0R = (1<<13)|(1<<12)|(1<<9)|(1<<5)|(1<<4);//respond with STALL to next IN/OUT packet, clear both CTR flags
      return;
    }
    

  //if host requests more data than there is, send all data in the configuration and all subordinate descriptors
  //else send as much data as requested
  if(descriptorSize < (ControlInfo.ControlRequest).wLength)
    {
      ControlInfo.BytesLeft = descriptorSize;
      //if size of data available is smaller than requested and is an integer
      //multiple of bMaxPacketSize0, ZLP is needed to end DATA_IN transfer stage
      if( (descriptorSize % MAXPACKET_0) == 0) ControlInfo.ZLPneeded = 1;
    }
  else
    {
      ControlInfo.BytesLeft = (ControlInfo.ControlRequest).wLength;
    }

  
  if(ControlInfo.BytesLeft <= MAXPACKET_0)//if one non-ZLP transaction is enough to copy all data
    {
      BTABLE->COUNT0_TX = ControlInfo.BytesLeft;//data payload size for next IN transaction = BytesLeft
      //copy data from configuration and all subordinate descriptors in RAM to PMA buffer EP0_TX
      bufferCopy( (unsigned short*) descriptorAddress, (unsigned short*) (BTABLE_BaseAddr + BTABLE->ADDR0_TX), ControlInfo.BytesLeft);
      
      ControlInfo.TransferStage = DATA_IN;//enter data stage, transmit data to host
      ControlInfo.BytesLeft = 0;//at next CTR event there will be no data left to transmit
      
      USB->EP0R = (1<<9)|(1<<4);//respond to next IN packet with data, to OUT packet with NAK, clear both CTR flags			 
    }
  else//if several non-ZLP transactions will be needed to transfer data
    {
      BTABLE->COUNT0_TX = MAXPACKET_0;//data payload size for next IN transaction = MAXPACKET_0  
      //copy data from configuration and all subordinate descriptors in RAM to PMA buffer EP0_TX
      bufferCopy( (unsigned short*) descriptorAddress, (unsigned short*) (BTABLE_BaseAddr + BTABLE->ADDR0_TX), MAXPACKET_0);
      
      ControlInfo.DataPointer = (unsigned int) descriptorAddress + MAXPACKET_0;//set DataPointer to address in RAM where to copy data from when next CTR event happens (IN transaction)
      ControlInfo.TransferStage = DATA_IN;//enter data stage, transmit data to host
      ControlInfo.BytesLeft = ControlInfo.BytesLeft - MAXPACKET_0;//at next CTR event there still will be data to transmit
      
      USB->EP0R = (1<<13)|(1<<12)|(1<<9)|(1<<4);//respond to next IN packet with data, to OUT packet with STALL, clear both CTR flags
    }
    
  return;
}

static void processGetStatusRequest()
{
  BTABLE->COUNT0_TX = 2;//data payload size for next IN transaction = 2
  *( (unsigned short*) (BTABLE_BaseAddr + BTABLE->ADDR0_TX) ) = 0x0000;//respond with 0x0000 to any GetStatus request (device / interface / endpoint)
  ControlInfo.TransferStage = DATA_IN;//enter data stage, transmit data to host
  ControlInfo.BytesLeft = 0;//at next CTR event there will be no data left to transmit
  
  USB->EP0R = (1<<9)|(1<<4);//respond to next IN packet with data, to next OUT packet with NAK, clear both CTR flags	 
  
  return;
}

static void processSetAddressRequest()
{
  BTABLE->COUNT0_TX = 0;//data payload size for next IN transaction = 0
  
  ControlInfo.NewAddress = (unsigned char) ((ControlInfo.ControlRequest).wValue & 0x7F);//assign NewAddress to 7 least significant bits of wValue
  if(ControlInfo.NewAddress) ControlInfo.DeviceState = ADDRESS;//if address is nonzero go to ADDRESS state
  else ControlInfo.DeviceState = DEFAULT;//if address is zero go to DEFAULT state
  ControlInfo.TransferStage = STATUS_IN;//enter status stage, transmit ZLP to host
  
  USB->EP0R = (1<<13)|(1<<12)|(1<<9)|(1<<4);//respond with ZLP to next IN packet, to next OUT packet with STALL, clear both CTR flags
  
  return;
}

void processClearFeatureRequest()
{
  //if ClearHaltRequest
  if( ((ControlInfo.ControlRequest).wValue == 0) && ((ControlInfo.ControlRequest).bmRequestType == 0x02) )
    {
      switch( (ControlInfo.ControlRequest).wIndex )
	{	  
	case 0x0002://target = EP2_OUT
	  USB->EP2R ^= (1<<15)|(1<<13)|(1<<12)|(1<<7)|(1<<6);//enable EP2_RX. set DTOG_RX=0, set DTOG_TX=1, ACK to OUT packets, ignore IN packets, clear both CTR flags; keep EP_TYPE, EP_KIND, EA
	  break;
	  
	case 0x0083://target = EP3_IN
	  USB->EP3R ^= (1<<15)|(1<<14)|(1<<7)|(1<<5)|(1<<4);//enable EP3_TX. set DTOG_RX=1, set DTOG_TX=0, data to IN packets, ignore OUT packets, clear both CTR flags; keep EP_TYPE, EP_KIND, EA
	  break;
	  
	default:
	  USB->EP0R = (1<<13)|(1<<12)|(1<<9)|(1<<5)|(1<<4);//respond with STALL to IN/OUT packets, clear both CTR flags
	  return;
	}

      BTABLE->COUNT0_TX = 0;//data payload size for next IN transaction = 0
      ControlInfo.TransferStage = STATUS_IN;//enter status stage, transmit ZLP to host
      USB->EP0R = (1<<13)|(1<<12)|(1<<9)|(1<<4);//respond with ZLP to next IN packet, with STALL to next OUT packet, clear both CTR flags

    }
  //if not a ClearHaltRequest
  else
    {
      USB->EP0R = (1<<13)|(1<<12)|(1<<9)|(1<<5)|(1<<4);//respond with STALL to IN/OUT packets, clear both CTR flags
    }
  
  return;
}

static void processGetConfigurationRequest()
{
  BTABLE->COUNT0_TX = 1;//data payload size for next IN transaction = 1
  *( (unsigned char*) (BTABLE_BaseAddr + BTABLE->ADDR0_TX) ) = ControlInfo.ConfigurationNumber;//respond with configuration number (if DeviceState != CONFIGURED respond with 0)
  ControlInfo.TransferStage = DATA_IN;//enter data stage, transmit data to host
  ControlInfo.BytesLeft = 0;//at next CTR event there will be no data left to transmit
  
  USB->EP0R = (1<<9)|(1<<4);//respond to next IN packet with data, to next OUT packet with NAK, clear both CTR flags
  
  return;
}

static void processSetConfigurationRequest()
{
  if((ControlInfo.ControlRequest).wValue == 0)//if new configuration number is 0
    {
      BTABLE->COUNT0_TX = 0;//data payload size for next IN transaction = 0
      ControlInfo.ConfigurationNumber = (unsigned char) (ControlInfo.ControlRequest).wValue;
      ControlInfo.DeviceState = ADDRESS;//enter ADDRESS device state
      ControlInfo.TransferStage = STATUS_IN;//enter status stage, transmit ZLP to host

      //deinitialize non-control endpoints
      USB->EP2R ^= (1<<15)|(1<<7)|(1<<6);//disable EP2_RX. set DTOG_RX=0, set DTOG_TX=1, ignore IN/OUT packets, clear both CTR flags; keep EP_TYPE, EP_KIND, EA untouched
      USB->EP3R ^= (1<<15)|(1<<7);//disable EP3_TX. set DTOG_RX=0, set DTOG_TX=0, ignore IN/OUT packets, clear both CTR flags; keep EP_TYPE, EP_KIND, EA untouched
      
      USB->EP0R = (1<<13)|(1<<12)|(1<<9)|(1<<4);//respond with ZLP to next IN packet, with STALL to next OUT packet, clear both CTR flags
    }
  else if((ControlInfo.ControlRequest).wValue == 1)//if new configuration number is 1
    {
      BTABLE->COUNT0_TX = 0;//data payload size for next IN transaction = 0
      ControlInfo.ConfigurationNumber = (unsigned char) (ControlInfo.ControlRequest).wValue;
      ControlInfo.DeviceState = CONFIGURED;//enter CONFIGURED device state
      ControlInfo.TransferStage = STATUS_IN;//enter status stage, transmit ZLP to host

      //initialize non-control endpoints
      USB->EP2R ^= (1<<15)|(1<<13)|(1<<12)|(1<<7)|(1<<6);//enable EP2_RX. set DTOG_RX=0, set DTOG_TX=1, ACK to OUT packets, ignore IN packets, clear both CTR flags; keep EP_TYPE, EP_KIND, EA untouched
      USB->EP3R ^= (1<<15)|(1<<7)|(1<<5);//enable EP3_TX. set DTOG_RX=0, set DTOG_TX=0, respond with NAK to IN packets, ignore OUT packets, clear both CTR flags; keep EP_TYPE, EP_KIND, EA untouched
      
      USB->EP0R = (1<<13)|(1<<12)|(1<<9)|(1<<4);//respond with ZLP to next IN packet, with STALL to next OUT packet, clear both CTR flags
    }
  else//if new configuration number is not 0 or 1
    {
      USB->EP0R = (1<<13)|(1<<12)|(1<<9)|(1<<5)|(1<<4);//respond to next IN/OUT packet with STALL, clear both CTR flags
    }
  
  return;
}

static void processINtransaction_EP0()
{
  if(ControlInfo.TransferStage == DATA_IN)//if data was sent to the host
    {
      if( (ControlInfo.BytesLeft == 0) && (ControlInfo.ZLPneeded) )//if non-ZLP transaction was just completed and only ZLP transaction is still left to transfer
	{
	  BTABLE->COUNT0_TX = 0;//data payload size for next IN transaction = 0
	  ControlInfo.ZLPneeded = 0;
	  USB->EP0R = (1<<15)|(1<<9)|(1<<4);//respond to next IN packet with ZLP, next OUT packet with NAK, clear CTR_TX flag
	}
      else if(ControlInfo.BytesLeft == 0)//if last transaction was just completed (ZLP or non-ZLP)
	{
	  ControlInfo.TransferStage = STATUS_OUT;//enter status stage, expect ZLP from host
	  USB->EP0R = (1<<15)|(1<<12)|(1<<9)|(1<<8)|(1<<5)|(1<<4);//respond to next ZLP OUT packet with ACK, to non-ZLP OUT packet with STALL, to next IN packet with STALL, clear CTR_TX flag
	}
      else if(ControlInfo.BytesLeft <= MAXPACKET_0)//if only one non-ZLP transaction is left
	{
	  BTABLE->COUNT0_TX = ControlInfo.BytesLeft;//data payload size for next IN transaction = 0
	  //copy data left to be transmitted from RAM to PMA buffer EP0_TX
	  bufferCopy( (unsigned short*) ControlInfo.DataPointer, (unsigned short*) (BTABLE_BaseAddr + BTABLE->ADDR0_TX), ControlInfo.BytesLeft);
	  
	  ControlInfo.BytesLeft = 0;//at next CTR event there will be no data left to transmit
	  
	  USB->EP0R = (1<<15)|(1<<13)|(1<<12)|(1<<9)|(1<<4);//respond to next IN packet with data, next OUT packet with NAK, clear CTR_TX flag	     
	}
      else if(ControlInfo.BytesLeft > MAXPACKET_0)//if there are still several non-ZLP transactions left
	{
	  BTABLE->COUNT0_TX = MAXPACKET_0;//data payload size for next IN transaction = MAXPACKET_0
	  //copy data left to be transmitted from RAM to PMA buffer EP0_TX
	  bufferCopy( (unsigned short*) ControlInfo.DataPointer, (unsigned short*) (BTABLE_BaseAddr + BTABLE->ADDR0_TX), MAXPACKET_0);
	  
	  ControlInfo.DataPointer = ControlInfo.DataPointer + MAXPACKET_0;//set DataPointer to address in RAM where to copy data from when next IN transaction happends
	  ControlInfo.BytesLeft = ControlInfo.BytesLeft - MAXPACKET_0;//at next CTR event there will be no data left to transmit
	  
	  USB->EP0R = (1<<15)|(1<<9)|(1<<4);//respond to next IN packet with data, OUT packet with STALL, clear CTR_TX flag
	}
    }
  
  else if(ControlInfo.TransferStage == STATUS_IN)//if ZLP was just successfully acknowledged by the host
    {
      USB->DADDR = (1<<7) | ControlInfo.NewAddress;//set the new device address
      
      ControlInfo.TransferStage = IDLE;//enter IDLE stage, expect SETUP from host
      USB->EP0R = (1<<15)|(1<<9)|(1<<5)|(1<<4);//respond with STALL to IN/OUT packets, clear CTR_TX flag
    }
  
  return;
}

static void processOUTtransaction_EP0()
{
  if(ControlInfo.TransferStage == DATA_OUT)//if data was received from the host
    { 
      if(ControlInfo.BytesLeft <= MAXPACKET_0)//if last data transaction was just completed
	{
	  BTABLE->COUNT0_TX = 0;//data payload size for next IN transaction = 0
	  //copy received data from PMA to RAM buffer specified by DataPointer
	  bufferCopy((unsigned short*) (BTABLE_BaseAddr + BTABLE->ADDR0_RX), (unsigned short*) ControlInfo.DataPointer, (BTABLE->COUNT0_RX & 0x03FF));
	  
	  ControlInfo.TransferStage = STATUS_IN;//enter status stage, send ZLP to host
	  USB->EP0R = (1<<13)|(1<<12)|(1<<9)|(1<<7)|(1<<4);//respond to next OUT packet with STALL, next IN packet with ZLP, clear CTR_RX flag
	}
      else if(ControlInfo.BytesLeft > MAXPACKET_0)//if there is at least one more data transaction left
	{ 
	  //copy received data from PMA to RAM buffer specified by DataPointer
	  bufferCopy((unsigned short*) (BTABLE_BaseAddr + BTABLE->ADDR0_RX), (unsigned short*) ControlInfo.DataPointer, (BTABLE->COUNT0_RX & 0x03FF));
	  
	  ControlInfo.DataPointer = ControlInfo.DataPointer + (BTABLE->COUNT0_RX & 0x03FF);//set DataPointer to address in RAM where to copy data to when next OUT transaction happends
	  ControlInfo.BytesLeft = ControlInfo.BytesLeft - (BTABLE->COUNT0_RX & 0x03FF);//at next CTR event there will be no data left to receive
	  
	  USB->EP0R = (1<<12)|(1<<9)|(1<<7);//respond to next IN packet with NAK, OUT packet with ACK, clear CTR_RX flag
	}
    }
  
  else if(ControlInfo.TransferStage == STATUS_OUT)//if ZLP was just successfully received from the host
    {
      ControlInfo.TransferStage = IDLE;//enter IDLE stage, wait for new setup packet
      USB->EP0R = (1<<13)|(1<<12)|(1<<9)|(1<<7);//respond to next IN/OUT packets with STALL, clear EP_KIND flag, clear CTR_RX flag      
    }
  
  return;
}


//class specific functions
static void processBulkOnlyResetRequest()
{
  //initialize MSD state machine related registers
  MSDinfo.ActiveBuffer = 0;
  MSDinfo.TargetFlag = 0;
  MSDinfo.MSDstage = READY;
  MSDinfo.DataPointer = 0;
  MSDinfo.BytesLeft = 0;
  (MSDinfo.CSW).dCSWSignature = 0x53425355;
  (MSDinfo.CSW).dCSWTag = 0;
  (MSDinfo.CSW).dCSWDataResidue = 0;
  (MSDinfo.CSW).bCSWStatus = 0;
  bufferCopy( (unsigned short*) &(MSDinfo.CSW), (unsigned short*) (BTABLE_BaseAddr + BTABLE->ADDR3_TX), 13 );
  BTABLE->COUNT3_TX = 13;
  
  BTABLE->COUNT0_TX = 0;//data payload size for next IN transaction = 0
  ControlInfo.TransferStage = STATUS_IN;//enter status stage, transmit ZLP to host
  ControlInfo.BytesLeft = 0;//at next CTR event there will be no data left to transmit
  
  USB->EP0R = (1<<13)|(1<<12)|(1<<9)|(1<<4);//respond to next IN packet with data, to next OUT packet with STALL, clear both CTR flags	 
  
  return;
}

static void processGetMaxLunRequest()
{
  *( (unsigned char*) (BTABLE_BaseAddr + BTABLE->ADDR0_TX) ) = 0x00;//maxLUN = 0
  
  BTABLE->COUNT0_TX = 1;//data payload size for next IN transaction = 1
  ControlInfo.TransferStage = DATA_IN;//enter data stage, transmit data to host
  ControlInfo.BytesLeft = 0;//at next CTR event there will be no data left to transmit
  
  USB->EP0R = (1<<9)|(1<<4);//respond to next IN packet with data, to next OUT packet with NAK, clear both CTR flags	 
  
  return;
}

//generic functions

//run TIM2 for a specified time (max argument for time is 1365), APBCLK is expected to be 48MHz;
//set prescaler = 0 for delay in microseconds, set prescaler = 999 for delay in milliseconds
static void restart_tim6(unsigned short time, unsigned short prescaler)
{
  if(time == 0) return;//do nothing if 0us time was specified
  
  TIM6->CR1 = (1<<7)|(1<<3);//disable TIM6 (in case it was running)
  TIM6->PSC = prescaler;//set TIM6 prescaler
  TIM6->ARR = time * 48 - 1;//set TIM6 reload value
  TIM6->EGR = (1<<0);//generate update event
  TIM6->CR1 = (1<<7)|(1<<3)|(1<<0);//ARR is buffered, one pulse mode, start upcounting
  
  return;
}
