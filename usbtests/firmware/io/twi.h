/*
 * twi.h
 *
 *  Created on: 2010-03-27
 *      Author: kharg
 */

#ifndef TWI_H_
#define TWI_H_

#include <stdint.h>
#include <util/twi.h>
#include "twi.h"

#define TWI_OK 0x0
#define TWI_ERROR 0x1

void twiInit(uint32_t bitrate);

uint8_t twiSyncMTMR(uint8_t address, uint8_t *requestData, uint8_t requestLen, uint8_t *responseData, uint8_t responseLen);

uint8_t twiSyncMT(uint8_t address, uint8_t *requestData, uint8_t requestLen);

#endif /* TWI_H_ */
;
