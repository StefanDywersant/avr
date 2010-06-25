/**
 * AVR Libs
 * Biblioteki do Atmel AVR
 *
 * Copyright: (c) 2010 by Karol Maciaszek
 * E-mail   : <karol.maciaszek@gmail.com>
 * WWW      : http://www.czystybeton.pl/
 *
 * $Id$
 */

#include <stdio.h>
#include "usart.h"


static FILE usart_stdout = FDEV_SETUP_STREAM(usart_put_char, NULL, _FDEV_SETUP_WRITE);


void debug_init(void) {
	usart_init();

	stdout = &usart_stdout;
}
