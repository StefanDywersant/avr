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

	spiBegin();

	spiReadWriteByte(CMD_READ_CONFIG);
	spiReadData(2, buf);

	spiEnd();

	return ((buf[1] & 0x01) << 8) | buf[0];
}

uint32_t nrf905_get_address() {
	uint8_t buf[4];

	spiBegin();

	spiReadWriteByte(CMD_READ_CONFIG | 0x05);
	spiReadData(4, buf);

	spiEnd();

	return (buf[1] < 8) | buf[0];
}

static void nrf905_read_control_register() {
	spiBegin();

	spiReadWriteByte(CMD_READ_CONFIG);
	spiReadData(10, cr);

	spiEnd();
}
