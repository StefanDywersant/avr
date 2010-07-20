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


#define CONCAT(a, b)		a ## b
#define OUTPORT(name)		CONCAT(PORT, name)
#define DDRPORT(name)		CONCAT(DDR, name)
#define INPORT(name)		CONCAT(PIN, name)

#define DDR_SETUP()			{ \
									DDRPORT(HWSPI_PORT) &= ~((0x01 << HWSPI_MOSI_PIN) | (0x01 << HWSPI_SCK_PIN) | (0x01 << HWSPI_CSN_PIN)); \
									DDRPORT(HWSPI_PORT) |= (0x01 << HWSPI_MISO_PIN); \
							}


static uint8_t (*slave_callback)(uint8_t) = NULL;


void hwspi_init_slave(uint8_t (*func)(uint8_t)) {
	DDR_SETUP();

	SPCR = (1 << SPE);

	slave_callback = func;
}


ISR(HWSPI_STC_VECT) {
	SPDR = slave_callback(SPDR);
}
