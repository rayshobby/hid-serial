=============
Dec 16 Update

- Fixed issue of host software printing garbage after running for a while
=============

This folder contains the host software (HIDSerialMonitor)
for HID-class serial communication. The precompiled programs
for Linux, Mac, and Windows can be found in their
correspondingsub-folder. These precompiled programs are stand-
alone applications and do not require any additional software.

On Windows, if you encounter an error or the program cannot run,
check if you have installed the Microsoft Visual C++ Redistributable
package. If not, you need to download and install that.

To use the serial monitor, you need to first click on 'Connect'
to find and connect to the device. Then the serial data from the 
device will be shown in the text area, and you can send serial
data to the device by typing strings in the text field and click
on 'Send'. 

To compile the code yourself, you need to install Processing
(http://processing.org/), and copy the libraries (G4P, and hidapi)
to your Processing's libraries folder. Then load HIDSerialMonitor.pde
in Processing and click on 'run'. 
