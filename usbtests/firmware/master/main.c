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

void txPacket(void) {
	uint8_t payload[32];

	uint8_t i;
	for (i = 0; i < 32; i++)
		payload[i] = i;

	nrf905_tx_packet(0x12345678, 32, payload);
}


void dupa(void) {
	PORTD = 0xff;
}

void dupa2(void) {
	PORTD = 0x00;
}

int main(void) {
	debug_init();

	PRINTF("Sentinel");

	DDRD = 0xFF;

	PORTD = 0x01;
	spi_init();
	PORTD = 0x02;

	PORTD = 0x03;
	nrf905_init();

	sei();

	nrf905_set_packet_tx_callback(dupa);
	nrf905_set_packet_rx_callback(dupa2);

	PORTD = 0x04;

	uint8_t i = 0x05;

	while (1) {
		_delay_ms(1000);
		PORTD = i++;
		txPacket();

	}
}
