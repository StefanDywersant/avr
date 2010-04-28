/**
 *
 *
 * @author Karol Maciaszek <karol.maciaszek@gmail.com>
 * $Id$
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "spi.h"

#define CONCAT(a, b)		a ## b
#define OUTPORT(name)		CONCAT(PORT, name)
#define DDRPORT(name)		CONCAT(DDR, name)
#define INPORT(name)		CONCAT(PIN, name)

#define MOSI_H()			{ OUTPORT(SPI_PORT) |= 0x01 << SPI_MOSI_PIN; }
#define MOSI_L()			{ OUTPORT(SPI_PORT) &= ~(0x01 << SPI_MOSI_PIN); }
#define SCK_H()				{ OUTPORT(SPI_PORT) |= 0x01 << SPI_SCK_PIN; }
#define SCK_L()				{ OUTPORT(SPI_PORT) &= ~(0x01 << SPI_SCK_PIN); }
#define CSN_H()				{ OUTPORT(SPI_PORT) |= 0x01 << SPI_CSN_PIN; }
#define CSN_L()				{ OUTPORT(SPI_PORT) &= ~(0x01 << SPI_CSN_PIN); }
#define MISO				(INPORT(SPI_PORT) & (0x01 << SPI_MISO_PIN))
#define DDR_SETUP()			{ \
									DDRPORT(SPI_PORT) |= (0x01 << SPI_MOSI_PIN) | (0x01 << SPI_SCK_PIN) | (0x01 << SPI_CSN_PIN); \
									DDRPORT(SPI_PORT) &= ~(0x01 << SPI_MISO_PIN); \
								}

#define WAIT()				{ _delay_us(SPI_FREQ / 1000); }


void spi_init() {
	// initialize SPI bus
	DDR_SETUP();

	// set CSN high
	CSN_H();
}


void spi_begin() {
	// disable interrupts
	cli();

	// activate peripherial
	CSN_L();
}


void spi_end() {
	// deactivate peripherial
	CSN_H();

	// enable interrupts
	sei();
}


uint8_t spi_read_write_byte(uint8_t wr) {
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


void spi_read_data(uint8_t len, uint8_t* data) {
	while (len--)
		*data++ = spi_read_write_byte(0x00);
}


void spi_write_data(uint8_t len, uint8_t* data) {
	while (len--)
		spi_read_write_byte(*data++);
}
