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

#include <avr/io.h>

#define LEDS_COUNT		20		// liczba diód
#define FILL_LEVELS		50		// liczba poziomów wypełnienia poszczególnej składowej

// mapa portów/pinów dla linii danych
#define D0_PORT			C
#define D0_PIN			0
#define D1_PORT			C
#define D1_PIN			1
#define D2_PORT			C
#define D2_PIN			2
#define D3_PORT			C
#define D3_PIN			3
#define D4_PORT			C
#define D4_PIN			4
#define D5_PORT			C
#define D5_PIN			5
#define D6_PORT			C
#define D6_PIN			6
#define D7_PORT			C
#define D7_PIN			7

// mapa portów/pinów dla linii adresowych
#define A0_PORT			A
#define A0_PIN			0
#define A1_PORT			A
#define A1_PIN			1
#define A2_PORT			A
#define A2_PIN			2
#define A3_PORT			A
#define A3_PIN			3
#define A4_PORT			A
#define A4_PIN			4
#define A5_PORT			A
#define A5_PIN			5
#define A6_PORT			A
#define A6_PIN			6
#define A7_PORT			A
#define A7_PIN			7

#endif /* RGBLEDCONFIG_H_ */
