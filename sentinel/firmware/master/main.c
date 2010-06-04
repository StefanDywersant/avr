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


#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "debug.h"
#include "spi.h"
#include "nrf905.h"
#include "rc2.h"
#include "rfproto.h"
#include "version.h"

static uint64_t seq = 0;

void tx_packet(void) {
	uint8_t payload[32];

	uint8_t i;
	for (i = 0; i < 32; i++)
		payload[i] = 0;

	payload[0] = (uint8_t)(seq & 0x00000000000000ff);
	payload[1] = (uint8_t)((seq & 0x000000000000ff00) >> 8);
	payload[2] = (uint8_t)((seq & 0x0000000000ff0000) >> 16);
	payload[3] = (uint8_t)((seq & 0x00000000ff000000) >> 24);
	payload[4] = (uint8_t)((seq & 0x000000ff00000000) >> 32);
	payload[5] = (uint8_t)((seq & 0x0000ff0000000000) >> 40);
	payload[6] = (uint8_t)((seq & 0x00ff000000000000) >> 48);
	payload[7] = (uint8_t)((seq & 0xff00000000000000) >> 56);

	rc2_encrypt(32, payload);

	rfproto_async_tx(8, payload);

	nrf905_tx_packet(0x12345678, 32, payload);

	PRINTF("tx_packet; seq=%016llx\n", seq);

	seq++;
}


void on_tx(void) {
	PRINTF("on_tx\n");
}

void on_rx(void) {
	PRINTF("on_rx\n");
}

int main(void) {
	uint8_t key[8] = {0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	debug_init();

	_delay_ms(1000);

	PRINTF("Sentinel Master Board, version %s\n", VERSION);

	PRINTF("Initializing SPI... ");
	spi_init();
	PRINTF("done.\n");

	PRINTF("Initializing NRF905... ");
	nrf905_init();
	nrf905_set_packet_tx_callback(on_tx);
	nrf905_set_packet_rx_callback(on_rx);
	PRINTF("done.\n");

	PRINTF("Initializing rc2 cipher... ");
	rc2_set_key(key);
	PRINTF("done\n");

	PRINTF("Initializing interrupts... ");
	sei();
	PRINTF("done.\n");

	DDRC = 0xff;

	while (1) {
		tx_packet();
		_delay_ms(500);
		PORTC = 0xff;
		_delay_ms(500);
		PORTC = 0x00;
	}
}
