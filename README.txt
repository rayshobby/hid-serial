========================================================

This folder contains Arduino code and host software to
demonstrate HID-class USB serial communication for AVRs
using V-USB. The code and circuit design are based on V-USB
http://www.obdev.at/products/vusb/index.html
and USnoobie. It is designed to work for ATmega328 but
can be adapted to other AVR microcontrollers as well.

The software is written by Ray Wang at Rayshobby LLC
and published under the Creative Commons Attribution-
ShareAlike (CC-SA) 3.0 license. 

========================================================

The folders are organized as follows:

- 'schematic' contains the circuit schematic and part list.

- 'bootloader' contains the modified USnoobie bootloader
  The bootloader is optional, but once flashed, it allows
  the mcu to bootload (by pressing the button during power-on
  or reset) as a usbasp programmer, so you will not need
  any external programer to flash a program.

- 'arduino code' contains the Arduino library for HIDSerial.
  To use it:
  * copy boards.txt (from hardware/arduino folder)
    to the corresponding folder in your arduino installation
    directory, and the HIDSerial library to your arduino's 
    libraries directory.
  * Run Arduino (the recommended version is 1.0.5 or 1.0.4).
  * Make sure you select 'USnoobie' from Tools -> Boards
  * If you use the USnoobie bootloader, select 'USBasp' from
    Tools -> Programmer.
  * Select any provided example from File -> Examples -> HIDSerial
  * Upload
  
- 'host software' contains the standalone applications and Processing
   source code for HIDSerialMonitor. Please check the README.txt
   therein.
   
========================================================
