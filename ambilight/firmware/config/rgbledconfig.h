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


#ifndef RGBLEDCONFIG_H_
#define RGBLEDCONFIG_H_

#define LEDS_COUNT		2 // liczba diód

#define SETUP_PORTS()	{\
							led_states[0].r_port = PORTA;\
							led_states[0].r_pin = 0;\
							led_states[0].g_port = PORTA;\
							led_states[0].g_pin = 1;\
							led_states[0].b_port = PORTA;\
							led_states[0].b_pin = 2;\
							\
							led_states[1].r_port = PORTA;\
							led_states[1].r_pin = 3;\
							led_states[1].g_port = PORTA;\
							led_states[1].g_pin = 4;\
							led_states[1].b_port = PORTA;\
							led_states[1].b_pin = 5;\
						}

#define FREQ			50 // częstotliwość podstawowa diody led (realna częstotliwość jest iloczynem wraz z liczbą poziomów wypełnienia)
#define FILL_LEVELS		5  // liczba poziomów wypełnienia poszczególnej składowej

#endif /* RGBLEDCONFIG_H_ */
