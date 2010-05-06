/**
 * Sentinel
 * Multi-aquarium controller.
 *
 * Copyright: (c) 2010 by Karol Maciaszek
 * E-mail   : <karol.maciaszek@gmail.com>
 * WWW      : http://www.czystybeton.pl/
 *
 * $Id$
 */

#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/io.h>
#include <util/delay.h>

// interfaces
#include "owi.h"
#include "spi.h"
#include "twi.h"
#include "usbdrv.h"

// devices
#include "nrf905.h"
#include "pcf8583.h"

// debug
#include "debug.h"


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
static pcf8583_datetime_t time;

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
	if (pcf8583_get_datetime(0xa0, &time) != PCF8583_OK) {
		led = 0xaa;
		return;
	}

	PRINTF("Second: 0x%02x\n", time.second);
}

void writeMinute() {
	pcf8583_datetime_t* _time = pcf8583_new_datetime();
	_time->milisecond = 0x00;
	_time->second = 0x00;
	_time->minute = 0x00;
	_time->hour = 0x00;
	_time->date = 0x01;
	_time->month = 0x0b;
	_time->year = 0x02;
	_time->weekday = 0x01;

	if (pcf8583_set_datetime(0xa0, _time) != PCF8583_OK) {
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

//	PRINTF("SPSR: %02x\n", SPSR);

	while (!(SPSR & (1 << SPIF)));

	return SPDR;
}

uint8_t spiReadByte() {
	while (!(SPSR & (1 < SPIF)));
	return SPDR;
}
*/

void nrf905Playground() {
	uint8_t buf[32];
	uint8_t i;


	PRINTF("NRF905 Playground\n");

	PRINTF(" SR=%02x\n", nrf905_get_status_register());

	PRINTF(" CH_NO=0x%03x\n", nrf905_get_channel_no());

	PRINTF(" RXADDR=0x%04lx\n", nrf905_get_rx_address());

	PRINTF(" TXADDR=0x%04lx\n", nrf905_get_tx_address());

	nrf905_get_tx_payload(buf);
	PRINTF(" TXPAYLOAD=0x");
	for (i = 0; i < 32; i++)
		PRINTF("%02x", buf[i]);
	PRINTF("\n");

	nrf905_rx_packet(32, buf);
	PRINTF(" RXPAYLOAD=0x");
	for (i = 0; i < 32; i++)
		PRINTF("%02x", buf[i]);
	PRINTF("\n");

	PRINTF("\n");
}

void nrf905ReadControlRegister() {
	uint8_t i;
	uint8_t buf[10];

	PRINTF("NRF905 Control Register:\n");

	nrf905_read_control_register(buf);

	for (i = 0; i < 10; i++)
		PRINTF("cr[0x%02x]=0x%02x\n", i, buf[i]);

	PRINTF("\n");

}

void nrf905TxPacket() {
	uint8_t payload[32];

	uint8_t i;
	for (i = 0; i < 32; i++)
		payload[i] = i;

	nrf905_tx_packet(0x11223344, 32, payload);
}

void executeCommand(uchar command) {
	PRINTF("Executing command: 0x%02x\n", command);
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
		case 0x06:
			nrf905ReadControlRegister();
			break;
		case 0x07:
			nrf905TxPacket();
			break;
		default:
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
	uint8_t last_diff = 0;
	uint8_t i;

	PRINTF("Searching for ROMs... ");

	cli();
	owi_get_next_rom(rom, &last_diff);

	for (i = 0; i < 8; i++) {
		data[i] = rom[i];
		PRINTF("%02x", rom[i]);
	}
	PRINTF("\n");

	owi_get_next_rom(rom, &last_diff);

	for (i = 0; i < 8; i++) {
		data[i] = rom[i];
		PRINTF("%02x", rom[i]);
	}
	PRINTF("\n");

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

void nrf905_print_tx(void) {
	PORTA &= ~(0x01 << 5);
	_delay_ms(250);
	PRINTF("nrf905_print_tx\n");
}

void nrf905_print_rx(void) {
	PORTA |= 0x01 << 5;
	_delay_ms(250);
	PRINTF("nrf905_print_rx\n");
}

int main(void) {
	debug_init();

	PRINTF("\n\n\nNapierdalator test board\n");

	DDRB = 0xFF;
	DDRA |= 0x01 << 5;

	PRINTF("Initializing TWI... ");
	twi_init();
	PRINTF("done\n");

	PRINTF("Initializing SPI... ");
	spi_init();
	PRINTF("done\n");

	PRINTF("Initializing USB... ");
	usbInit();
	usbDeviceDisconnect();
	_delay_ms(250);
	usbDeviceConnect();
	PRINTF("done\n");

	PRINTF("Initializing interrupts... ");
	sei();
	PRINTF("done\n");

	PRINTF("Initializing nrf905... ");
	nrf905_init();
	nrf905_set_packet_rx_callback(nrf905_print_rx);
	nrf905_set_packet_tx_callback(nrf905_print_tx);
	PRINTF("done\n");

	PRINTF("Entering main loop...\n\n");

	while (1) {
		usbPoll();
//		PORTB = led;
	}
}
