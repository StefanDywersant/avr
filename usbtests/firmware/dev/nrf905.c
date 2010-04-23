/*
 * nrf905.c
 *
 *  Created on: 2010-04-22
 *      Author: kharg
 */

#include "spi.h"

#define CMD_WRITE_CONFIG			0x00
#define CMD_READ_CONFIG				0x10

#define CFG_CHANNEL					256L

static uint8_t cr[10];

void nrf905_init() {

}

uint16_t nrf905_get_channel_no() {
	uint8_t buf[2];

	spi_begin();

	spi_read_write_byte(CMD_READ_CONFIG);
	spi_read_data(2, buf);

	spi_end();

	return ((buf[1] & 0x01) << 8) | buf[0];
}

uint32_t nrf905_get_address() {
	uint8_t buf[4];

	spi_begin();

	spi_read_write_byte(CMD_READ_CONFIG | 0x05);
	spi_read_data(4, buf);

	spi_end();

	return ((uint32_t)buf[3] << 24) | ((uint32_t)buf[2] << 16) | ((uint32_t)buf[1] << 8) | (uint32_t)buf[0];
}

static void nrf905_read_control_register() {
	spi_begin();

	spi_read_write_byte(CMD_READ_CONFIG);
	spi_read_data(10, cr);

	spi_end();
}
