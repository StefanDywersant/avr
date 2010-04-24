/*
 * nrf905.c
 *
 *  Created on: 2010-04-22
 *      Author: kharg
 */

#include <stdlib.h>
#include "spi.h"
#include "nrf905.h"
#include "nrf905config.h"

#define CONCAT(a, b)				a ## b
#define INPORT(name)				CONCAT(PIN, name)
#define OUTPORT(name)				CONCAT(PORT, name)
#define DDRPORT(name)				CONCAT(DDR, name)

#define TX_EN_H()					{ OUTPORT(PORT) |= 0x01 << TX_EN_PIN; }
#define TX_EN_L()					{ OUTPORT(PORT) &= ~(0x01 << TX_EN_PIN); }
#define TRX_CE_H()					{ OUTPORT(PORT) |= 0x01 << TRX_CE_PIN; }
#define TRX_CE_L()					{ OUTPORT(PORT) &= ~(0x01 << TRX_CE_PIN); }
#define PWR_UP_H()					{ OUTPORT(PORT) |= 0x01 << PWR_UP_PIN; }
#define PWR_UP_L()					{ OUTPORT(PORT) &= ~(0x01 << PWR_UP_PIN); }

#define CMD_WRITE_CONFIG			0x00
#define CMD_READ_CONFIG				0x10
#define CMD_CHANNEL_CONFIG			0x80

static void command_read(uint8_t cmd, uint8_t len, uint8_t* buf) {
	spi_begin();
	spi_read_write_byte(cmd);
	spi_read_data(len, buf);
	spi_end();
}


static void command_write(uint8_t cmd, uint8_t len, uint8_t* buf) {
	spi_begin();
	spi_read_write_byte(cmd);
	spi_write_data(len, buf);
	spi_end();
}


void nrf905_init(void) {
	// setting up port
	DDRPORT(PORT) |= (0x01 << TX_EN_PIN) | (0x01 << TRX_CE_PIN) | (0x01 << PWR_UP_PIN);
	DDRPORT(PORT) &= ~(0x01 << DR_PIN);

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

	command_write(CMD_WRITE_CONFIG | 0x00, 10, buf);

	free(buf);
}


uint16_t nrf905_get_channel_no(void) {
	uint8_t buf[2];

	command_read(CMD_READ_CONFIG | 0x00, 2, buf);

	return (((uint16_t)buf[1] & 0x01) << 8) | buf[0];
}

void nrf905_set_channel_no(uint16_t no) {
	command_write(CMD_CHANNEL_CONFIG | PA_PWR | HFREQ_PLL | ((no & 0x0100) >> 8), 1, (uint8_t*)&no);
}


uint32_t nrf905_get_address(void) {
	uint8_t buf[4];

	command_read(CMD_READ_CONFIG | 0x05, 4, buf);

	return ((uint32_t)buf[3] << 24) | ((uint32_t)buf[2] << 16) | ((uint32_t)buf[1] << 8) | (uint32_t)buf[0];
}


void nrf905_set_address(uint32_t addr) {
	command_write(CMD_WRITE_CONFIG | 0x05, 4, (uint8_t*)&addr);
}


void nrf905_read_control_register(uint8_t* buf) {
	spi_begin();

	spi_read_write_byte(CMD_READ_CONFIG | 0x00);
	spi_read_data(10, buf);

	spi_end();
}
