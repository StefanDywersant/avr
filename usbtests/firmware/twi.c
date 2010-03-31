#include "twi.h"
#include <util/twi.h>

// Private methods

void twiWriteStart() {
	TWCR = (1 << TWSTA) | (1 << TWINT) | (1 << TWEN);
}

void twiWaitForBus() {
	while (!(TWCR & (1 << TWINT)));
}

void twiWriteStop() {
	TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);
}

void twiWriteSlaveAddress(uint8_t address, uint8_t mode) {
	TWDR = (address & 0xf0) | mode;
	TWCR = (1 << TWINT) | (1 << TWEN);
}

void twiWriteDataByte(uint8_t byte) {
	TWDR = byte;
	TWCR = (1 << TWINT) | (1 << TWEN);
}

void twiReadDataByte(uint8_t *byte, uint8_t ack) {
	if (ack)
		TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
	else
		TWCR = (1 << TWINT) | (1 << TWEN);

	twiWaitForBus();

	*byte = TWDR;
}

// Public methods

void twiInit(uint32_t bitrate) {
	// clear status register
	TWSR = 0x00;

	// set bitrate
	TWBR = ((F_CPU / bitrate) - 16) / 2;
}

uint8_t twiSyncMT(uint8_t address, uint8_t *requestData, uint8_t requestLen) {
	// write start condition
	twiWriteStart();
	twiWaitForBus();

	// write SLA+W
	twiWriteSlaveAddress(address, TW_WRITE);
	twiWaitForBus();

	if (TW_STATUS != TW_MT_SLA_ACK) {
		twiWriteStop();
		return TWI_ERROR;
	}

	// write request data
	while (requestLen--) {
		twiWriteDataByte(*requestData++);
		twiWaitForBus();

		if (TW_STATUS != TW_MT_DATA_ACK) {
			twiWriteStop();
			return TWI_ERROR;
		}
	}

	// write stop condition
	twiWriteStop();

	while (!(TWCR & (1 << TWSTO)));

	return TWI_OK;
}

uint8_t twiSyncMTMR(uint8_t address, uint8_t *requestData, uint8_t requestLen, uint8_t *responseData, uint8_t responseLen) {

	// Master Transmitter

	// write start condition
	twiWriteStart();
	twiWaitForBus();

	// write SLA+W
	twiWriteSlaveAddress(address, TW_WRITE);
	twiWaitForBus();

	if (TW_STATUS != TW_MT_SLA_ACK) {
		twiWriteStop();
		return TWI_ERROR;
	}

	// write request data
	while (requestLen--) {
		twiWriteDataByte(*requestData++);
		twiWaitForBus();

		if (TW_STATUS != TW_MT_DATA_ACK) {
			twiWriteStop();
			return TWI_ERROR;
		}
	}

	// Master Receiver

	// write repeated start
	twiWriteStart();
	twiWaitForBus();

	if (TW_STATUS != TW_REP_START) {
		twiWriteStop();
		return TWI_ERROR;
	}

	// write SLA+R
	twiWriteSlaveAddress(address, TW_READ);
	twiWaitForBus();

	if (TW_STATUS != TW_MR_SLA_ACK) {
		twiWriteStop();
		return TWI_ERROR;
	}

	// read data
	while (responseLen > 1) {
		twiReadDataByte(responseData++, 1);
		twiWaitForBus();

		if (TW_STATUS != TW_MR_DATA_ACK) {
			twiWriteStop();
			return TWI_ERROR;
		}

		responseLen--;
	}

	twiReadDataByte(responseData++, 0);
	twiWaitForBus();

	if (TW_STATUS != TW_MR_DATA_NACK) {
		twiWriteStop();
		return TWI_ERROR;
	}

	// write stop condition
	twiWriteStop();

	while (!(TWCR & (1 << TWSTO)));

	return TWI_OK;
}
