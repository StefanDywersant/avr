/**
 *
 *
 * @author Karol Maciaszek <karol.maciaszek@gmail.com>
 * $Id$
 */


#include <stdlib.h>
#include "debug.h"
#include "spi.h"
#include "nrf905.h"
#include "nrf905config.h"

// port naming macors
#define CONCAT(a, b)				a ## b
#define INPORT(name)				CONCAT(PIN, name)
#define OUTPORT(name)				CONCAT(PORT, name)
#define DDRPORT(name)				CONCAT(DDR, name)

// SPI commands
#define WRITE_CONFIG				0x00 // write config register
#define READ_CONFIG					0x10 // read config register
#define CHANNEL_CONFIG				0x80 // fast setting of CH_NO, HFREQ_PLL and PA_PWR
#define WRITE_TX_PAYLOAD			0x20 // write TX payload
#define READ_TX_PAYLOAD				0x21 // read TX payload
#define WRITE_TX_ADDRESS			0x22 // write TX address
#define READ_TX_ADDRESS				0x23 // read TX address
#define READ_RX_PAYLOAD				0x24 // read RX payload

// modes of operation
#define POWER_DOWN()				{ OUTPORT(PORT) &= ~(0x01 << PWR_UP_PIN); }
#define STANDBY()					{ \
										OUTPORT(PORT) |= (0x01 << PWR_UP_PIN); \
										OUTPORT(PORT) &= ~(0x01 << TRX_CE_PIN); \
									}
#define READ_RX()					{ \
										OUTPORT(PORT) |= (0x01 << PWR_UP_PIN); \
										OUTPORT(PORT) &= ~(0x01 << TX_EN_PIN); \
									}
#define TX_PACKET()					{ OUTPORT(PORT) |= (0x01 << PWR_UP_PIN) | (0x01 << TRX_CE_PIN) | (0x01 << TX_EN_PIN); }
#define RX_PACKET()					{ \
										OUTPORT(PORT) |= (0x01 << PWR_UP_PIN) | (0x01 << TRX_CE_PIN); \
										OUTPORT(PORT) &= ~(0x01 << TX_EN_PIN); \
									}

// get TX_EN state
#define GET_TX_EN					(OUTPORT(PORT) & (0x01 << TX_EN_PIN)) >> TX_EN_PIN

static void (*on_packet_rx)(void) = NULL;

static void (*on_packet_tx)(void) = NULL;

static uint8_t command_read(uint8_t cmd, uint8_t len, uint8_t* buf) {
	uint8_t sr;

	spi_begin();
	sr = spi_read_write_byte(cmd);
	spi_read_data(len, buf);
	spi_end();

	return sr;
}


static uint8_t command_write(uint8_t cmd, uint8_t len, uint8_t* buf) {
	uint8_t sr;

	spi_begin();
	sr = spi_read_write_byte(cmd);
	spi_write_data(len, buf);
	spi_end();

	return sr;
}


void nrf905_init(void) {
	// setting up port
	DDRPORT(PORT) |= (0x01 << TX_EN_PIN) | (0x01 << TRX_CE_PIN) | (0x01 << PWR_UP_PIN);
	DDRPORT(PORT) &= ~(0x01 << DR_PIN);

	// setting chip mode to standby
	STANDBY();

	// configuring nrf905 chip
	uint8_t* buf;

	buf = (uint8_t*)malloc(10 * sizeof(uint8_t));
	buf[0] = (uint8_t)(CH_NO & 0xff);
	buf[1] = (uint8_t)((CH_NO & 0x100) >> 8) | HFREQ_PLL | PA_PWR | RX_RED_PWR | AUTO_RETRAN;
	buf[2] = RX_AFW | TX_AFW;
	buf[3] = RX_PW;
	buf[4] = TX_PW;
	buf[5] = (uint8_t)(RX_ADDRESS & 0x000000ff);
	buf[6] = (uint8_t)((RX_ADDRESS & 0x0000ff00) >> 8);
	buf[7] = (uint8_t)((RX_ADDRESS & 0x00ff0000) >> 16);
	buf[8] = (uint8_t)((RX_ADDRESS & 0xff000000) >> 24);
	buf[9] = UP_CLK_FREQ | UP_CLK_EN | XOF | CRC_EN | CRC_MODE;

	// write config register
	command_write(WRITE_CONFIG | 0x00, 10, buf);

	// setup data ready interrupt
	DR_SETUP();

	free(buf);
}


uint16_t nrf905_get_channel_no(void) {
	uint8_t buf[2];

	STANDBY();
	command_read(READ_CONFIG | 0x00, 2, buf);

	return (((uint16_t)buf[1] & 0x01) << 8) | buf[0];
}

void nrf905_set_channel_no(uint16_t no) {
	command_write(CHANNEL_CONFIG | PA_PWR | HFREQ_PLL | ((no & 0x0100) >> 8), 1, (uint8_t*)&no);
}


uint32_t nrf905_get_rx_address(void) {
	uint8_t buf[4];

	STANDBY();
	command_read(READ_CONFIG | 0x05, 4, buf);

	return ((uint32_t)buf[3] << 24) | ((uint32_t)buf[2] << 16) | ((uint32_t)buf[1] << 8) | (uint32_t)buf[0];
}


void nrf905_set_rx_address(uint32_t addr) {
	STANDBY();
	command_write(WRITE_CONFIG | 0x05, 4, (uint8_t*)&addr);
}


void nrf905_read_control_register(uint8_t* buf) {
	STANDBY();
	command_read(READ_CONFIG | 0x00, 10, buf);
}


void nrf905_tx_packet(uint32_t addr, uint8_t len, uint8_t* payload) {
	STANDBY();
	command_write(WRITE_TX_ADDRESS, 4, (uint8_t*)&addr);
	command_write(WRITE_TX_PAYLOAD, len, payload);
	TX_PACKET();
}


uint32_t nrf905_get_tx_address(void) {
	uint8_t buf[4];

	STANDBY();
	command_read(READ_TX_ADDRESS, 4, buf);

	return ((uint32_t)buf[3] << 24) | ((uint32_t)buf[2] << 16) | ((uint32_t)buf[1] << 8) | (uint32_t)buf[0];
}


void nrf905_get_tx_payload(uint8_t* payload) {
	STANDBY();
	command_read(READ_TX_PAYLOAD, 32, payload);
}


void nrf905_rx_packet(uint8_t len, uint8_t* payload) {
	READ_RX();
	command_read(READ_RX_PAYLOAD, len, payload);
}


uint8_t nrf905_get_status_register(void) {
	STANDBY();
	return command_read(READ_CONFIG, 0, NULL);
}

void nrf905_set_packet_rx_callback(void (*func)(void)) {
	on_packet_rx = func;
}

void nrf905_set_packet_tx_callback(void (*func)(void)) {
	on_packet_tx = func;
}

ISR(DR_INT_VECT) {
	if (GET_TX_EN) {
		// transmit complete
		if (on_packet_tx != NULL)
			on_packet_tx();

		RX_PACKET();
	} else {
		// receive complete
		if (on_packet_rx != NULL)
			on_packet_rx();

		RX_PACKET();
	}
}

