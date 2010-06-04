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

#include <stdint.h>
#include <stdlib.h>

void rfproto_async_tx(uint8_t len, uint8_t* payload) {
	uint8_t offset = 2 + (uint8_t)((30 - len) * rand() / RAND_MAX);
	uint8_t i;

	for (i = offset + len; i > offset; i--)
		payload[i] = payload[i - offset];

	payload[0] = offset;
	payload[1] = len;
}
