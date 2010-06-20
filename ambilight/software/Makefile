# Ambilight
# Aktywne podświetlenie monitora.
#
# Copyright: (c) 2010 by Karol Maciaszek
# E-mail   : <karol.maciaszek@gmail.com>
# WWW      : http://www.czystybeton.pl/
#
# $Id$
  
USBFLAGS=`pkg-config --cflags libusb-1.0`
USBLIBS=`pkg-config --libs libusb-1.0`

CC=gcc
CFLAGS=-O -Wall $(USBFLAGS)
LIBS=$(USBLIBS)

OBJ=main.o
PROGRAM=sentinel

all: $(PROGRAM)

$(PROGRAM): $(OBJ)
	$(CC) -o $(PROGRAM) $(OBJ) $(LIBS)

strip: $(PROGRAM)
	strip $(PROGRAM)

clean:
	rm -f $(OBJ) $(PROGRAM)

.c.o:
	$(CC) $(ARCH_COMPILE) $(CFLAGS) -c $*.c -o $*.o
