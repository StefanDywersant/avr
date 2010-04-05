#include <avr/io.h>
#include <util/delay.h>
#include "owi.h"

#define OWI_CONCAT(a, b)		a ## b
#define OWI_INPORT(name)		OWI_CONCAT(PIN, name)
#define OWI_DDRPORT(name)		OWI_CONCAT(DDR, name)

#define OWI_SET_0()		{ OWI_DDRPORT(OWI_PORT) |= OWI_PIN; }
#define OWI_SET_1()		{ OWI_DDRPORT(OWI_PORT) &= !OWI_PIN; }
#define OWI_GET_BIT		OWI_INPORT(OWI_PORT) & OWI_PIN

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
