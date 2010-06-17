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

#include "rgbled.h"
#include "debug.h"

int main(void) {
	PRINTF("\nAmbilight - firmware\n");

	PRINTF("Inicjalizacja bufora ed... ");
	rgbled_init();
	PRINTF("ok\n");

	PRINTF("Pętla główna\n");

	while (1) {
		rgbled_refresh();
	}
}
