#
# Ambilight
# Aktywne podświetlenie monitora.
#
# Copyright: (c) 2010 by Karol Maciaszek
# E-mail   : <karol.maciaszek@gmail.com>
# WWW      : http://www.czystybeton.pl/
#
# $Id$
#

CC      = avr-gcc
DEVICE  = atmega32
F_CPU   = 20000000
FUSE_L  = 0xEF
FUSE_H  = 0xC9
PROG_ID = bsd
AVRDUDE = avrdude -c $(PROG_ID) -p $(DEVICE)

CFLAGS  = -I. -Iconfig -Idev -Iio -Iio/usbdrv -Iutil -DDEBUG=1
OBJECTS = main.o dev/rgbled.o io/usart.o io/usbdrv/usbdrv.o io/usbdrv/usbdrvasm.o util/debug.o

COMPILE = $(CC) -Wall -Os -DF_CPU=$(F_CPU) $(CFLAGS) -mmcu=$(DEVICE)

hex: fw.hex

fuse:
	@[ "$(FUSE_H)" != "" -a "$(FUSE_L)" != "" ] || \
		{ echo "*** Edit Makefile and choose values for FUSE_L and FUSE_H!"; exit 1; }
	$(AVRDUDE) -U hfuse:w:$(FUSE_H):m -U lfuse:w:$(FUSE_L):m

flash: fw.hex
	$(AVRDUDE) -U flash:w:fw.hex:i -E noreset

reset: 
	$(AVRDUDE) -E noreset

clean:
	rm -f *.elf *.hex $(OBJECTS)

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

fw.elf: $(OBJECTS)
	$(COMPILE) -o fw.elf $(OBJECTS)

fw.hex: fw.elf
	rm -f fw.hex fw.eep.hex
	avr-objcopy -j .text -j .data -O ihex fw.elf fw.hex
	avr-size fw.hex

disasm:	fw.elf
	avr-objdump -d fw.elf

cpp:
	$(COMPILE) -E main.c
