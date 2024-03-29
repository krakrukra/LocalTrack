LocalTrack provides end user an ablity to update firmware  
over USB using DFU bootloader. To do so, you will need to  
install some dedicated software on your PC and connect the  
LocalTrack to this same PC, while your device is in DFU mode.  
Then, you feed a particular firmware image in DfuSe format  
to this dedicated software and wait for it to finish the update.  
The latest firmware image is available in the repository, in  
**/firmware/firmware_13nnn.dfu** file, where nnn is a number  
which stands for firmware version (eg. firmware_13001.dfu);  
  
There are 2 ways to enter DFU mode. If your device is already  
functional, you can get into DFU mode by adding "ENTER_DFU_MODE"  
configuration command to config.txt file. Once you disconnect  
LocalTrack from PC and then connect it again, device will delete  
config.txt and enter DFU mode instead of usual disk mode. To  
exit DFU mode you need to disconnect device from the host PC  
and power the device off using the switch.  
  
If the previous firmware on the device is corrupted/nonexistent,  
you will have to power the device off with the switch and then  
short out boot0 pin to 3.3V line. This will require you to take  
the lid off. Be careful, as there are wires connecting the board  
with battery mounted on the lid. You will then see a 5 pin header  
where you should connect 2 pins (using a jumper or some other  
means) which are the farthest away from a nearby screw. Only then  
connect the device via USB and it will enter DFU mode right away;  
  
---
  
#### linux (debian) DFU update procedure  
  
1. install dfu-util (sudo apt-get install dfu-util)  
2. download this repository (.zip) to your PC and unpack it  
3. make sure device power switch is turned OFF  
4. connect the device to PC with a USB cable  
5. add "ENTER_DFU_MODE" command to config.txt  
6. eject the drive and disconnect device from PC  
7. wait for 1-2 seconds and connect it back again  
8. open terminal window in /firmware/ directory  
9. run command: dfu-util -a 0 -D firmware_13nnn.dfu  
10. wait for update process to finish  
11. disconnect your device from PC  
  
#### windows DFU update procedure  
  
1. download and install [DfuSe demo](https://www.st.com/en/development-tools/stsw-stm32080.html) software  
2. download this repository (.zip) to your PC and unpack it  
3. make sure device power switch is turned OFF  
4. connect the device to PC with a USB cable  
5. add "ENTER_DFU_MODE" command to config.txt  
6. eject the drive and disconnect device from PC  
7. wait for 1-2 seconds and connect it back again  
8. start DfuSe demo, click on "Choose" button (bottom right)  
9. select the /firmware/firmware_13nnn.dfu file  
10. select "Internal Flash" as target region (Target ID = 00)  
11. click on "Upgrade" button (bottom right)  
12. confirm by clicking "yes", wait for update process  
13. disconnect your device from PC  
  