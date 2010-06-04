/**
 * Sentinel
 * Multi-aquarium controller.
 *
 * Copyright: (c) 2010 by Karol Maciaszek
 * E-mail   : <karol.maciaszek@gmail.com>
 * WWW      : http://www.czystybeton.pl/
 *
 * $Id$
 */

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include "debug.h"

void rfproto_async_tx(uint8_t len, uint8_t* payload) {

	uint8_t data[32];
	uint8_t i;

	uint8_t offset = 2 + (rand() % (31 - len));

	PRINTF("len=%d offset=%d ", len, offset);

	for (i = 0; i < 32; i++)
		if (i < offset || i >= offset + len)
			data[i] = (uint8_t)rand() & 0xff;
		else
			data[i] = payload[i - offset];

	data[0] = offset;
	data[1] = len;

	for (i = 0; i < 32; i++)
		PRINTF("%02x", data[i]);

	PRINTF("\n");

//	uint8_t offset = 2 + (uint8_t)((30 - len) * rand() / RAND_MAX);
//	uint8_t i;

//	for (i = offset + len; i > offset; i--)
//		payload[i] = payload[i - offset];

//	payload[0] = offset;
//	payload[1] = len;
}
