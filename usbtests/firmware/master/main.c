/**
 *
 *
 * @author Karol Maciaszek <karol.maciaszek@gmail.com>
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
#include "version.h"

static uint8_t seq = 0;

void tx_packet(void) {
	uint8_t payload[32];

	uint8_t i;
	for (i = 0; i < 32; i++)
		payload[i] = seq;

	nrf905_tx_packet(0x12345678, 32, payload);

	PRINTF("tx_packet; seq=%02x\n", seq);

	seq++;
}


void on_tx(void) {
	PRINTF("on_tx\n");
}

void on_rx(void) {
	PRINTF("on_rx\n");
}

int main(void) {
	debug_init();

	DDRD = 0xFF;

	PRINTF("Sentinel Master Device, version %s\n", VERSION);

	PRINTF("Initializing SPI... ");
	spi_init();
	PRINTF("done.\n");

	PRINTF("Initializing NRF905... ");
	nrf905_init();
	nrf905_set_packet_tx_callback(on_tx);
	nrf905_set_packet_rx_callback(on_rx);
	PRINTF("done.\n");

	PRINTF("Initializing interrupts... ");
	sei();
	PRINTF("done.\n");

	while (1) {
		_delay_ms(1000);
		tx_packet();
	}
}
