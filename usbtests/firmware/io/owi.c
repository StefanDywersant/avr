#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include "owi.h"

#define OWI_CONCAT(a, b)		a ## b
#define OWI_INPORT(name)		OWI_CONCAT(PIN, name)
#define OWI_DDRPORT(name)		OWI_CONCAT(DDR, name)

#define OWI_SET_0()				{ OWI_DDRPORT(OWI_PORT) |= 0x01 << (OWI_PIN); }
#define OWI_SET_1()				{ OWI_DDRPORT(OWI_PORT) &= ~(0x01 << (OWI_PIN)); }
#define OWI_GET_BIT				OWI_INPORT(OWI_PORT) & 0x01 << (OWI_PIN)

#define OWI_SEARCH_ROM			0xf0

#define OWI_GET_ITH_BIT(byte, idx)		(((byte) >> (idx)) & 0x01)
#define OWI_SET_ITH_BIT(byte, idx)		{ byte |= 0x01 << (idx); }
#define OWI_UNSET_ITH_BIT(byte, idx)	{ byte &= ~(0x01 << (idx)); }

uint8_t OwiInit() {
	uint8_t initBit = 0;

	OWI_SET_0();
	_delay_us(500);

	OWI_SET_1();
	_delay_us(75);

	initBit = OWI_GET_BIT;
	_delay_us(425);

	return initBit;
}

void OwiWriteBit(uint8_t bit) {
	if (bit) {
		OWI_SET_0();
		_delay_us(10);

		OWI_SET_1();
		_delay_us(65);
	} else {
		OWI_SET_0();
		_delay_us(100);

		OWI_SET_1();
		_delay_us(3);
	}
}

uint8_t OwiReadBit() {
	uint8_t bit = 0;

	OWI_SET_0();
	_delay_us(1);

	OWI_SET_1();
	_delay_us(8);

	bit = OWI_GET_BIT;
	_delay_us(60);

	return bit;
}

void OwiWriteByte(uint8_t byte) {
	uint8_t i = 0;

	for (i = 0; i < 8; i++)
		OwiWriteBit((byte >> i) & 0x01);
}

uint8_t OwiReadByte() {
	uint8_t byte = 0x00;
	uint8_t i;

	for (i = 0; i < 8; i++)
		if (OwiReadBit())
			byte |= 1 << i;

	return byte;
}

uint8_t OwiGetNextROM(uint8_t* prevROM, uint8_t* nextROM, uint8_t *lastDiff) {
	uint8_t diff = 0;
	uint8_t i = 1;

	// no devices
	if (OwiInit()) {
		*lastDiff = 0;
		return 0x1;
	}

	// place search command on wire
	OwiWriteByte(OWI_SEARCH_ROM);

	// read 64 bits of device ROM
	while (i < 65) {
		uint8_t bit0 = OwiReadBit();
		uint8_t bit1 = OwiReadBit();

		// error
		if (bit0 & bit1) {
			*lastDiff = 0;
			return 0x2;
		}

		// conflict - some devices have 0 while other have 1 on current position
		if ((bit0 | bit1) == 0x0) {

			if (i == *lastDiff) {
				OWI_SET_ITH_BIT(nextROM[(i - 1) / 8], (i - 1) % 8);
			} else if (i > *lastDiff) {
				OWI_UNSET_ITH_BIT(nextROM[(i - 1) / 8], (i - 1) % 8);
				diff = i;
			} else if (OWI_GET_ITH_BIT(prevROM[(i - 1) / 8], (i - 1) % 8) == 0x0) {
				diff = i;
			}
		// no conflict - all devices have the same bit on current position
		} else {
			if (bit0)
				OWI_SET_ITH_BIT(nextROM[(i - 1) / 8], (i - 1) % 8)
			else
				OWI_UNSET_ITH_BIT(nextROM[(i - 1) / 8], (i - 1) % 8);
		}

		// write selected bit to wire
		OwiWriteBit(OWI_GET_ITH_BIT(nextROM[(i - 1) / 8], (i - 1) % 8));

		i++;
	}

	*lastDiff = diff;

	// last device
	if (*lastDiff == 0)
		return 0x3;

	return 0x0;
}
