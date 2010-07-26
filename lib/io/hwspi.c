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
#include <hwspiconfig.h>
#include <debug.h>
#include <util/delay.h>


#define CONCAT(a, b)		a ## b
#define OUTPORT(name)		CONCAT(PORT, name)
#define DDRPORT(name)		CONCAT(DDR, name)
#define INPORT(name)		CONCAT(PIN, name)

#define CSN_H()				{ OUTPORT(HWSPI_PORT) |= 0x01 << HWSPI_CSN_PIN; }
#define CSN_L()				{ OUTPORT(HWSPI_PORT) &= ~(0x01 << HWSPI_CSN_PIN); }

#define DDR_SETUP_MASTER()	{ \
									DDRPORT(HWSPI_PORT) |= (0x01 << HWSPI_MOSI_PIN) | (0x01 << HWSPI_SCK_PIN) | (0x01 << HWSPI_CSN_PIN); \
									DDRPORT(HWSPI_PORT) &= ~(0x01 << HWSPI_MISO_PIN); \
							}

#define DDR_SETUP_SLAVE()	{ \
									DDRPORT(HWSPI_PORT) &= ~((0x01 << HWSPI_MOSI_PIN) | (0x01 << HWSPI_SCK_PIN) | (0x01 << HWSPI_CSN_PIN)); \
									DDRPORT(HWSPI_PORT) |= (0x01 << HWSPI_MISO_PIN); \
							}


static uint8_t (*slave_callback)(uint8_t);


void hwspi_init_slave(uint8_t (*func)(uint8_t)) {
	DDR_SETUP_SLAVE();

	SPCR = (1 << SPE) | (1 << SPIE);

	slave_callback = func;
}


void hwspi_init_master(void) {
	DDR_SETUP_MASTER();

	SPCR = (1 << SPE) | (1 << MSTR) | (HWSPI_SPR0 << SPR0) | (HWSPI_SPR1 << SPR1);
	SPSR |= (HWSPI_SPI2X << SPI2X);
}


void hwspi_begin() {
	// activate peripherial
	CSN_L();
}


void hwspi_end() {
	// deactivate peripherial
	CSN_H();
	_delay_us(10);
}


uint8_t hwspi_read_write_byte(uint8_t wr) {
	SPDR = wr;

	while (!(SPSR & (1 << SPIF)));

	return SPDR;
}


void hwspi_read_data(uint8_t len, uint8_t* data) {
	while (len--)
		*data++ = hwspi_read_write_byte(0x00);
}


void hwspi_write_data(uint8_t len, uint8_t* data) {
	while (len--)
		hwspi_read_write_byte(*data++);
}


ISR(HWSPI_STC_VECT) {
	SPDR = slave_callback(SPDR);
}
