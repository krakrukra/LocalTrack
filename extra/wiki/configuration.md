LocalTrack provides several configuration options with which you can  
further specify how exactly the device should behave once powered on.  
  
Such behaviour changes are done through the use of configuration  
commands. These commands are placed inside of a text file named  
**config.txt** in the root directory of LocalTrack's USB storage.  
  
Here is a quick summary of the rules:  
  
1. Text encoding must be ASCII. Quotation marks used here are not parts  
of commands and are just used to indicate start and end of a command.  
  
2. All configuration commands must be placed at the start of  
the line and terminated with a newline character.  
  
3. Some commands take decimal number as argument, which follows the  
command keyword. These commands have **n** character shown here, which  
should be replaced with an actual number in config.txt; The command  
and it's argument must be separated by exactly one space character.  
  
4. If some command is not present in config.txt default setting for  
the corresponding configuration value is used.  
  
### Below is a complete list of all configuration commands:  
  
* "FIX_INTERVAL **n**"   --- sets fix interval in milliseconds  
* "UTC_OFFSET +**n**"   --- sets positive offset of local time relative to UTC time  
* "UTC_OFFSET -**n**"   --- sets negative offset of local time relative to UTC time  
* "ALLOW_SHORT_TRACKS"   --- save all tracks, even if shorter than 4 minutes  
* "DISABLE_1PPS_LED"   --- prevents blue status LED from blinking every second 
* "ENTER_DFU_MODE"   --- enter USB DFU mode next time device is plugged in  
* "REPORT_SPEED"   --- add speed info (in meters/sec) to each trackpoint  
* "REPORT_COURSE"   --- add course info (in degrees) to each trackpoint  
* "REPORT_NUMSAT"   --- add number of sattelites used to each trackpoint  
* "REPORT_FIXTYPE"   --- add fix type info (none, 2d or 3d) to each trackpoint  
* "MASS_ERASE"   --- unrecoverably erase all flash memory blocks  
  
---
  
1. "FIX_INTERVAL **n**" command allows user to change default fix interval of 1000ms.  
Accepted values are integers in the range of 200 to 10000. Lower values will  
give you more detailed tracks (more trackpoints) but also take more disk space.  
For example, "FIX_INTERVAL 200" will result in making 5 trackpoints per second.  
  
2. "UTC_OFFSET +**n**" and "UTC_OFFSET -**n**" commands set positive/negative offset  
in minutes of local time relative to UTC time. Accepted values are integers  
from 0 to 1440, default value is 0. Local time is used in track and file names.  
For example, "UTC_OFFSET +180" command will set local timezone to UTC+3.  
  
3. "ALLOW_SHORT_TRACKS" command makes the device save all valid tracks, while by  
default only tracks of 4 minutes or more are kept to prevent spam. This command  
is intended for debugging / testing and it is not recommended in normal use.  
  
4. "DISABLE_1PPS_LED" command disables blue status LED, which normally blinks  
once per second while receiver gets valid position fixes. The main purpose  
of this command is to make device a bit more covert in complete darkness.  
  
5. "ENTER_DFU_MODE" command will make the device enter DFU mode next  
time it is connected to host PC via USB, while normally it would act as USB  
disk. To prevent repeated DFU entry config.txt file will be deleted. In order  
to exit DFU mode you will have to put the power switch in OFF position and  
disconnect the USB cable.  
  
6. "REPORT_SPEED", "REPORT_COURSE", "REPORT_NUMSAT", "REPORT_FIXTYPE"  
commands add additional information to each GPX trackpoint. By default, all of  
this information is omitted. If enabled, speed is reported in meters per second,  
course is reported in degrees relative to true north in WGS84 datum,  
number of sattelites will show you a number (1 to 12) of sattelites used  
to make position fix, fix type will give you a status string (2d or 3d).  
  
7. "MASS_ERASE" command erases all the data blocks on the device which contain,  
or have previously contained any data. The process takes just a few seconds.  
Keep in mind, that any stored files and partition table will be lost. This command  
is mainly intended to be a quick way to unrecoverably erase all the data on the  
device, without having to manually overwrite all logical blocks for a couple of times.  
  