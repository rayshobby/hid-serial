==========================================================================

NOTE: the original code by Frank Zhao has been slightly modified to use
button on pin PD4 for bootloading condition.

Please check the burn.bat for recommende fuse bits.

==========================================================================

bootloader files for USnooBie, base off USBaspLoader

all hex files generated August 16 2010 by Frank Zhao for USnooBie bootloader, WinAVR20060421 is used to for the non-328 variants, and WinAVR20100110 is used for the 328P variant, V-USB 20100715 is used for usbdrv

due to the size limits of the bootloader, the ATmega8/48/88/168 variants requires WinAVR 20060421 while ATmega328P requires any WinAVR version that supports ATmega328P

fuses.txt tells you the recommended fuse bits to use with each chip

If the size of the bootloader exceeds 2048 bytes, then you must use the 4k bootloader file and fuses, if it is under or exactly 2048 bytes, then you may use the 2k bootloader files and fuses. Only ATmega328P is capable of using bootloaders exceeding 2048 bytes.
