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
#define PCF8583_ADDR_YEAR_DATE 0x05
#define PCF8583_ADDR_WEEKDAY_MONTH 0x06

uint8_t pcf8583GetDateTime(uint8_t addr, pcf8583DateTimeStruct* time) {
	uint8_t type;

	// milisecond
	type = PCF8583_ADDR_MILISECOND;
	if (twiSyncMTMR(addr, &type, 1, &time->milisecond, 1) != TWI_OK)
		return 0xfe;
	time->milisecond = (time->milisecond >> 4) * 0x0a + (time->milisecond & 0x0f);

	// second
	type = PCF8583_ADDR_SECOND;
	if (twiSyncMTMR(addr, &type, 1, &time->second, 1) != TWI_OK)
		return 0xfd;
	time->second = (time->second >> 4) * 0x0a + (time->second & 0x0f);

	// minute
	type = PCF8583_ADDR_MINUTE;
	if (twiSyncMTMR(addr, &type, 1, &time->minute, 1) != TWI_OK)
		return PCF8583_ERROR;
	time->minute = (time->minute >> 4) * 0x0a + (time->minute & 0x0f);

	// hour
	type = PCF8583_ADDR_HOUR;
	if (twiSyncMTMR(addr, &type, 1, &time->hour, 1) != TWI_OK)
		return PCF8583_ERROR;
	time->hour = (time->hour >> 4) * 0x0a + (time->hour & 0x0f);

	// year/date
	type = PCF8583_ADDR_YEAR_DATE;
	uint8_t buf;
	if (twiSyncMTMR(addr, &type, 1, &buf, 1) != TWI_OK)
		return PCF8583_ERROR;
	time->year = buf & 0xf0;
	time->date = buf & 0x0f;

	// year/date
	type = PCF8583_ADDR_WEEKDAY_MONTH;
	if (twiSyncMTMR(addr, &type, 1, &buf, 1) != TWI_OK)
		return PCF8583_ERROR;
	time->weekday = buf & 0xf0;
	time->month = buf & 0x0f;

	return PCF8583_OK;
}
