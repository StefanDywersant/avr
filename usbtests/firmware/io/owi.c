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


#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include "owi.h"

#define CONCAT(a, b)		a ## b
#define INPORT(name)		CONCAT(PIN, name)
#define DDRPORT(name)		CONCAT(DDR, name)

#define SET_L()				{ DDRPORT(OWI_PORT) |= 0x01 << (OWI_PIN); }
#define SET_H()				{ DDRPORT(OWI_PORT) &= ~(0x01 << (OWI_PIN)); }
#define READ_BIT				INPORT(OWI_PORT) & 0x01 << (OWI_PIN)

#define SEARCH_ROM			0xf0

#define GET_ITH_BIT(byte, idx)		(((byte) >> (idx)) & 0x01)
#define SET_ITH_BIT(byte, idx)		{ byte |= 0x01 << (idx); }
#define UNSET_ITH_BIT(byte, idx)	{ byte &= ~(0x01 << (idx)); }


static void write_bit(uint8_t bit) {
	if (bit) {
		SET_L();
		_delay_us(10);

		SET_H();
		_delay_us(65);
	} else {
		SET_L();
		_delay_us(100);

		SET_H();
		_delay_us(3);
	}
}


static uint8_t read_bit(void) {
	uint8_t bit = 0;

	SET_L();
	_delay_us(1);

	SET_H();
	_delay_us(8);

	bit = READ_BIT;
	_delay_us(60);

	return bit;
}


uint8_t owi_init(void) {
	uint8_t init_bit = 0;

	SET_L();
	_delay_us(500);

	SET_H();
	_delay_us(75);

	init_bit = READ_BIT;
	_delay_us(425);

	return init_bit;
}


void owi_write_byte(uint8_t byte) {
	uint8_t i = 0;

	for (i = 0; i < 8; i++)
		write_bit((byte >> i) & 0x01);
}

uint8_t owi_read_byte(void) {
	uint8_t byte = 0x00;
	uint8_t i;

	for (i = 0; i < 8; i++)
		if (read_bit())
			byte |= 1 << i;

	return byte;
}


uint8_t owi_get_next_rom(uint8_t* rom, uint8_t *last_diff) {
	uint8_t diff = 0;
	uint8_t i = 1;

	// no devices
	if (owi_init()) {
		*last_diff = 0;
		return 0x1;
	}

	// place search command on wire
	owi_write_byte(SEARCH_ROM);

	// read 64 bits of device ROM
	while (i < 65) {
		uint8_t bit0 = read_bit();
		uint8_t bit1 = read_bit();

		// error
		if (bit0 & bit1) {
			*last_diff = 0;
			return 0x2;
		}

		// conflict - some devices have 0 while other have 1 on current position
		if ((bit0 | bit1) == 0x0) {

			if (i == *last_diff) {
				SET_ITH_BIT(rom[(i - 1) / 8], (i - 1) % 8);
			} else if (i > *last_diff) {
				UNSET_ITH_BIT(rom[(i - 1) / 8], (i - 1) % 8);
				diff = i;
			} else if (GET_ITH_BIT(rom[(i - 1) / 8], (i - 1) % 8) == 0x0) {
				diff = i;
			}
		// no conflict - all devices have the same bit on current position
		} else {
			if (bit0)
				SET_ITH_BIT(rom[(i - 1) / 8], (i - 1) % 8)
			else
				UNSET_ITH_BIT(rom[(i - 1) / 8], (i - 1) % 8);
		}

		// write selected bit to wire
		write_bit(GET_ITH_BIT(rom[(i - 1) / 8], (i - 1) % 8));

		i++;
	}

	*last_diff = diff;

	// last device
	if (*last_diff == 0)
		return 0x3;

	return 0x0;
}
