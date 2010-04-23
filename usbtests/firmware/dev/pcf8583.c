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
	if (twi_sync_mtmr(addr, &type, 1, &time->milisecond, 1) != TWI_OK)
		return PCF8583_ERROR;
	time->milisecond = (time->milisecond >> 4) * 0x0a + (time->milisecond & 0x0f);

	// second
	type = PCF8583_ADDR_SECOND;
	if (twi_sync_mtmr(addr, &type, 1, &time->second, 1) != TWI_OK)
		return PCF8583_ERROR;
	time->second = (time->second >> 4) * 0x0a + (time->second & 0x0f);

	// minute
	type = PCF8583_ADDR_MINUTE;
	if (twi_sync_mtmr(addr, &type, 1, &time->minute, 1) != TWI_OK)
		return PCF8583_ERROR;
	time->minute = (time->minute >> 4) * 0x0a + (time->minute & 0x0f);

	// hour
	type = PCF8583_ADDR_HOUR;
	if (twi_sync_mtmr(addr, &type, 1, &time->hour, 1) != TWI_OK)
		return PCF8583_ERROR;
	time->hour = (time->hour >> 4) * 0x0a + (time->hour & 0x0f);

	// year/date
	type = PCF8583_ADDR_YEAR_DATE;
	uint8_t buf;
	if (twi_sync_mtmr(addr, &type, 1, &buf, 1) != TWI_OK)
		return PCF8583_ERROR;
	time->year = buf >> 6; // 0 - 3
	time->date = ((buf & 0x30) >> 4) * 0x0a + (buf & 0x0f); // 0 - 31

	// weekday/month
	type = PCF8583_ADDR_WEEKDAY_MONTH;
	if (twi_sync_mtmr(addr, &type, 1, &buf, 1) != TWI_OK)
		return PCF8583_ERROR;
	time->weekday = buf >> 5; // 0 - 6
	time->month = ((buf & 0x10) >> 4) * 0x0a + (buf & 0x0f); // 1 - 12

	return PCF8583_OK;
}

uint8_t pcf8583SetDateTime(uint8_t addr, pcf8583DateTimeStruct* time) {
	uint8_t request[7];

	request[0] = 0x01;
	request[1] = ((time->milisecond / 0xa) << 4) | (time->milisecond - (time->milisecond / 0xa) * 0xa);
	request[2] = ((time->second / 0xa) << 4) | (time->second - (time->second / 0xa) * 0xa);
	request[3] = ((time->minute / 0xa) << 4) | (time->minute - (time->minute / 0xa) * 0xa);
	request[4] = ((time->hour / 0xa) << 4) | (time->hour - (time->hour / 0xa) * 0xa);
	request[5] = (time->year << 6) | ((time->date / 0xa) << 4) | (time->date - (time->date / 0xa) * 0xa);
	request[6] = (time->weekday << 5) | ((time->month / 0xa) << 4) | (time->month - (time->month / 0xa) * 0xa);

	if (twi_sync_mt(addr, request, 7) != TWI_OK)
		return PCF8583_ERROR;

	return PCF8583_OK;
}

pcf8583DateTimeStruct* pcf8583NewDateTimeStruct() {
	return (pcf8583DateTimeStruct*)malloc(sizeof(pcf8583DateTimeStruct));
}
