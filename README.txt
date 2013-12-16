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
  Note that the circuit uses a 12MHz crystal (or resonator).
  This is the lowest frequency clock supported by V-USB.
  You need to set the mcu fuse bits accordingly to use external
  crystal oscillator. See the 'bootloader' folder below.

- 'bootloader' contains the modified USnoobie bootloader
  The bootloader is optional, but once flashed, it allows
  the mcu to bootload (by pressing the button during power-on
  or reset) as a usbasp programmer, so you will not need
  any external programer to flash a program.
  
  Check the 'burn.bat' file in the 'bootloader' folder for the
  recommended fuse bits.
  
- 'arduino code' contains the Arduino library for HIDSerial.
  To use it:
  * copy boards.txt (from hardware/arduino folder)
    to the corresponding folder in your arduino installation
    directory, and the HIDSerial library to your arduino's 
    libraries directory.
  * Run Arduino (the recommended version is 1.0.5 or 1.0.4).
  * Make sure you select 'USnoobie' from Tools -> Boards
  * If you use the USnoobie bootloader, select 'USBasp' from
    Tools -> Programmer. If not, select the name of the external
    ISP programmer you are using to flash the program.
  * Select any provided example from File -> Examples -> HIDSerial
  * Upload the example code to ATmega328.  

- 'host software' contains the standalone applications and Processing
   source code for HIDSerialMonitor. Please check the README.txt
   therein.

- Using the Arduino HIDSerial library:
  * The library provides several examples for you to learn how to
    get started using the API functions.
  * The HIDSerial class is similar to Arduino's Serial class. To
    use it, you need to first define a class variable, e.g.
    
    HIDSerial serial;
    
    Then, in your setup() function, call:
    
    serial.begin();
    
    Next, in your loop() function, you can use:
    
    serial.print(...);
    serial.println(...);
    serial.write(...);
    
    to print values to the host. You can also use:
    
    serial.available();
    
    to check if there is any incoming data, and
    
    serial.read(...);
    
    to read the incoming data to a char buffer. Again,these are similar
    to the Arduino's Serial class. The ***main difference***, though, 
    is that in the loop() you need to call
    
    serial.poll();
    
    as often as you can -- since the USB tasks are simulated in software,
    calling serial.poll() frequently ensures the mcu has ample time to 
    perform USB tasks. This requirement can be relieved in future updates,
    but for you please cope with the requirement.

========================================================
