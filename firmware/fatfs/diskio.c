#include "../cmsis/stm32f0xx.h"
#include "diskio.h"	/* FatFs lower layer API */

unsigned char pdrv0_status = STA_NOINIT;
static unsigned char R3[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

DSTATUS disk_status (
        BYTE pdrv	/* Physical drive number to identify the drive */
)
{  
  if(pdrv != 0) return STA_NOINIT;//only physical drive 0 is available

  return pdrv0_status;
}

DSTATUS disk_initialize (BYTE pdrv)
{
  volatile unsigned int i;
  volatile unsigned int limit = 0;

  if(pdrv != 0) return STA_NOINIT;//only physical drive 0 is available
  
  for(i=0; i<4000; i++);// wait for SD card supply voltage to rise

  //send 96 SCK pulses for SD card initialization
  GPIOB->BSRR |= (1<<10);//keep CS high
  for(i=0; i<12; i++)
    {
      while( !(SPI1->SR & (1<<1)) );
      *( (unsigned char*) &(SPI1->DR) ) = 0xFF;
    }
  while(SPI1->SR & (1<<7));//wait until SPI1 is not busy

  //try CMD0 up to 5 times (not 1 to increase reliability)
  for(limit = 0; limit < 5; limit++)
    {
      if ( SD_SendCommand_R1(0x40, 0x00000000, 0x95) <= 1 ) break;//CMD0 = card is reset, SPI mode, CRC off
      for(i=0; i<400; i++);//delay
    }
  if(limit == 5) return STA_NOINIT;
  
  //CMD8 = 3.3V operating voltage
  SD_SendCommand_R3(0x48, 0x0000001AA, 0x87);
  if( (R3[0] > 1) || ((R3[3] & 0x0F) != 0x01) || (R3[4] != 0xAA) ) return STA_NOINIT;

  //CMD55 = next command is application specific
  if ( SD_SendCommand_R1(0x77, 0x00000000, 0x01) > 1) return STA_NOINIT;
  
  //repeat ACMD41 until card responds with 0x00
  for( limit=20000; SD_SendCommand_R1(0x69, 0x40000000, 0x01) && limit; limit-- ) //host supports high capacity cards
    {
      for(i=0; i<400; i++);//delay
      if ( SD_SendCommand_R1(0x77, 0x00000000, 0x01) > 1 ) return STA_NOINIT;//CMD55 = next command is application specific
    }
  if ( limit == 0 ) return STA_NOINIT;

  for(i=0; i<400; i++);//wait for SD card ACMD41 processing
  
  //CMD58 = read OCR register
  SD_SendCommand_R3(0x7A, 0x00000000, 0x01);
  if( R3[0] ) return STA_NOINIT;

  //CMD16 = if card is SDSC set blocksize to 512 bytes
  if( !(R3[1] & 0x40) && (R3[1] & 0x80) )
    {
      if( SD_SendCommand_R1(0x50, 0x00000200, 0x01) ) return STA_NOINIT;
    }

  pdrv0_status = 0;
  return 0;
}

DRESULT disk_read (
	BYTE pdrv,	/* Physical drive number to identify the drive */
	BYTE* buff,	/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count	/* Number of sectors to read */
)
{
  volatile unsigned int limit=40000;
  volatile unsigned int i;
  
  if(pdrv != 0) return RES_PARERR;
  if(count == 0) return RES_PARERR;
  
  if(count == 1)
    {
      if( SD_SendCommand_R1(0x51, sector, 0x01) ) return RES_ERROR;//CMD17 = read one block
      
      while(SPI1->SR & (1<<0)) *buff = (unsigned char) SPI1->DR;//empty RX FIFO
      
      GPIOB->BSRR |= (1<<26);//pull CS low
      
      //send SCK until card starts transmitting data
      do
	{
	  while( !(SPI1->SR & (1<<1)) );
	  *( (unsigned char*) &(SPI1->DR) ) = 0xFF;
	  while( !(SPI1->SR & (1<<0)) );
	  *buff = (unsigned char) SPI1->DR;
	  limit--;
	}
      while( (*buff != 0xFE) && limit );
      if(limit == 0) return RES_ERROR;
      
      //receive data to specified buffer
      for(i=0; i<512; i++)
	{
	  while( !(SPI1->SR & (1<<1)) );
	  *( (unsigned char*) &(SPI1->DR) ) = 0xFF;
	  while( !(SPI1->SR & (1<<0)) );
	  *(buff + i) = (unsigned char) SPI1->DR;
	}
      
      //receive and ignore CRC bytes
      for(i=0; i<2; i++)
	{
	  while( !(SPI1->SR & (1<<1)) );
	  *( (unsigned char*) &(SPI1->DR) ) = 0xFF;
	}
      
      while(SPI1->SR & (1<<7));//wait until SPI1 is no longer busy
      GPIOB->BSRR |= (1<<10);//pull CS high
      
      //send an extra 0xFF byte so card finishes its processing and is ready to receive next command
      while( !(SPI1->SR & (1<<1)) );
      *( (unsigned char*) &(SPI1->DR) ) = 0xFF;

      while(SPI1->SR & (1<<7));//wait until SPI1 is no longer busy
      return RES_OK;
    }
  else return RES_PARERR;
}

DRESULT disk_write (
	BYTE pdrv,	   /* Physical drive nmuber to identify the drive */
	const BYTE* buff,  /* Data to be written */
	DWORD sector,	   /* Start sector in LBA */
	UINT count	   /* Number of sectors to write */
)
{
  volatile unsigned int i;
  volatile unsigned int limit;
  volatile unsigned char response;
  
  if(pdrv != 0) return RES_PARERR;//only physical drive 0 is available
  if(count == 0) return RES_PARERR;//only single block writes are supported

  if(count == 1)
    {
      if(SD_SendCommand_R1(0x58, sector, 0x01)) return RES_ERROR;//CMD24 = write single block

      while(SPI1->SR & (1<<7));//wait until SPI1 is no longer busy
      GPIOB->BSRR |= (1<<26);//pull CS low

      //send start block token
      while( !(SPI1->SR & (1<<1)) );
      *( (unsigned char*) &(SPI1->DR) ) = 0xFE;

      //send data from specified buffer
      for(i=0; i<512; i++)
	{
	  while( !(SPI1->SR & (1<<1)) );
	  *( (unsigned char*) &(SPI1->DR) ) = *(buff+i);
	}

      //send fake CRC bytes
      for(i=0; i<2; i++)
	{
	  while( !(SPI1->SR & (1<<1)) );
	  *( (unsigned char*) &(SPI1->DR) ) = 0XFF;
	}

      while(SPI1->SR & (1<<7));//wait until SPI1 is no longer busy
      while(SPI1->SR & (1<<0)) response = (unsigned char) SPI1->DR;//empty RX FIFO
      
      //send SCK until card sends data response token
      limit = 10000;
      do
	{
	  while( !(SPI1->SR & (1<<1)) );
	  *( (unsigned char*) &(SPI1->DR) ) = 0xFF;
	  while( !(SPI1->SR & (1<<0)) );
	  response = (unsigned char) SPI1->DR;
	  limit--;
	}
      while( (response == 0xFF) && limit );
      if( ((response & 0x1F) != 0x05) || (limit == 0) ) return RES_ERROR;

      while(SPI1->SR & (1<<7));//wait until SPI1 is no longer busy
      while(SPI1->SR & (1<<0)) response = (unsigned char) SPI1->DR;//empty RX FIFO

      //send SCK until card keeps sending busy token back
      limit = 10000;
      do
	{
	  while( !(SPI1->SR & (1<<1)) );
	  *( (unsigned char*) &(SPI1->DR) ) = 0xFF;
	  while( !(SPI1->SR & (1<<0)) );
	  response = (unsigned char) SPI1->DR;
	  limit--;
	}
      while( (response == 0) && limit);
      if(limit == 0) return RES_ERROR;
      
      while(SPI1->SR & (1<<7));//wait until SPI1 is no longer busy
      GPIOB->BSRR |= (1<<10);//pull CS high

      //send an extra 0xFF byte so card finishes its processing and is ready to receive next command
      while( !(SPI1->SR & (1<<1)) );
      *( (unsigned char*) &(SPI1->DR) ) = 0xFF;

      while(SPI1->SR & (1<<7));//wait until SPI1 is no longer busy

      if( SD_SendCommand_R2(0x4D, 0x00000000, 0x01) ) return RES_ERROR;
      
      return RES_OK;
    }
  else return RES_PARERR;
}

DRESULT disk_ioctl (
	BYTE pdrv,   /* Physical drive number (0..) */
	BYTE cmd,    /* Control code */
	void* buff   /* Buffer to send/receive control data */
)
{
  if(pdrv != 0) return RES_PARERR;//only physical drive 0 is available
  
  switch(cmd)
    {
    case CTRL_SYNC:
      return RES_OK;
      break;

    case GET_SECTOR_COUNT:
      *((DWORD*) buff) = 1;
      return RES_OK;
      break;

    case GET_SECTOR_SIZE:
      *((WORD*) buff) = 512;
      return RES_OK;
      break;

    case GET_BLOCK_SIZE:
      *((DWORD*) buff) = 1;
      return RES_OK;
      break;

    case CTRL_TRIM:
      return RES_OK;
      break;
    }

  return RES_ERROR;
}

DWORD get_fattime (void)
{
  return (1<<21)|(1<<16);
}


//functions for direct communication to SD card

unsigned char SD_SendCommand_R1(unsigned char cmd, unsigned int arg, unsigned char crc)
{
  volatile unsigned char response = 0xFF;
  volatile unsigned char limit;

  GPIOB->BSRR = (1<<26);//set CS low
  
  //send command to SD card
  while( !(SPI1->SR & (1<<1)) );
  *( (unsigned char*) &(SPI1->DR) ) = cmd;
  while( !(SPI1->SR & (1<<1)) );
  *( (unsigned char*) &(SPI1->DR) ) = (unsigned char) ( (arg & (0xFF000000))>>24 );
  while( !(SPI1->SR & (1<<1)) );
  *( (unsigned char*) &(SPI1->DR) ) = (unsigned char) ( (arg & (0x00FF0000))>>16 );
  while( !(SPI1->SR & (1<<1)) );
  *( (unsigned char*) &(SPI1->DR) ) = (unsigned char) ( (arg & (0x0000FF00))>>8 );
  while( !(SPI1->SR & (1<<1)) );
  *( (unsigned char*) &(SPI1->DR) ) = (unsigned char) ( (arg & (0x000000FF))>>0 );
  while( !(SPI1->SR & (1<<1)) );
  *( (unsigned char*) &(SPI1->DR) ) = crc;

  while(SPI1->SR & (1<<7));//wait until SPI1 is not busy
  while(SPI1->SR & (1<<0)) response = (unsigned char) SPI1->DR;//empty the RX FIFO

  //get R1 response
  limit = 20;
  do
    {
      while( !(SPI1->SR & (1<<1)) );
      *( (unsigned char*) &(SPI1->DR) ) = 0xFF;
      while( !(SPI1->SR & (1<<0)) );
      response = (unsigned char) SPI1->DR;
      limit--;
    }
  while( (response & (1<<7)) && (limit != 0) );
  if(limit == 0) return RES_ERROR;

  while(SPI1->SR & (1<<7));//wait until SPI1 is not busy
  
  GPIOB->BSRR = (1<<10);//set CS high
  
  //send an extra 0xFF byte so card finishes its processing and is ready to receive next command
  while( !(SPI1->SR & (1<<1)) );
  *( (unsigned char*) &(SPI1->DR) ) = 0xFF;

  while(SPI1->SR & (1<<7));//wait until SPI1 is not busy
  return response;
}

unsigned short SD_SendCommand_R2(unsigned char cmd, unsigned int arg, unsigned char crc)
{
  volatile unsigned short response;
  volatile unsigned char limit;

  GPIOB->BSRR = (1<<26);//set CS low
  
  //send command to SD card
  while( !(SPI1->SR & (1<<1)) );
  *( (unsigned char*) &(SPI1->DR) ) = cmd;
  while( !(SPI1->SR & (1<<1)) );
  *( (unsigned char*) &(SPI1->DR) ) = (unsigned char) ( (arg & (0xFF000000))>>24 );
  while( !(SPI1->SR & (1<<1)) );
  *( (unsigned char*) &(SPI1->DR) ) = (unsigned char) ( (arg & (0x00FF0000))>>16 );
  while( !(SPI1->SR & (1<<1)) );
  *( (unsigned char*) &(SPI1->DR) ) = (unsigned char) ( (arg & (0x0000FF00))>>8 );
  while( !(SPI1->SR & (1<<1)) );
  *( (unsigned char*) &(SPI1->DR) ) = (unsigned char) ( (arg & (0x000000FF))>>0 );
  while( !(SPI1->SR & (1<<1)) );
  *( (unsigned char*) &(SPI1->DR) ) = crc;

  while(SPI1->SR & (1<<7));//wait until SPI1 is not busy
  while(SPI1->SR & (1<<0)) response = (unsigned char) SPI1->DR;//empty the RX FIFO

  //get R2 response
  limit = 20;
  do
    {
      while( !(SPI1->SR & (1<<1)) );
      *( (unsigned char*) &(SPI1->DR) ) = 0xFF;
      while( !(SPI1->SR & (1<<0)) );
      response = (unsigned char) SPI1->DR;
      limit--;
    }
  while( (response & (1<<7)) && (limit != 0) );
  if(limit == 0) return RES_ERROR;
  response = response<<8;
  while( !(SPI1->SR & (1<<1)) );
  *( (unsigned char*) &(SPI1->DR) ) = 0xFF;
  while( !(SPI1->SR & (1<<0)) );
  response |= (unsigned char) SPI1->DR;
  
  while(SPI1->SR & (1<<7));//wait until SPI1 is not busy  
  GPIOB->BSRR = (1<<10);//set CS high
  
  //send an extra 0xFF byte so card finishes its processing and is ready to receive next command
  while( !(SPI1->SR & (1<<1)) );
  *( (unsigned char*) &(SPI1->DR) ) = 0xFF;

  while(SPI1->SR & (1<<7));//wait until SPI1 is not busy
  return response;
}

void SD_SendCommand_R3(unsigned char cmd, unsigned int arg, unsigned char crc)
{
  volatile unsigned char i;
  volatile unsigned char limit;

  for(i=0; i<5; i++) R3[i] = 0xFF;

  GPIOB->BSRR = (1<<26);//set CS low
  
  //send command to SD card
  while( !(SPI1->SR & (1<<1)) );
  *( (unsigned char*) &(SPI1->DR) ) = cmd;
  while( !(SPI1->SR & (1<<1)) );
  *( (unsigned char*) &(SPI1->DR) ) = (unsigned char) ( (arg & (0xFF000000))>>24 );
  while( !(SPI1->SR & (1<<1)) );
  *( (unsigned char*) &(SPI1->DR) ) = (unsigned char) ( (arg & (0x00FF0000))>>16 );
  while( !(SPI1->SR & (1<<1)) );
  *( (unsigned char*) &(SPI1->DR) ) = (unsigned char) ( (arg & (0x0000FF00))>>8 );
  while( !(SPI1->SR & (1<<1)) );
  *( (unsigned char*) &(SPI1->DR) ) = (unsigned char) ( (arg & (0x000000FF))>>0 );
  while( !(SPI1->SR & (1<<1)) );
  *( (unsigned char*) &(SPI1->DR) ) = crc;

  while(SPI1->SR & (1<<7));//wait until SPI1 is not busy
  while(SPI1->SR & (1<<0)) R3[0] = (unsigned char) SPI1->DR;//empty the RX FIFO

  //get R3 or R7 response
  limit = 20;
  do
    {
      while( !(SPI1->SR & (1<<1)) );
      *( (unsigned char*) &(SPI1->DR) ) = 0xFF;
      while( !(SPI1->SR & (1<<0)) );
      R3[0] = (unsigned char) SPI1->DR;
      limit--;
    }
  while( (R3[0] & (1<<7)) && (limit != 0) );
  if(limit == 0) return;
  
  for(i=1; i<5; i++)
    {
      while( !(SPI1->SR & (1<<1)) );
      *( (unsigned char*) &(SPI1->DR) ) = 0xFF;
      while( !(SPI1->SR & (1<<0)) );
      R3[i] = (unsigned char) SPI1->DR;
    }
  
  while(SPI1->SR & (1<<7));//wait until SPI1 is not busy
  
  GPIOB->BSRR = (1<<10);//set CS high
  
  //send an extra 0xFF byte so card finishes its processing and is ready to receive next command
  while( !(SPI1->SR & (1<<1)) );
  *( (unsigned char*) &(SPI1->DR) ) = 0xFF;

  while(SPI1->SR & (1<<7));//wait until SPI1 is not busy
  return;
}
