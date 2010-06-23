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


#ifndef RGBLED_H_
#define RGBLED_H_

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} rgbled_colour;


void rgbled_set_colour(uint8_t led_no, rgbled_colour* colour);

void rgbled_refresh();

void rgbled_init(void (*func)(void));


#endif /* RGBLED_H_ */
