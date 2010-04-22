/*
 * spi.c
 *
 *  Created on: 2010-04-21
 *      Author: kharg
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "spi.h"
#include <util/delay.h>

void spiInit() {
	cli();
	DDRA = 0xfe;
//	DDRA &= ~(0x01 << SPI_MISO_PIN);
	PORTA = 0x08;
	_delay_us(10);
	PORTA = 0x00;
}

void spiOff() {
	PORTA |= 0x01 << SPI_CSN_PIN;
	sei();
}

uint8_t spiReadWriteByte(uint8_t byte) {
	uint8_t i;
	uint8_t ret = 0x00;

	for (i = 0; i < 8; i++) {
		if ((byte << i) & 0x80)
			PORTA |= 0x04;
		else
			PORTA &= 0xfb;

		PORTA |= 0x02;
		_delay_us(10);

		ret |= (PINA & 0x01) << (7 - i);

		PORTA &= 0xfd;
		_delay_us(10);
	}

	return ret;
}
