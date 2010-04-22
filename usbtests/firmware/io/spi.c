/*
 * spi.c
 *
 *  Created on: 2010-04-21
 *      Author: kharg
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "spi.h"

#define SPI_CONCAT(a, b)		a ## b
#define SPI_OUTPORT(name)		SPI_CONCAT(PORT, name)
#define SPI_DDRPORT(name)		SPI_CONCAT(DDR, name)
#define SPI_INPORT(name)		SPI_CONCAT(PIN, name)

#define SPI_MOSI_H()			{ SPI_OUTPORT(SPI_PORT) |= 0x01 << SPI_MOSI_PIN; }
#define SPI_MOSI_L()			{ SPI_OUTPORT(SPI_PORT) &= ~(0x01 << SPI_MOSI_PIN); }
#define SPI_SCK_H()				{ SPI_OUTPORT(SPI_PORT) |= 0x01 << SPI_SCK_PIN; }
#define SPI_SCK_L()				{ SPI_OUTPORT(SPI_PORT) &= ~(0x01 << SPI_SCK_PIN); }
#define SPI_CSN_H()				{ SPI_OUTPORT(SPI_PORT) |= 0x01 << SPI_CSN_PIN; }
#define SPI_CSN_L()				{ SPI_OUTPORT(SPI_PORT) &= ~(0x01 << SPI_CSN_PIN); }
#define SPI_MISO				(SPI_INPORT(SPI_PORT) & (0x01 << SPI_MISO_PIN))
#define SPI_DDR_SET()			{ \
									SPI_DDRPORT(SPI_PORT) |= (0x01 << SPI_MOSI_PIN) | (0x01 << SPI_SCK_PIN) | (0x01 << SPI_CSN_PIN); \
									SPI_DDRPORT(SPI_PORT) &= ~(0x01 << SPI_MISO_PIN); \
								}

void spiInit() {
	// initialize SPI bus
	SPI_DDR_SET();

	// set CSN high
	SPI_CSN_H();
}

void spiBegin() {
	// disable interrupts
	cli();

	// activate peripherial
	SPI_CSN_L();
}

void spiEnd() {
	// deactivate peripherial
	SPI_CSN_H();

	// enable interrupts
	sei();
}

uint8_t spiReadWriteByte(uint8_t wr) {
	uint8_t i;
	uint8_t re = 0x00;

	for (i = 0; i < 8; i++) {
		if ((wr << i) & 0x80) {
			SPI_MOSI_H();
		} else {
			SPI_MOSI_L();
		}

		SPI_SCK_H();
		_delay_us(10);

		re |= SPI_MISO << (7 - i);

		SPI_SCK_L();
		_delay_us(10);
	}

	return re;
}

void spiReadData(uint8_t len, uint8_t* data) {
	uint8_t i;

	for (i = 0; i < len; i++)
		data[i] = spiReadWriteByte(0x00);
}

void spiWriteData(uint8_t len, uint8_t* data) {
	uint8_t i;

	for (i = 0; i < len; i++)
		spiReadWriteByte(data[i]);
}

