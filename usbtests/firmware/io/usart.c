/*
 * usart.c
 *
 *  Created on: 2010-04-09
 *      Author: kharg
 */

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include "usart.h"

void usartInit(uint32_t baud) {
	// set bitrate register
	UBRRH = (uint8_t)((F_CPU / (baud * 16L)) >> 8);
	UBRRL = (uint8_t)(F_CPU / (baud * 16L));

	// enable TX
	UCSRB = (1 << TXEN);

	// set 8N1
	UCSRC = (1 << URSEL) | (3 << UCSZ0);
}

int usartPutChar(char chr, FILE* stream) {
	// CR & LF
	if (chr == '\n')
		usartPutByte('\r');

	usartPutByte(chr);

	return 0;
}

void usartPutByte(char byte) {
	while (!(UCSRA & (1 << UDRE)));
	UDR = byte;
}
