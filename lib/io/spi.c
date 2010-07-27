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

#include <avr/interrupt.h>
#include <spiconfig.h>
#include <debug.h>
#include <util/delay.h>


#define CONCAT(a, b)		a ## b
#define OUTPORT(name)		CONCAT(PORT, name)
#define DDRPORT(name)		CONCAT(DDR, name)
#define INPORT(name)		CONCAT(PIN, name)

#define CSN_H()				{ OUTPORT(SPI_PORT) |= 0x01 << SPI_CSN_PIN; }
#define CSN_L()				{ OUTPORT(SPI_PORT) &= ~(0x01 << SPI_CSN_PIN); }

#define DDR_SETUP_MASTER()	{ \
									DDRPORT(SPI_PORT) |= (0x01 << SPI_MOSI_PIN) | (0x01 << SPI_SCK_PIN) | (0x01 << SPI_CSN_PIN); \
									DDRPORT(SPI_PORT) &= ~(0x01 << SPI_MISO_PIN); \
							}

#define DDR_SETUP_SLAVE()	{ \
									DDRPORT(SPI_PORT) &= ~((0x01 << SPI_MOSI_PIN) | (0x01 << SPI_SCK_PIN) | (0x01 << SPI_CSN_PIN)); \
									DDRPORT(SPI_PORT) |= (0x01 << SPI_MISO_PIN); \
							}


static uint8_t (*slave_callback)(uint8_t);


void spi_init_slave(uint8_t (*func)(uint8_t)) {
	DDR_SETUP_SLAVE();

	SPCR = (1 << SPE) | (1 << SPIE);

	slave_callback = func;
}


void spi_init_master(void) {
	DDR_SETUP_MASTER();

	SPCR = (1 << SPE) | (1 << MSTR) | (SPI_SPR0 << SPR0) | (SPI_SPR1 << SPR1);
	SPSR |= (SPI_SPI2X << SPI2X);
}


void spi_begin() {
	// activate peripherial
	CSN_L();
}


void spi_end() {
	// deactivate peripherial
	CSN_H();
	_delay_us(10);
}


uint8_t spi_read_write_byte(uint8_t wr) {
	SPDR = wr;

	while (!(SPSR & (1 << SPIF)));

	return SPDR;
}


void spi_read_data(uint8_t len, uint8_t* data) {
	while (len--)
		*data++ = spi_read_write_byte(0x00);
}


void spi_write_data(uint8_t len, uint8_t* data) {
	while (len--)
		spi_read_write_byte(*data++);
}


ISR(SPI_STC_VECT) {
	SPDR = slave_callback(SPDR);
}
