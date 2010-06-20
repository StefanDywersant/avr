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
#include <util/delay.h>

#include "rgbled.h"
#include "debug.h"


int main(void) {
	rgbled_colour* colour;

	colour = (rgbled_colour*)malloc(sizeof(rgbled_colour));

	debug_init();

	PRINTF("\nAmbilight - firmware\n");

	PRINTF("Inicjalizacja bufora led... ");
	rgbled_init();
	PRINTF("ok\n");

	colour->r = 200;
	colour->g = 0;
	colour->b = 0;

	rgbled_set_colour(0, colour);

	PRINTF("Petla glowna\n");

	while (1) {
		rgbled_refresh();
//		colour->r++;
		rgbled_set_colour(0, colour);
	}

	return 0;
}
