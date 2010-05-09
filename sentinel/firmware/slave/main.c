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


/*PROGMEM char usbHidReportDescriptor[52] = {
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
};*/

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

static uint8_t key_pos = 0;

#define s_1(x)		(x << 1) + ((x & 0x8000) >> 15)
#define s_2(x)		(x << 2) + ((x & 0xc000) >> 14)
#define s_3(x)		(x << 3) + ((x & 0xe000) >> 13)
#define s_5(x)		(x << 5) + ((x & 0xf800) >> 11)

void mixup(uint16_t* payload, uint16_t* key_buf) {
	uint8_t i;

	for (i = 0; i < 4; i++) {
		payload[i] = (uint16_t)(
				payload[i]
				+ key_buf[key_pos]
				+ (payload[(uint8_t)(i - 1) % 4] & payload[(uint8_t)(i - 2) % 4])
				+ ((~payload[(uint8_t)(i - 1) % 4]) & payload[(uint8_t)(i - 3) % 4]));

		key_pos = (key_pos + 1) % 64;

		if (i == 0)
			payload[i] = s_1(payload[i]);
		else if (i == 1)
			payload[i] = s_2(payload[i]);
		else if (i == 2)
			payload[i] = s_3(payload[i]);
		else if (i == 3)
			payload[i] = s_5(payload[i]);
	}

	PRINTF("mixup: 0x");
	for (i = 3; i != 255; i--) {
		PRINTF("%04x", payload[i]);
	}
	PRINTF("\n");
}

#define rs_1(x)		(x >> 1) + ((x & 0x0001) << 15)
#define rs_2(x)		(x >> 2) + ((x & 0x0003) << 14)
#define rs_3(x)		(x >> 3) + ((x & 0x0007) << 13)
#define rs_5(x)		(x >> 5) + ((x & 0x001f) << 11)

void rmixup(uint16_t* payload, uint16_t* key_buf) {
	uint8_t i;

	for (i = 3; i != 255; i--) {
		if (i == 0)
			payload[i] = rs_1(payload[i]);
		else if (i == 1)
			payload[i] = rs_2(payload[i]);
		else if (i == 2)
			payload[i] = rs_3(payload[i]);
		else if (i == 3)
			payload[i] = rs_5(payload[i]);

		payload[i] = (uint16_t)(
				payload[i]
				- key_buf[key_pos]
				- (payload[(uint8_t)(i - 1) % 4] & payload[(uint8_t)(i - 2) % 4])
				- ((~payload[(uint8_t)(i - 1) % 4]) & payload[(uint8_t)(i - 3) % 4]));

		key_pos = (uint8_t)(key_pos - 1) % 64;
	}

	PRINTF("rmixup: 0x");
	for (i = 3; i != 255; i--) {
		PRINTF("%04x", payload[i]);
	}
	PRINTF("\n");
}

void rmashup(uint16_t* payload, uint16_t* key_buf) {
	uint8_t i;

	for (i = 3; i != 255; i--)
		payload[i] = payload[i] - key_buf[payload[(uint8_t)(i - 1) % 4] & 63];

	PRINTF("rmashup: 0x");
	for (i = 3; i != 255; i--) {
		PRINTF("%04x", payload[i]);
	}
	PRINTF("\n");
}

void mashup(uint16_t* payload, uint16_t* key_buf) {
	uint8_t i;

	for (i = 0; i < 4; i++)
		payload[i] = payload[i] + key_buf[payload[(uint8_t)(i - 1) % 4] & 63];

	PRINTF("mashup: 0x");
	for (i = 3; i != 255; i--) {
		PRINTF("%04x", payload[i]);
	}
	PRINTF("\n");
}

void encrypt(void) {
	uint8_t key[8];
	uint8_t key_buf[128];
	uint8_t payload[8];
	uint8_t i;

	static uint8_t pitable[256] PROGMEM = {
		0xd9, 0x78, 0xf9, 0xc4, 0x19, 0xdd, 0xb5, 0xed, 0x28, 0xe9, 0xfd, 0x79, 0x4a, 0xa0, 0xd8, 0x9d,
		0xc6, 0x7e, 0x37, 0x83, 0x2b, 0x76, 0x53, 0x8e, 0x62, 0x4c, 0x64, 0x88, 0x44, 0x8b, 0xfb, 0xa2,
		0x17, 0x9a, 0x59, 0xf5, 0x87, 0xb3, 0x4f, 0x13, 0x61, 0x45, 0x6d, 0x8d, 0x09, 0x81, 0x7d, 0x32,
		0xbd, 0x8f, 0x40, 0xeb, 0x86, 0xb7, 0x7b, 0x0b, 0xf0, 0x95, 0x21, 0x22, 0x5c, 0x6b, 0x4e, 0x82,
		0x54, 0xd6, 0x65, 0x93, 0xce, 0x60, 0xb2, 0x1c, 0x73, 0x56, 0xc0, 0x14, 0xa7, 0x8c, 0xf1, 0xdc,
		0x12, 0x75, 0xca, 0x1f, 0x3b, 0xbe, 0xe4, 0xd1, 0x42, 0x3d, 0xd4, 0x30, 0xa3, 0x3c, 0xb6, 0x26,
		0x6f, 0xbf, 0x0e, 0xda, 0x46, 0x69, 0x07, 0x57, 0x27, 0xf2, 0x1d, 0x9b, 0xbc, 0x94, 0x43, 0x03,
		0xf8, 0x11, 0xc7, 0xf6, 0x90, 0xef, 0x3e, 0xe7, 0x06, 0xc3, 0xd5, 0x2f, 0xc8, 0x66, 0x1e, 0xd7,
		0x08, 0xe8, 0xea, 0xde, 0x80, 0x52, 0xee, 0xf7, 0x84, 0xaa, 0x72, 0xac, 0x35, 0x4d, 0x6a, 0x2a,
		0x96, 0x1a, 0xd2, 0x71, 0x5a, 0x15, 0x49, 0x74, 0x4b, 0x9f, 0xd0, 0x5e, 0x04, 0x18, 0xa4, 0xec,
		0xc2, 0xe0, 0x41, 0x6e, 0x0f, 0x51, 0xcb, 0xcc, 0x24, 0x91, 0xaf, 0x50, 0xa1, 0xf4, 0x70, 0x39,
		0x99, 0x7c, 0x3a, 0x85, 0x23, 0xb8, 0xb4, 0x7a, 0xfc, 0x02, 0x36, 0x5b, 0x25, 0x55, 0x97, 0x31,
		0x2d, 0x5d, 0xfa, 0x98, 0xe3, 0x8a, 0x92, 0xae, 0x05, 0xdf, 0x29, 0x10, 0x67, 0x6c, 0xba, 0xc9,
		0xd3, 0x00, 0xe6, 0xcf, 0xe1, 0x9e, 0xa8, 0x2c, 0x63, 0x16, 0x01, 0x3f, 0x58, 0xe2, 0x89, 0xa9,
		0x0d, 0x38, 0x34, 0x1b, 0xab, 0x33, 0xff, 0xb0, 0xbb, 0x48, 0x0c, 0x5f, 0xb9, 0xb1, 0xcd, 0x2e,
		0xc5, 0xf3, 0xdb, 0x47, 0xe5, 0xa5, 0x9c, 0x77, 0x0a, 0xa6, 0x20, 0x68, 0xfe, 0x7f, 0xc1, 0xad
	};

	// initialize key and data
	for (i = 0; i < 8; i++)
		payload[i] = 0xff;

	for (i = 0; i < 8; i++)
		key[i] = 0xff;

	// key expansion algorithm
	uint8_t t = 8;
	uint8_t t8 = 8;
	uint8_t tm = 0xff;

	PRINTF("key_buf: empty");
	for (i = 0; i < 128; i++) {
		if (i % 32 == 0)
			PRINTF("\n  0x");
		PRINTF("%02x", key_buf[i]);
	}
	PRINTF("\n\n");

	for (i = 0; i < 8; i++)
		key_buf[i] = key[i];

	PRINTF("key_buf: insert key");
	for (i = 0; i < 128; i++) {
		if (i % 32 == 0)
			PRINTF("\n  0x");
		PRINTF("%02x", key_buf[i]);
	}
	PRINTF("\n\n");

	for (i = t; i < 128; i++)
		key_buf[i] = pgm_read_byte(&pitable[key_buf[i - 1] + key_buf[i - t]]);

	PRINTF("key_buf: step 1");
	for (i = 0; i < 128; i++) {
		if (i % 32 == 0)
			PRINTF("\n  0x");
		PRINTF("%02x", key_buf[i]);
	}
	PRINTF("\n\n");

	key_buf[128 - t8] = pgm_read_byte(&pitable[key_buf[128 - t8]]);

	PRINTF("key_buf: step 2");
	for (i = 0; i < 128; i++) {
		if (i % 32 == 0)
			PRINTF("\n  0x");
		PRINTF("%02x", key_buf[i]);
	}
	PRINTF("\n\n");

	for (i = 127 - t8; i != 255; i--)
		key_buf[i] = pgm_read_byte(&pitable[key_buf[i + 1] ^ key_buf[i + t8]]);

	PRINTF("key_buf: step3");
	for (i = 0; i < 128; i++) {
		if (i % 32 == 0)
			PRINTF("\n  0x");
		PRINTF("%02x", key_buf[i]);
	}
	PRINTF("\n\n");

	// encrypt

	key_pos = 0;

	// 5 x mixup
	for (i = 0; i < 5; i++)
		mixup((uint16_t*)payload, (uint16_t*)key_buf);

	// 1 x mashup
	mashup((uint16_t*)payload, (uint16_t*)key_buf);

	// 6 x mixup
	for (i = 0; i < 6; i++)
		mixup((uint16_t*)payload, (uint16_t*)key_buf);

	// 1 x mashup
	mashup((uint16_t*)payload, (uint16_t*)key_buf);

	// 5 x mixup
	for (i = 0; i < 5; i++)
		mixup((uint16_t*)payload, (uint16_t*)key_buf);

	PRINTF("\ncipher: 0x");
	for (i = 7; i != 255; i--) {
		PRINTF("%02x", payload[i]);
	}
	PRINTF("\n\n");

	// decrypt

	key_pos = 63;

	// 5 x mixup
	for (i = 0; i < 5; i++)
		rmixup((uint16_t*)payload, (uint16_t*)key_buf);

	// 1 x mashup
	rmashup((uint16_t*)payload, (uint16_t*)key_buf);

	// 6 x mixup
	for (i = 0; i < 6; i++)
		rmixup((uint16_t*)payload, (uint16_t*)key_buf);

	// 1 x mashup
	rmashup((uint16_t*)payload, (uint16_t*)key_buf);

	// 5 x mixup
	for (i = 0; i < 5; i++)
		rmixup((uint16_t*)payload, (uint16_t*)key_buf);

	PRINTF("decrypted: 0x");
	for (i = 7; i != 255; i--) {
		PRINTF("%02x", payload[i]);
	}
	PRINTF("\n");
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
		case 0x08:
			encrypt();
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
	PRINTF("nrf905_print_tx\n");
}

void nrf905_print_rx(void) {
	uint8_t buf[32];
	uint8_t i;

	PRINTF("nrf905_print_rx\n");

	nrf905_rx_packet(32, buf);

	PRINTF(" RXPAYLOAD=0x");
	for (i = 0; i < 32; i++)
		PRINTF("%02x", buf[i]);
	PRINTF("\n");

}

int main(void) {
	debug_init();

	PRINTF("\nSentinel Slave Board\n");

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
