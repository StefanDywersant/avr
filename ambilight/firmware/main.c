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

// debug
#include "debug.h"

int main(void) {
	PRINTF("\nAmbilight Board\n");

	while (1) {
		usbPoll();
	}
}
