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

#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdio.h>
#include <avr/pgmspace.h>

#if DEBUG == 1

#define PRINTF(f, ...)		{ printf_P(PSTR(f), ##__VA_ARGS__); }

void debug_init(void);

#else

#define PRINTF(f, ...)		{ }

#endif /* DEBUG */
#endif /* DEBUG_H_ */
