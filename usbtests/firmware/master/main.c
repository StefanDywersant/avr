#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "debug.h"
#include "spi.h"
#include "nrf905.h"

static char led = 0xff;

int main(void) {
	DDRD = 0xFF;


	while (1) {
		PORTD = led;
		_delay_ms(255);
		PORTD = ~led;
		_delay_ms(255);
	}
}
