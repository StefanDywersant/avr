#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "owi.h"
#include "spi.h"
#include "twi.h"
#include "usart.h"
#include "nrf905.h"
#include "pcf8583.h"
#include "usbdrv.h"

PROGMEM char usbHidReportDescriptor[52] = { /* USB report descriptor, size must match usbconfig.h */
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x02,                    // USAGE (Mouse)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x09, 0x01,                    //   USAGE (Pointer)
    0xA1, 0x00,                    //   COLLECTION (Physical)
    0x05, 0x09,                    //     USAGE_PAGE (Button)
    0x19, 0x01,                    //     USAGE_MINIMUM
    0x29, 0x03,                    //     USAGE_MAXIMUM
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x95, 0x03,                    //     REPORT_COUNT (3)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x75, 0x05,                    //     REPORT_SIZE (5)
    0x81, 0x03,                    //     INPUT (Const,Var,Abs)
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
    0x09, 0x38,                    //     USAGE (Wheel)
    0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
    0x25, 0x7F,                    //     LOGICAL_MAXIMUM (127)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x03,                    //     REPORT_COUNT (3)
    0x81, 0x06,                    //     INPUT (Data,Var,Rel)
    0xC0,                          //   END_COLLECTION
    0xC0,                          // END COLLECTION
};

static char led = 0xff;
static pcf8583DateTimeStruct time;
//static uint8_t buf[9];

FILE usartStdout = FDEV_SETUP_STREAM(usartPutChar, NULL, _FDEV_SETUP_WRITE);

void twiPlayground() {
/*	uint8_t request[1];
	uint8_t response[1];

	*request = 0x02;

	if (twiSyncMTMR(0xa0, request, 1, response, 1) == TWI_OK)
		led = response[0];
	else
		led = 0x55;*/
}

void readSecond() {
	if (pcf8583GetDateTime(0xa0, &time) != PCF8583_OK) {
		led = 0xaa;
		return;
	}
}

void writeMinute() {
	pcf8583DateTimeStruct* _time = pcf8583NewDateTimeStruct();
	_time->milisecond = 0x00;
	_time->second = 0x00;
	_time->minute = 0x00;
	_time->hour = 0x00;
	_time->date = 0x01;
	_time->month = 0x0b;
	_time->year = 0x02;
	_time->weekday = 0x01;

	if (pcf8583SetDateTime(0xa0, _time) != PCF8583_OK) {
		led = 0x55;
		return;
	}

	free(_time);
}

void owiPlayground() {
/*	uint8_t x = OwiInit();
	led = x;

	OwiWriteByte(0xcc);

	OwiWriteByte(0xbe);

	uint8_t i = 0;
	for (i = 0; i < 9; i++)
		buf[i] = OwiReadByte();

	OwiInit();

	OwiWriteByte(0xcc);

	OwiWriteByte(0x44);

	led = buf[0];*/
}

void owiSearchROMs() {
/*	uint8_t* rom = (uint8_t*)malloc(8 * sizeof(uint8_t));
	uint8_t lastDiff = 0;
	uint8_t lastROM[8];
	uint8_t i;

	for (i = 0; i < 8; i++)
		lastROM[i] = 0;

	led = OwiGetNextROM(lastROM, rom, &lastDiff);

	led = ((uint8_t*)rom)[0];*/
}

/*
//uint8_t spiReadWriteByte(uint8_t byte) {
//	SPDR = byte;

//	printf("SPSR: %02x\n", SPSR);

	while (!(SPSR & (1 << SPIF)));

	return SPDR;
}

uint8_t spiReadByte() {
	while (!(SPSR & (1 < SPIF)));
	return SPDR;
}
*/

void nrf905Playground() {
	printf("SPI start\n");

/*	spiBegin();
	spiReadWriteByte(0x10);

	uint8_t data[10];
	spiReadData(10, data);*/

//	nrf905_read_control_register();

	printf("CH_NO=%d\n", nrf905_get_channel_no());

	printf("ADDR=%04x\n", nrf905_get_address());

//	uint8_t i;
//	for (i = 0; i < 10; i++)
//		printf("%d: %02x\n", i, cr[i]);

//	spiEnd();
}

void executeCommand(uchar command) {
	printf("Executing command: 0x%02x\n", command);
	switch (command) {
		case 0x00:
			twiPlayground();
			break;
		case 0x01:
			readSecond();
			break;
		case 0x02:
			writeMinute();
			break;
		case 0x03:
			owiPlayground();
			break;
		case 0x04:
			owiSearchROMs();
			break;
		case 0x05:
			nrf905Playground();
			break;
		default:
			usartPutByte(command);
			led = command;
	}
}

usbMsgLen_t usbFunctionSetup(uchar data[8]) {
	usbRequest_t* request = (usbRequest_t *)data;

	if (request->bRequest == 0x04) {
//		led = 0x00;
		return USB_NO_MSG;
	}

    return 0;
}

uchar usbFunctionRead(uchar *data, uchar len) {
/*	data[0] = time.milisecond;
	data[1] = time.second;
	data[2] = time.minute;
	data[3] = time.hour;
	data[4] = time.date;
	data[5] = time.month;
	data[6] = time.year;
	data[7] = time.weekday;

	return 8;*/

//	for (i = 0; i < 2; i++)
//		data[i] = buf[i];

//	return 2;

	uint8_t* rom = (uint8_t*)malloc(8 * sizeof(uint8_t));
	uint8_t lastDiff = 0;
	uint8_t lastROM[8];
	uint8_t i;

	for (i = 0; i < 8; i++)
		lastROM[i] = 0;

	printf_P("Searching for ROMs... ");

	cli();
	OwiGetNextROM(rom, rom, &lastDiff);

	for (i = 0; i < 8; i++) {
		data[i] = rom[i];
		printf("%02x", rom[i]);
	}
	printf("\n");

	OwiGetNextROM(rom, rom, &lastDiff);

	for (i = 0; i < 8; i++) {
		data[i] = rom[i];
		printf("%02x", rom[i]);
	}
	printf("\n");

	sei();

	free(rom);

	return 8;
}

uchar usbFunctionWrite(uchar *data, uchar len) {
	unsigned int i;

	for (i = 0; i < len; i++)
		executeCommand(data[i]);

	return len;
}

int main(void) {
	usartInit(9600);
	stdout = &usartStdout;

	printf("\n\n\nNapierdalator test board\n");

	DDRB = 0xFF;

	printf("Initializing TWI... ");
	twiInit(100000);
	printf("done\n");

	printf("Initializing SPI... ");
	spiInit();
	printf("done\n");

	printf("Initializing USB... ");
	usbInit();
	usbDeviceDisconnect();
	_delay_ms(250);
	usbDeviceConnect();
	printf("done\n");

	printf("Initializing interrupts... ");
	sei();
	printf("done\n");

	printf("Entering main loop...\n\n");

	while (1) {
		usbPoll();
//		PORTB = led;
	}
}
