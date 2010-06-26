/**
 * Ambilight Project
 * Console library test.
 *
 * Copyright: (c) 2010 by Karol Maciaszek
 * E-mail   : <karol.maciaszek@gmail.com>
 * WWW      : http://www.czystybeton.pl/
 *
 * $Id$
 */


#include <ambilight.h>


int main(int argc, char** argv) {
	struct ambilight_config config;

	config.edge_thickness = 50;
	config.margin_bottom = 0;
	config.margin_left = 0;
	config.margin_right = 0;
	config.margin_top = 0;
	config.leds_bottom = 6;
	config.leds_left = 4;
	config.leds_right = 4;
	config.leds_top = 6;
	config.frequency = 25;

	ambilight_init(&config);

	while (1)
		ambilight_refresh();

	return 0;
}
