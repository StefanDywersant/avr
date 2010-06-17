/**
 * Ambilight
 * Aktywne podświetlenie monitora.
 *
 * Copyright: (c) 2010 by Karol Maciaszek
 * E-mail   : <karol.maciaszek@gmail.com>
 * WWW      : http://www.czystybeton.pl/
 *
 * $Id$
 */


#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include "usart.h"

#include "usartconfig.h"

#define SETUP_SPEED()		{ \
								UBRRH = (uint8_t)((F_CPU / (USART_BAUD * 16L)) >> 8); \
								UBRRL = (uint8_t)(F_CPU / (USART_BAUD * 16L)); \
							}


void usart_init() {
	// set bitrate register
	SETUP_SPEED();

	// enable TX
	UCSRB = (1 << TXEN);

	// set 8N1
	UCSRC = (1 << URSEL) | (3 << UCSZ0);
}


int usart_put_char(char chr, FILE* stream) {
	// CR & LF
	if (chr == '\n')
		usart_put_byte('\r');

	usart_put_byte(chr);

	return 0;
}


void usart_put_byte(char byte) {
	while (!(UCSRA & (1 << UDRE)));
	UDR = byte;
}
