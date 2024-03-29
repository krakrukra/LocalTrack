#### Русская версия документации доступна [здесь](https://github.com/krakrukra/LocalTrack/blob/master/extra/wiki/rus/home.md)  
  
LocalTrack GPS logger uses internal 96MiB flash memory to store  
location data. It requires FAT16 or FAT32 filesystem on the first  
partition (MBR partition table), and will place all the track  
files there. Track files are saved in GPX format, which is widely  
used and should be easily accepted by many different mapping  
softwares. If necessary, you can use a program for file format  
conversion, for example [gpsbabel](https://www.gpsbabel.org).  
  
Once you power the device ON with the switch, it will try to receive  
signal from available GPS sattelites. If a valid position fix is acquired,  
a new trackpoint will be added to current GPX track. After a set period  
of time elapses (1s by default) the next point will be added. Each GPX track  
has a name in this format: **HH:MM:SS**, where HH is hour in 24-hour  
format, MM is minute, SS is second of when the GPX track was started.  
Tracks are grouped into files, with file names like this: **YY-MM-DD.GPX**,  
where YY is year (since 2000), MM is a month number (from 1 to 12),  
DD is day of month (1 to 31) of when the file was created. So, a file  
called 21-10-05.GPX will contain tracks which started on Oct. 5, 2021.  
  
To increase battery life, device enters sleep mode when it sees no  
movement for 120 seconds. When this happends, current track will be  
completed. After some movement activity is detected again, device  
wakes up and all the new incoming data is put into a new track.  
After enough data was saved, you retrieve the device and connect it  
to your PC via USB interface. It will show up as a mass storage  
device (USB flash drive) and you will be able to take all the GPX  
files from there and feed them to some mapping software, eg. [viking](https://sourceforge.net/projects/viking).  
  
Optionally, you can have a **config.txt** file in the root directory.  
This is the file where configuration commands can be placed.  
These commands alter the behaviour of LocalTrack, such as  
setting different fix interval, adding extra information to each  
trackpoint, performing mass erase operations on flash memory, etc.  
You can find more info about that in dedicated wiki pages.  
  
#### Read other wiki pages:  
* [configuration](https://github.com/krakrukra/LocalTrack/wiki/configuration)  
* [status file](https://github.com/krakrukra/LocalTrack/wiki/status)  
* [DFU bootloader](https://github.com/krakrukra/LocalTrack/wiki/bootloader)  
  
#### Some tutorial videos are available on my [youtube channel](https://www.youtube.com/channel/UC8HZCV1vNmZvp7ci1vNmj7g)  
  