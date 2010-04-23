#include "twi.h"
#include <util/twi.h>

#define SPEED				((F_CPU / TWI_BITRATE) - 16) / 2
#define WAIT_FOR_BUS()		{ while (!(TWCR & (1 << TWINT))); }


static void write_start(void) {
	TWCR = (1 << TWSTA) | (1 << TWINT) | (1 << TWEN);
}


static void write_stop(void) {
	TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);
}


static void write_slave_address(uint8_t address, uint8_t mode) {
	TWDR = (address & 0xf0) | mode;
	TWCR = (1 << TWINT) | (1 << TWEN);
}


static void write_data_byte(uint8_t byte) {
	TWDR = byte;
	TWCR = (1 << TWINT) | (1 << TWEN);
}


static void read_data_byte(uint8_t *byte, uint8_t ack) {
	if (ack)
		TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
	else
		TWCR = (1 << TWINT) | (1 << TWEN);

	WAIT_FOR_BUS();

	*byte = TWDR;
}


void twi_init(void) {
	// clear status register
	TWSR = 0x00;

	// set bitrate
	TWBR = SPEED;
}


uint8_t twi_sync_mt(uint8_t address, uint8_t *req_data, uint8_t req_len) {
	// write start condition
	write_start();
	WAIT_FOR_BUS();

	// write SLA+W
	write_slave_address(address, TW_WRITE);
	WAIT_FOR_BUS();

	if (TW_STATUS != TW_MT_SLA_ACK) {
		write_stop();
		return TWI_ERROR;
	}

	// write request data
	while (req_len--) {
		write_data_byte(*req_data++);
		WAIT_FOR_BUS();

		if (TW_STATUS != TW_MT_DATA_ACK) {
			write_stop();
			return TWI_ERROR;
		}
	}

	// write stop condition
	write_stop();

	while (!(TWCR & (1 << TWSTO)));

	return TWI_OK;
}


uint8_t twi_sync_mtmr(uint8_t address, uint8_t *req_data, uint8_t req_len, uint8_t *res_data, uint8_t res_len) {

	// Master Transmitter

	// write start condition
	write_start();
	WAIT_FOR_BUS();

	// write SLA+W
	write_slave_address(address, TW_WRITE);
	WAIT_FOR_BUS();

	if (TW_STATUS != TW_MT_SLA_ACK) {
		write_stop();
		return TWI_ERROR;
	}

	// write request data
	while (req_len--) {
		write_data_byte(*req_data++);
		WAIT_FOR_BUS();

		if (TW_STATUS != TW_MT_DATA_ACK) {
			write_stop();
			return TWI_ERROR;
		}
	}

	// Master Receiver

	// write repeated start
	write_start();
	WAIT_FOR_BUS();

	if (TW_STATUS != TW_REP_START) {
		write_stop();
		return TWI_ERROR;
	}

	// write SLA+R
	write_slave_address(address, TW_READ);
	WAIT_FOR_BUS();

	if (TW_STATUS != TW_MR_SLA_ACK) {
		write_stop();
		return TWI_ERROR;
	}

	// read data
	while (res_len > 1) {
		read_data_byte(res_data++, 1);
		WAIT_FOR_BUS();

		if (TW_STATUS != TW_MR_DATA_ACK) {
			write_stop();
			return TWI_ERROR;
		}

		res_len--;
	}

	read_data_byte(res_data++, 0);
	WAIT_FOR_BUS();

	if (TW_STATUS != TW_MR_DATA_NACK) {
		write_stop();
		return TWI_ERROR;
	}

	// write stop condition
	write_stop();

	while (!(TWCR & (1 << TWSTO)));

	return TWI_OK;
}
