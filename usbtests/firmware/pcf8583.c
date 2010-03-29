/*
 * pcf8583.c
 *
 *  Created on: 2010-03-28
 *      Author: kharg
 */

#include <stdlib.h>
#include "pcf8583.h"

#define PCF8583_ADDR_CRSR 0x00
#define PCF8583_ADDR_MILISECOND 0x01
#define PCF8583_ADDR_SECOND 0x02
#define PCF8583_ADDR_MINUTE 0x03
#define PCF8583_ADDR_HOUR 0x04

uint8_t pcf8583GetTime(uint8_t addr, pcf8583TimeStruct** time) {
	uint8_t type;
	pcf8583TimeStruct* _time;

	_time = malloc(sizeof(pcf8583TimeStruct));

	// milisecond
	type = PCF8583_ADDR_MILISECOND;
	if (twiSyncMTMR(addr, &type, 1, &_time->milisecond, 1) != TWI_OK)
		return 0xfe;
	_time->milisecond = (_time->milisecond >> 4) * 0x0a + (_time->milisecond & 0x0f);

	// second
	type = PCF8583_ADDR_SECOND;
	if (twiSyncMTMR(addr, &type, 1, &_time->second, 1) != TWI_OK)
		return 0xfd;
	_time->second = (_time->second >> 4) * 0x0a + (_time->second & 0x0f);

	// minute
	type = PCF8583_ADDR_MINUTE;
	if (twiSyncMTMR(addr, &type, 1, &_time->minute, 1) != TWI_OK)
		return PCF8583_ERROR;
	_time->minute = (_time->minute >> 4) * 0x0a + (_time->minute & 0x0f);

	// hour
	type = PCF8583_ADDR_HOUR;
	if (twiSyncMTMR(addr, &type, 1, &_time->hour, 1) != TWI_OK)
		return PCF8583_ERROR;
	_time->hour = (_time->hour >> 4) * 0x0a + (_time->hour & 0x0f);

	*time = _time;

	return PCF8583_OK;
}
