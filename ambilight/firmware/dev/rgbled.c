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
#include <avr/interrupt.h>

#include "debug.h"

#define CONCAT(a, b)				a ## b
#define OUTPORT(name)				CONCAT(PORT, name)
#define DDRPORT(name)				CONCAT(DDR, name)

#define GET_FILLED_CYCLES_COUNT(component)		FILL_LEVELS * component / 0xff
#define CYCLE_DELAY								1000000l / (FREQ * FILL_LEVELS)
#define LED_ADDR_PIN(no, col)					(uint8_t)(no * 3 + col) / 8
#define LED_DATA_PIN(no, col)					(uint8_t)(no * 3 + col) - LED_ADDR_PIN(no, col) * 8


typedef struct {
	uint8_t r_cycles;
	uint8_t g_cycles;
	uint8_t b_cycles;
} led_state;


static led_state* led_states;


static void set_data_pin(uint8_t data_pin, uint8_t value) {
	switch (data_pin) {
	case 0:
		if (value)
			OUTPORT(D0_PORT) |= 0x01 << D0_PIN;
		else
			OUTPORT(D0_PORT) &= ~(0x01 << D0_PIN);
		break;
	case 1:
		if (value)
			OUTPORT(D1_PORT) |= 0x01 << D1_PIN;
		else
			OUTPORT(D1_PORT) &= ~(0x01 << D1_PIN);
		break;
	case 2:
		if (value)
			OUTPORT(D2_PORT) |= 0x01 << D2_PIN;
		else
			OUTPORT(D2_PORT) &= ~(0x01 << D2_PIN);
		break;
	case 3:
		if (value)
			OUTPORT(D3_PORT) |= 0x01 << D3_PIN;
		else
			OUTPORT(D3_PORT) &= ~(0x01 << D3_PIN);
		break;
	case 4:
		if (value)
			OUTPORT(D4_PORT) |= 0x01 << D4_PIN;
		else
			OUTPORT(D4_PORT) &= ~(0x01 << D4_PIN);
		break;
	case 5:
		if (value)
			OUTPORT(D5_PORT) |= 0x01 << D5_PIN;
		else
			OUTPORT(D5_PORT) &= ~(0x01 << D5_PIN);
		break;
	case 6:
		if (value)
			OUTPORT(D6_PORT) |= 0x01 << D6_PIN;
		else
			OUTPORT(D6_PORT) &= ~(0x01 << D6_PIN);
		break;
	case 7:
		if (value)
			OUTPORT(D7_PORT) |= 0x01 << D7_PIN;
		else
			OUTPORT(D7_PORT) &= ~(0x01 << D7_PIN);
		break;
	}
}


static void set_clock(uint8_t addr_pin) {
	switch (addr_pin) {
	case 0:
		OUTPORT(A0_PORT) |= 0x01 << A0_PIN;
		OUTPORT(A0_PORT) &= ~(0x01 << A0_PIN);
		break;
	case 1:
		OUTPORT(A1_PORT) |= 0x01 << A1_PIN;
		OUTPORT(A1_PORT) &= ~(0x01 << A1_PIN);
		break;
	case 2:
		OUTPORT(A2_PORT) |= 0x01 << A2_PIN;
		OUTPORT(A2_PORT) &= ~(0x01 << A2_PIN);
		break;
	case 3:
		OUTPORT(A3_PORT) |= 0x01 << A3_PIN;
		OUTPORT(A3_PORT) &= ~(0x01 << A3_PIN);
		break;
	case 4:
		OUTPORT(A4_PORT) |= 0x01 << A4_PIN;
		OUTPORT(A4_PORT) &= ~(0x01 << A4_PIN);
		break;
	case 5:
		OUTPORT(A5_PORT) |= 0x01 << A5_PIN;
		OUTPORT(A5_PORT) &= ~(0x01 << A5_PIN);
		break;
	case 6:
		OUTPORT(A6_PORT) |= 0x01 << A6_PIN;
		OUTPORT(A6_PORT) &= ~(0x01 << A6_PIN);
		break;
	case 7:
		OUTPORT(A7_PORT) |= 0x01 << A7_PIN;
		OUTPORT(A7_PORT) &= ~(0x01 << A7_PIN);
		break;
	}

}


void rgbled_set_colour(uint8_t led_no, rgbled_colour* colour) {
	led_states[led_no].r_cycles = GET_FILLED_CYCLES_COUNT(colour->r);
	led_states[led_no].g_cycles = GET_FILLED_CYCLES_COUNT(colour->g);
	led_states[led_no].b_cycles = GET_FILLED_CYCLES_COUNT(colour->b);
}


void rgbled_refresh() {
	uint8_t i;

	for (i = 0; i < FILL_LEVELS; i++) {
		uint8_t data_pin = 0;
		uint8_t addr_pin = 0;
		uint8_t led_no = 0;
		uint8_t comp_no = 0;
		while (1) {
			if (data_pin == 8 || (addr_pin * 8 + data_pin) == (LEDS_COUNT - 1) * 8 + 2) {
				set_clock(addr_pin);
				data_pin = 0;
				addr_pin++;
			}

			if (comp_no == 3) {
				comp_no = 0;
				led_no++;
			}

			if (led_no >= LEDS_COUNT)
				break;

//			PRINTF("led_no=%d addr_pin=%d data_pin=%d\n", led_no, addr_pin, data_pin);

			if (comp_no == 0) {
				if (i >= led_states[led_no].r_cycles)
					set_data_pin(data_pin, 0x01);
				else
					set_data_pin(data_pin, 0x00);
			} else if (comp_no == 1) {
				if (i >= led_states[led_no].g_cycles)
					set_data_pin(data_pin, 0x01);
				else
					set_data_pin(data_pin, 0x00);
			} else {
				if (i >= led_states[led_no].b_cycles)
					set_data_pin(data_pin, 0x01);
				else
					set_data_pin(data_pin, 0x00);
			}

			data_pin++;
			comp_no++;
		}
	}
}


void rgbled_init() {
	led_states = (led_state*)malloc(LEDS_COUNT * sizeof(led_state));
	DDRPORT(A0_PORT) |= 0x01 << A0_PIN;
	OUTPORT(A0_PORT) &= ~(0x01 << A0_PIN);
	DDRPORT(A1_PORT) |= 0x01 << A1_PIN;
	OUTPORT(A1_PORT) &= ~(0x01 << A1_PIN);
	DDRPORT(A2_PORT) |= 0x01 << A2_PIN;
	OUTPORT(A2_PORT) &= ~(0x01 << A2_PIN);
	DDRPORT(A3_PORT) |= 0x01 << A3_PIN;
	OUTPORT(A3_PORT) &= ~(0x01 << A3_PIN);
	DDRPORT(A4_PORT) |= 0x01 << A4_PIN;
	OUTPORT(A4_PORT) &= ~(0x01 << A4_PIN);
	DDRPORT(A5_PORT) |= 0x01 << A5_PIN;
	OUTPORT(A5_PORT) &= ~(0x01 << A5_PIN);
	DDRPORT(A6_PORT) |= 0x01 << A6_PIN;
	OUTPORT(A6_PORT) &= ~(0x01 << A6_PIN);
	DDRPORT(A7_PORT) |= 0x01 << A7_PIN;
	OUTPORT(A7_PORT) &= ~(0x01 << A7_PIN);
	DDRPORT(D0_PORT) |= 0x01 << D0_PIN;
	DDRPORT(D1_PORT) |= 0x01 << D1_PIN;
	DDRPORT(D2_PORT) |= 0x01 << D2_PIN;
	DDRPORT(D3_PORT) |= 0x01 << D3_PIN;
	DDRPORT(D4_PORT) |= 0x01 << D4_PIN;
	DDRPORT(D5_PORT) |= 0x01 << D5_PIN;
	DDRPORT(D6_PORT) |= 0x01 << D6_PIN;
	DDRPORT(D7_PORT) |= 0x01 << D7_PIN;
}
