USBFLAGS=`pkg-config --cflags libusb-1.0`
USBLIBS=`pkg-config --libs libusb-1.0`

CC=gcc
CFLAGS=-O -Wall $(USBFLAGS)
LIBS=$(USBLIBS)

OBJ=main.o
PROGRAM=usbtests

all: $(PROGRAM)

$(PROGRAM): $(OBJ)
	$(CC) -o $(PROGRAM) $(OBJ) $(LIBS)

strip: $(PROGRAM)
	strip $(PROGRAM)

clean:
	rm -f $(OBJ) $(PROGRAM)

.c.o:
	$(CC) $(ARCH_COMPILE) $(CFLAGS) -c $*.c -o $*.o
