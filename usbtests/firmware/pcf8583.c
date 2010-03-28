/*
 * pcf8583.c
 *
 *  Created on: 2010-03-28
 *      Author: kharg
 */

#include <pcf8583.h>

#define PCF8583_ADDR_CRSR 0x00
#define PCF8583_ADDR_MILISECOND 0x01
#define PCF8583_ADDR_SECOND 0x02
#define PCF8583_ADDR_MINUTE 0x03
#define PCF8583_ADDR_HOUR 0x04

uint8_t pcf8583GetTime(uint8_t a0, pcf8583TimeStruct* time) {
	uint8_t request[1];
	uint8_t response[1];

	// milisecond
	*request = PCF8583_ADDR_MILISECOND;
	if (twiSyncMTMR(PCF8583_ADDR | a0, request, 1, response, 1) != TWI_OK)
		return PCF8583_ERROR;
	time->milisecond = (response[0] >> 4) * 0x0a + (response[0] & 0x0f);

	// second
	*request = PCF8583_ADDR_SECOND;
	if (twiSyncMTMR(PCF8583_ADDR | a0, request, 1, response, 1) != TWI_OK)
		return PCF8583_ERROR;
	time->second = (response[0] >> 4) * 0x0a + (response[0] & 0x0f);

	// minute
	*request = PCF8583_ADDR_MINUTE;
	if (twiSyncMTMR(PCF8583_ADDR | a0, request, 1, response, 1) != TWI_OK)
		return PCF8583_ERROR;
	time->minute = (response[0] >> 4) * 0x0a + (response[0] & 0x0f);

	// hour
	*request = PCF8583_ADDR_HOUR;
	if (twiSyncMTMR(PCF8583_ADDR | a0, request, 1, response, 1) != TWI_OK)
		return PCF8583_ERROR;
	time->hour = (response[0] >> 4) * 0x0a + (response[0] & 0x0f);

	return PCF8583_OK;
}

