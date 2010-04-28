/**
 * debug.c
 * 
 * @author Karol Maciaszek <karol.maciaszek@gmail.com>
 */

#include <stdio.h>
#include "usart.h"


static FILE usart_stdout = FDEV_SETUP_STREAM(usart_put_char, NULL, _FDEV_SETUP_WRITE);


void debug_init(void) {
	usart_init();

	stdout = &usart_stdout;
}
