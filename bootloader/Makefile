# 16k bootloader addr = 0x3800
# 8k bootloader addr = 0x1800
# 32k bootloader addr for 4k bootloader = 0x7000
# 32k bootloader addr for 2k bootloader = 0x7800

F_CPU = 12000000
DEVICE = atmega328p
BOOTLOADER_ADDRESS = 0x7000

#Fuses
#atmega8: -U lfuse:w:0x7F:m -U hfuse:w:0xD8:m -U lock:w:0xCF:m
#atmega88: -U lfuse:w:0xFF:m -U hfuse:w:0xDF:m -U efuse:w:0xF8:m -U lock:w:0xCF:m
#atmega88p: -U lfuse:w:0xFF:m -U hfuse:w:0xDF:m -U efuse:w:0xF8:m -U lock:w:0xCF:m
#atmega168: -U lfuse:w:0xFF:m -U hfuse:w:0xDF:m -U efuse:w:0xF8:m -U lock:w:0xCF:m
#atmega168p: -U lfuse:w:0xFF:m -U hfuse:w:0xDF:m -U efuse:w:0xF8:m -U lock:w:0xCF:m
#atmega328p (4k bootloader): -U lfuse:w:0xFF:m -U hfuse:w:0xD8:m -U efuse:w:0xFF:m -U lock:w:0x0F:m
#atmega328p (2k bootloader): -U lfuse:w:0xFF:m -U hfuse:w:0xDA:m -U efuse:w:0xFF:m -U lock:w:0xCF:m

CC = avr-gcc

# Options:
DEFINES = #-DDEBUG_LEVEL=2
CFLAGS = -Wall -Os -I. -mmcu=$(DEVICE) -DF_CPU=$(F_CPU) $(DEFINES)
LDFLAGS = -Wl,--section-start=.text=$(BOOTLOADER_ADDRESS)
FUSEBITS = -U lfuse:w:0xFF:m -U hfuse:w:0xD8:m -U efuse:w:0x05:m -U lock:w:0x0F:m

OBJECTS =  usbdrv/usbdrvasm.o main.o #usbdrv/oddebug.o

# symbolic targets:
all: main.hex

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.S.o:
	$(CC) $(CFLAGS) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(CC) $(CFLAGS) -S $< -o $@

fuse:
	avrdude -c usbasp -p m328p -F $(FUSEBITS)

flash:
	avrdude -c usbasp -p m328p -F -B 1 -U flash:w:main.hex

clean:
	rm -f main.hex main.bin *.o usbdrv/*.o main.s usbdrv/oddebug.s usbdrv/usbdrv.s

main.bin:	$(OBJECTS)
	$(CC) $(CFLAGS) -o main.bin $(OBJECTS) $(LDFLAGS)

main.hex:	main.bin
	avr-objcopy -j .text -j .data -O ihex main.bin main.hex
	avr-size main.hex

disasm:	main.bin
	avr-objdump -d main.bin

cpp:
	$(CC) $(CFLAGS) -E main.c

# Special rules for generating hex files for various devices and clock speeds
ALLHEXFILES = hexfiles/mega8_12mhz.hex hexfiles/mega8_15mhz.hex hexfiles/mega8_16mhz.hex \
	hexfiles/mega88_12mhz.hex hexfiles/mega88_15mhz.hex hexfiles/mega88_16mhz.hex hexfiles/mega88_20mhz.hex\
	hexfiles/mega168_12mhz.hex hexfiles/mega168_15mhz.hex hexfiles/mega168_16mhz.hex hexfiles/mega168_20mhz.hex

allhexfiles: $(ALLHEXFILES)
	$(MAKE) clean
	avr-size hexfiles/*.hex
	avr-size hexfiles/*.hex >> results.txt

$(ALLHEXFILES):
	@[ -d hexfiles ] || mkdir hexfiles
	@device=`echo $@ | sed -e 's|.*/mega||g' -e 's|_.*||g'`; \
	clock=`echo $@ | sed -e 's|.*_||g' -e 's|mhz.*||g'`; \
	addr=`echo $$device | sed -e 's/\([0-9]\)8/\1/g' | awk '{printf("%x", ($$1 - 2) * 1024)}'`; \
	echo "### Make with F_CPU=$${clock}000000 DEVICE=atmega$$device BOOTLOADER_ADDRESS=$$addr"; \
	$(MAKE) clean; \
	$(MAKE) main.hex F_CPU=$${clock}000000 DEVICE=atmega$$device BOOTLOADER_ADDRESS=$$addr DEFINES=-DUSE_AUTOCONFIG=1
	mv main.hex $@
