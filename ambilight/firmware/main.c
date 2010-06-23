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
#include <avr/interrupt.h>

#include "debug.h"
#include "rgbled.h"
#include "usbdrv.h"


rgbled_colour* colour;


usbMsgLen_t usbFunctionSetup(uchar data[8]) {
	usbRequest_t* request = (usbRequest_t *)data;

	if (request->bRequest == 0x04)
		return USB_NO_MSG;

    return 0;
}


uchar usbFunctionRead(uchar *data, uchar len) {
	PRINTF("usbRead\n");
	return 0;
}


uchar usbFunctionWrite(uchar *data, uchar len) {
	colour->r = data[0];
	colour->g = data[1];
	colour->b = data[2];

	rgbled_set_colour(0, colour);

	return len;
}


int main(void) {
	colour = (rgbled_colour*)malloc(sizeof(rgbled_colour));

	debug_init();

	PRINTF("\nAmbilight - firmware\n");

	sei();

	PRINTF("Inicjalizacja USB... ");
	usbInit();
	usbDeviceDisconnect();
	_delay_ms(250);
	usbDeviceConnect();
	PRINTF("ok\n");

	PRINTF("Inicjalizacja bufora led... ");
	rgbled_init(&usbPoll);
	PRINTF("ok\n");

	colour->r = 0;
	colour->g = 0;
	colour->b = 0;
	rgbled_set_colour(0, colour);

	PRINTF("Petla glowna\n");

	while (1)
		rgbled_refresh();

	return 0;
}
