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
DEVICE  = atmega16
F_CPU   = 1000000
FUSE_L  = 0xE1
FUSE_H  = 0x99
PROG_ID = bsd
AVRDUDE = avrdude -c $(PROG_ID) -p $(DEVICE)

CFLAGS  = -I../ -I. -Iconfig -I../dev -I../io -I../util -DDEBUG=1
OBJECTS = main.o ../dev/nrf905.o ../io/spi.o ../io/usart.o ../util/debug.o ../util/rc2.o

COMPILE = $(CC) -Wall -Os -DF_CPU=$(F_CPU) $(CFLAGS) -mmcu=$(DEVICE)

hex: masterfw.hex

fuse:
	@[ "$(FUSE_H)" != "" -a "$(FUSE_L)" != "" ] || \
		{ echo "*** Edit Makefile and choose values for FUSE_L and FUSE_H!"; exit 1; }
	$(AVRDUDE) -U hfuse:w:$(FUSE_H):m -U lfuse:w:$(FUSE_L):m

flash: masterfw.hex
	$(AVRDUDE) -U flash:w:masterfw.hex:i -E noreset

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

masterfw.elf: $(OBJECTS)
	$(COMPILE) -o masterfw.elf $(OBJECTS)

masterfw.hex: masterfw.elf
	rm -f masterfw.hex masterfw.eep.hex
	avr-objcopy -j .text -j .data -O ihex masterfw.elf masterfw.hex
	avr-size masterfw.hex

disasm:	masterfw.elf
	avr-objdump -d masterfw.elf

cpp:
	$(COMPILE) -E main.c
