avrdude -c usbtiny -p m328p -F -U lfuse:w:0xFF:m -U hfuse:w:0xD8:m -U efuse:w:0x05:m -U lock:w:0x0F:m
avrdude -c usbtiny -p m328p -F -B 1 -U flash:w:main.hex
