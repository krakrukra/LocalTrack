Every time device is plugged into a host PC it saves latest  
status data into a file inside of it's own root directory.  
Name of the file is **STATUS.TXT**. This status data includes  
battery charge (in volts and % of full charge), operation  
mode (Autonomous / DGPS / Fix not available), number of  
visible sattelites (0 to 12); PDOP, HDOP, VDOP values (0 to 50)  
These values are sampled at the moment when USB connection  
is established, and will only be updated again if device is  
disconnected and connected back to PC. Status data also  
includes the maximum number of GPS sattelites in sight since  
device was powered on or since last status file was created.  
Status data can look something like this:  
  
Battery status: 4.12V 91%  
Operation mode: Autonomous  
Satellites used now: 6  
Max number of satellites: 9  
PDOP: 1.74  
HDOP: 1.45  
VDOP: 0.96  
  
Battery status is also saved every time a new GPX track  
has started, and placed into track's comment string, so  
you can monitor historical data this way.  
Max number of sattelites value is useful in cases when  
GPS signal is weak or jammed so you will know why you  
are not getting any data recorded in such cases.  
PDOP means Position Dilution of Precision, HDOP -  
Horizontal Dilution of Precision, VDOP - Vertical  
Dilution of Precision. These values characterise how  
precise your current GPS fix is, the colser this value  
is to 0 the better. Maximum reported value is 50, so  
when value 50 is reported, the actual DOP may be much  
larger.  
  