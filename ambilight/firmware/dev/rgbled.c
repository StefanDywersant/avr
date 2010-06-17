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

#include <util/delay.h>
#include <stdlib.h>
#include "rgbled.h"
#include "rgbledconfig.h"

#include "debug.h"

#define GET_FILLED_CYCLES_COUNT(component)		FILL_LEVELS * component / 0xff
#define CYCLE_DELAY								1000000l / FREQ * FILL_LEVELS

typedef struct {
	uint8_t r_port;
	uint8_t r_pin;
	uint8_t b_port;
	uint8_t b_pin;
	uint8_t g_port;
	uint8_t g_pin;
	uint8_t r_cycles;
	uint8_t g_cycles;
	uint8_t b_cycles;
} led_state;

static led_state* led_states;


void rgbled_set_colour(uint8_t led_no, rgbled_colour* colour) {
	PRINTF("r=%d g=%d b=%d\n", colour->r, colour->g, colour->b);
	led_states[led_no].r_cycles = GET_FILLED_CYCLES_COUNT(colour->r);
	led_states[led_no].g_cycles = GET_FILLED_CYCLES_COUNT(colour->g);
	led_states[led_no].b_cycles = GET_FILLED_CYCLES_COUNT(colour->b);
}


void rgbled_refresh() {
	uint8_t i, j;

	for (i = 0; i < FILL_LEVELS; i++)
		for (j = 0; j < LEDS_COUNT; i++) {
			// składowa czerwona
			if (i >= led_states[j].r_cycles)
				led_states[j].r_port &= ~(0x01 << led_states[j].r_pin);
			else
				led_states[j].r_port |= 0x01 << led_states[j].r_pin;

			// składowa zielona
			if (i >= led_states[j].b_cycles)
				led_states[j].b_port &= ~(0x01 << led_states[j].b_pin);
			else
				led_states[j].b_port |= 0x01 << led_states[j].b_pin;

			// składowa niebieska
			if (i >= led_states[j].b_cycles)
				led_states[j].g_port &= ~(0x01 << led_states[j].g_pin);
			else
				led_states[j].g_port |= 0x01 << led_states[j].g_pin;
		}

		_delay_us(CYCLE_DELAY);
}


void rgbled_init() {
	led_states = (led_state*)malloc(LEDS_COUNT * sizeof(led_state));
	SETUP_PORTS();
}
