/*
 * nrf905.c
 *
 *  Created on: 2010-04-22
 *      Author: kharg
 */

#include "spi.h"
#include "nrf905.h"
#include "nrf905config.h"

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

}


uint16_t nrf905_get_channel_no(void) {
	uint8_t buf[2];

	command_read(CMD_READ_CONFIG | 0x00, 2, buf);

	return (((uint16_t)buf[1] & 0x01) << 8) | buf[0];
}

void nrf905_set_channel_no(uint16_t no) {
	command_write(
			CMD_CHANNEL_CONFIG
			| ((PA_PWR & 0x03) << 2)
			| ((HFREQ_PLL & 0x01) << 1)
			| ((no & 0x0100) >> 8),
			1,
			(uint8_t*)&no
		);
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
