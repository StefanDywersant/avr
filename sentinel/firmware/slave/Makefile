#
# Sentinel
# Multi-aquarium controller.
#
# Copyright: (c) 2010 by Karol Maciaszek
# E-mail   : <karol.maciaszek@gmail.com>
# WWW      : http://www.czystybeton.pl/
#
# $Id$
#

CC      = avr-gcc
DEVICE  = atmega32
F_CPU   = 12000000
FUSE_L  = 0xEF
FUSE_H  = 0x89
PROG_ID = bsd
AVRDUDE = avrdude -c $(PROG_ID) -p $(DEVICE)

CFLAGS  = -I.. -I. -Iconfig -I../dev -I../io -I../io/usbdrv -DDEBUG_LEVEL=0 -DDEBUG=1
OBJECTS = main.o ../debug.o ../dev/nrf905.o ../dev/pcf8583.o ../io/owi.o ../io/spi.o ../io/twi.o ../io/usart.o ../io/usbdrv/oddebug.o ../io/usbdrv/usbdrv.o ../io/usbdrv/usbdrvasm.o

COMPILE = $(CC) -Wall -Os -DF_CPU=$(F_CPU) $(CFLAGS) -mmcu=$(DEVICE)

hex: slavefw.hex

fuse:
	@[ "$(FUSE_H)" != "" -a "$(FUSE_L)" != "" ] || \
		{ echo "*** Edit Makefile and choose values for FUSE_L and FUSE_H!"; exit 1; }
	$(AVRDUDE) -U hfuse:w:$(FUSE_H):m -U lfuse:w:$(FUSE_L):m

flash: slavefw.hex
	$(AVRDUDE) -U flash:w:slavefw.hex:i -E noreset

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

slavefw.elf: $(OBJECTS)
	$(COMPILE) -o slavefw.elf $(OBJECTS)

slavefw.hex: slavefw.elf
	rm -f slavefw.hex slavefw.eep.hex
	avr-objcopy -j .text -j .data -O ihex slavefw.elf slavefw.hex
	avr-size slavefw.hex

disasm:	slavefw.elf
	avr-objdump -d slavefw.elf

cpp:
	$(COMPILE) -E main.c
