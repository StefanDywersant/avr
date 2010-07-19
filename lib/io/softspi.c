/**
 * AVRLibs
 * Atmel AVR libraries
 *
 * Copyright: (c) 2010 by Karol Maciaszek
 * E-mail   : <karol.maciaszek@gmail.com>
 * WWW      : http://www.czystybeton.pl/
 *
 * $Id$
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "softspi.h"

#define CONCAT(a, b)		a ## b
#define OUTPORT(name)		CONCAT(PORT, name)
#define DDRPORT(name)		CONCAT(DDR, name)
#define INPORT(name)		CONCAT(PIN, name)

#define MOSI_H()			{ OUTPORT(SOFTSPI_PORT) |= 0x01 << SOFTSPI_MOSI_PIN; }
#define MOSI_L()			{ OUTPORT(SOFTSPI_PORT) &= ~(0x01 << SOFTSPI_MOSI_PIN); }
#define SCK_H()				{ OUTPORT(SOFTSPI_PORT) |= 0x01 << SOFTSPI_SCK_PIN; }
#define SCK_L()				{ OUTPORT(SOFTSPI_PORT) &= ~(0x01 << SOFTSPI_SCK_PIN); }
#define CSN_H()				{ OUTPORT(SOFTSPI_PORT) |= 0x01 << SOFTSPI_CSN_PIN; }
#define CSN_L()				{ OUTPORT(SOFTSPI_PORT) &= ~(0x01 << SOFTSPI_CSN_PIN); }
#define MISO				(INPORT(SOFTSPI_PORT) & (0x01 << SOFTSPI_MISO_PIN))
#define DDR_SETUP()			{ \
									DDRPORT(SOFTSPI_PORT) |= (0x01 << SOFTSPI_MOSI_PIN) | (0x01 << SOFTSPI_SCK_PIN) | (0x01 << SOFTSPI_CSN_PIN); \
									DDRPORT(SOFTSPI_PORT) &= ~(0x01 << SOFTSPI_MISO_PIN); \
								}

#define WAIT()				{ _delay_us(SPI_FREQ / 1000); }


void softspi_init() {
	// initialize SPI bus
	DDR_SETUP();

	// set CSN high
	CSN_H();
}


void softspi_begin() {
	// disable interrupts
	cli();

	// activate peripherial
	CSN_L();
}


void softspi_end() {
	// deactivate peripherial
	CSN_H();

	// enable interrupts
	sei();
}


uint8_t softspi_read_write_byte(uint8_t wr) {
	uint8_t i;
	uint8_t re = 0x00;

	for (i = 0; i < 8; i++) {
		if ((wr << i) & 0x80) {
			MOSI_H();
		} else {
			MOSI_L();
		}

		SCK_H();
		WAIT();

		re |= MISO << (7 - i);

		SCK_L();
		WAIT();
	}

	return re;
}


void softspi_read_data(uint8_t len, uint8_t* data) {
	while (len--)
		*data++ = softspi_read_write_byte(0x00);
}


void softspi_write_data(uint8_t len, uint8_t* data) {
	while (len--)
		softspi_read_write_byte(*data++);
}
