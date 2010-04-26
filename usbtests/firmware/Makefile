DEVICE  = atmega32
F_CPU   = 12000000
FUSE_L  = 0xEF
FUSE_H  = 0x89
PROG_ID = bsd
AVRDUDE = avrdude -c $(PROG_ID) -p $(DEVICE) # edit this line for your programmer
REMOTE_HOST = twarzromana.czystybeton.pl
REMOTE_DIR = __avr

CFLAGS  = -I. -Iconfig -Idev -Iio -Iio/usbdrv -DDEBUG_LEVEL=0 -DDEBUG=1
OBJECTS = main.o dev/nrf905.o dev/pcf8583.o io/owi.o io/spi.o io/twi.o io/usart.o io/usbdrv/oddebug.o io/usbdrv/usbdrv.o io/usbdrv/usbdrvasm.o

COMPILE = avr-gcc -Wall -Os -DF_CPU=$(F_CPU) $(CFLAGS) -mmcu=$(DEVICE)

help:
	@echo "This Makefile has no default rule. Use one of the following:"
	@echo "make hex ....... to build main.hex"
	@echo "make program ... to flash fuses and firmware"
	@echo "make fuse ...... to flash the fuses"
	@echo "make flash ..... to flash the firmware (use this on metaboard)"
	@echo "make clean ..... to delete objects and hex file"

hex: firmware.hex

program: flash fuse

# rule for programming fuse bits:
fuse:
	@[ "$(FUSE_H)" != "" -a "$(FUSE_L)" != "" ] || \
		{ echo "*** Edit Makefile and choose values for FUSE_L and FUSE_H!"; exit 1; }
	$(AVRDUDE) -U hfuse:w:$(FUSE_H):m -U lfuse:w:$(FUSE_L):m

rflash: rsync
	ssh $(REMOTE_HOST) "cd $(REMOTE_DIR); make clean flash" || exit 1
	ssh $(REMOTE_HOST) "rm -rf $(REMOTE_DIR)" || exit 1

rreset: rsync
	ssh $(REMOTE_HOST) "cd $(REMOTE_DIR); make reset" || exit 1
	ssh $(REMOTE_HOST) "rm -rf $(REMOTE_DIR)" || exit 1

rsync:
	ssh $(REMOTE_HOST) "mkdir -p $(REMOTE_DIR)" || exit 1
	rsync --rsh=ssh -r *  $(REMOTE_HOST):$(REMOTE_DIR) || exit 1


# rule for uploading firmware:
flash: firmware.hex
	$(AVRDUDE) -U flash:w:firmware.hex:i -E noreset

# reset
reset: 
	$(AVRDUDE) -E noreset

# rule for deleting dependent files (those which can be built by Make):
clean:
	rm -f *.elf *.hex $(OBJECTS)

# Generic rule for compiling C files:
.c.o:
	$(COMPILE) -c $< -o $@

# Generic rule for assembling Assembler source files:
.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

# Generic rule for compiling C to assembler, used for debugging only.
.c.s:
	$(COMPILE) -S $< -o $@

# file targets:

firmware.elf: $(OBJECTS)
	$(COMPILE) -o firmware.elf $(OBJECTS)

firmware.hex: firmware.elf
	rm -f firmware.hex firmware.eep.hex
	avr-objcopy -j .text -j .data -O ihex firmware.elf firmware.hex
	avr-size firmware.hex

# debugging targets:

disasm:	firmware.elf
	avr-objdump -d firmware.elf

cpp:
	$(COMPILE) -E main.c
