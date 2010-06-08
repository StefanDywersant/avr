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
#include "debug.h"
#include "rc2.h"


static void (*rx_func)(uint8_t len, uint8_t* buf) = NULL;

static void shift(uint8_t len, uint8_t* msg, uint8_t* raw) {
	uint8_t offset = 2 + (rand() % (31 - len));
	uint8_t i;

	for (i = 0; i < 32; i++)
		if (i < offset || i >= offset + len)
			raw[i] = (uint8_t)rand();
		else
			raw[i] = msg[i - offset];

	raw[0] = offset;
	raw[1] = len;
}


static uint8_t unshift(uint8_t len, uint8_t* msg, uint8_t* raw) {
	uint8_t i;

	for (i = 0; i < (len <= (raw[1] & 0x1f) ? len : raw[1] & 0x1f); i++)
		msg[i] = raw[i + (raw[0] & 0x1f)];

	return i;
}

void rfproto_async_tx(uint8_t len, uint8_t* msg, uint8_t type) {
	uint8_t raw[32];
	uint8_t i;

	shift(len, msg, raw);

	raw[0] |= (type & 0x07) << 5;
	raw[1] |= (type & 0x38) << 5;

	for (i = 0; i < 32; i++)
		PRINTF("%02x", raw[i]);

	PRINTF("\n");

	rc2_encrypt(32, raw);

	for (i = 0; i < 32; i++)
		PRINTF("%02x", raw[i]);

	PRINTF("\n");
}

uint8_t rfproto_rx(uint8_t len, uint8_t* buf, uint8_t* type) {
	uint8_t i;

	uint8_t raw[32];
	raw[0] = 0xc4;
	raw[1] = 0x24;
	raw[3] = 0xaa;
	raw[4] = 0xff;
	raw[5] = 0xff;
	raw[6] = 0xff;
	raw[7] = 0xaa;

	rc2_decrypt(32, raw);

	*type = (raw[0] & 0xe0) >> 5;
	*type |= (raw[1] & 0xe0) >> 2;
	uint8_t real_len = unshift(len, buf, raw);

	PRINTF("real_len=%d type=%d ", real_len, *type);

	for (i = 0; i < len; i++)
		PRINTF("%02x", buf[i]);

	PRINTF("\n");

	return real_len;
}


void rfproto_set_rx_func(void (*func)(uint8_t len, uint8_t* buf)) {
	rx_func = func;
}
