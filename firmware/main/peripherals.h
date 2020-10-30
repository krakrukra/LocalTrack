#ifndef PERIPHERALS_H
#define PERIPHERALS_H

//structure describing information extracted from the last processed GPRMC message
typedef struct
{
  unsigned int time;//decimal number in this format: HHMMSSSSS (in units of hour, minute, second/1000)
  int latitude; //signed decimal number in this format:  DDDDDDDDD (in units of degree/10^7)
  int longitude;//signed decimal number in this format: DDDDDDDDDD (in units of degree/10^7)
  unsigned int speed;//decimal number in this format: SSSSSSSSS (in units of meters_per_second/10^3)
  unsigned int course;//decimal number in this format: DDDDD (in units of degree/100)
  unsigned int date;//decimal number in this format: DDMMYY (in units of day, month, year)
  unsigned char dataStatus;//0 means data is invalid, 1 means data is valid
} RMCdata_TypeDef;

//structure describing information extracted from the last processed GPGGA message
typedef struct
{
  unsigned int time;//decimal number in this format: HHMMSSSSS (in units of hour, minute, second/1000)
  int latitude;// signed decimal number in this format:  DDDDDDDDD (in units of degree/10^7)
  int longitude;//signed decimal number in this format: DDDDDDDDDD (in units of degree/10^7)
  int altitude;// signed decimal number in this format: MMMMMMM (in units of meter/1000)
  unsigned short hdop;//decimal number in this format: NNNN (in units of 1/100)
  unsigned char  numSatUsed;//number of sattelites used for position fix
  unsigned char  dataStatus;//0 means data is invalid, 1 means data is valid
} GGAdata_TypeDef;

//structure describing information extracted from the last processed GPGSA message
typedef struct
{
  unsigned short pdop;//decimal number in this format: NNNN (in units of 1/100)
  unsigned short hdop;//decimal number in this format: NNNN (in units of 1/100)
  unsigned short vdop;//decimal number in this format: NNNN (in units of 1/100)
  unsigned char  fixType;//1 means fix not available, 2 means fixtype = 2D, 3 means fixtype = 3D
} GSAdata_TypeDef;

//structure for GPS logger state machine
typedef struct
{
  RMCdata_TypeDef RMCdata;//holds information extracted from the last processed GPRMC message
  GGAdata_TypeDef GGAdata;//holds information extracted from the last processed GPGGA message
  GSAdata_TypeDef GSAdata;//holds information extracted from the last processed GPGSA message
  unsigned int   SupplyVoltage;//holds last measurement of MCU supply voltage in units of 10uV
  unsigned int   BatteryVoltage;//holds last measurement of battery voltage in units of 10uV
  unsigned int   TrackFileOffset;//holds read/write pointer into the file where current GPX track starts
  unsigned char* NMEApointer;//pointer to the next byte that will be processed in NMEAbuffer[]
    signed short UTCoffset;//holds offset in minutes of local time/date from UTC time/date
  unsigned short GPXsize;//holds how many bytes should be written to file from GPXbuffer[]
  unsigned char  FileName[13];//name of file where to save current GPX track, formatted as YY-MM-DD.GPX (year-month-day.GPX)
  unsigned char  TrackName[9];//name of current GPX track, formatted as HH:MM:SS (hour:minute:second)
  unsigned char  FixInterval[14];//SIM28 command to set position fix interval in milliseconds

  unsigned char ConfigFlags;//holds config flag bitmask with meanings of each bit specified below:
  // (1<<0) ShortTrackFlag; 1 means short tracks are allowed, 0 means short tracks are deleted
  // (1<<1) NoLedFlag; 1 means prevent SIM28 from turning on 1PPS LED, 0 allows 1PPS LED to blink
  // (1<<2) DFUmodeFlag; 1 means next diskmode entry is replaced with DFU entry, 0 means normal operation
  // (1<<3) Reserved
  // (1<<4) ReportSpeedFlag; 1 means add speed information into GPX trackpoints, 0 means skip this field
  // (1<<5) ReportCourseFlag; 1 means add course information into GPX trackpoints, 0 means skip this field
  // (1<<6) ReportNumSatFlag; 1 means add number of sattelites used into GPX trackpoints, 0 means skip this field
  // (1<<7) ReportFixTypeFlag; 1 means add fix type information into GPX trackpoints, 0 means skip this field
} LoggerInfo_TypeDef;

//ADXL345 accelerometer related functions
void adxl_init();
void adxl_clear();
void adxl_standby();

//SIM28 GPS receiver related functions
void sim28_init();
void sim28_sleep();
void sim28_send(char* cmd);
unsigned char sim28_receive(char* referenceString);

//generic functions
void restart_tim2(unsigned int time);
void restart_tim3(unsigned short time);

#endif //PERIPHERALS_H
