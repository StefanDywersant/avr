/**
 * Sentinel
 * Multi-aquarium controller.
 *
 * Copyright: (c) 2010 by Karol Maciaszek
 * E-mail   : <karol.maciaszek@gmail.com>
 * WWW      : http://www.czystybeton.pl/
 *
 * $Id$
 */


#include <stdlib.h>
#include "pcf8583.h"

#define ADDR_CRSR 0x00
#define ADDR_MILISECOND 0x01
#define ADDR_SECOND 0x02
#define ADDR_MINUTE 0x03
#define ADDR_HOUR 0x04
#define ADDR_YEAR_DATE 0x05
#define ADDR_WEEKDAY_MONTH 0x06


uint8_t pcf8583_get_datetime(uint8_t addr, pcf8583_datetime_t* dt) {
	uint8_t type;

	// milisecond
	type = ADDR_MILISECOND;
	if (twi_sync_mtmr(addr, &type, 1, &dt->milisecond, 1) != TWI_OK)
		return PCF8583_ERROR;
	dt->milisecond = (dt->milisecond >> 4) * 0x0a + (dt->milisecond & 0x0f);

	// second
	type = ADDR_SECOND;
	if (twi_sync_mtmr(addr, &type, 1, &dt->second, 1) != TWI_OK)
		return PCF8583_ERROR;
	dt->second = (dt->second >> 4) * 0x0a + (dt->second & 0x0f);

	// minute
	type = ADDR_MINUTE;
	if (twi_sync_mtmr(addr, &type, 1, &dt->minute, 1) != TWI_OK)
		return PCF8583_ERROR;
	dt->minute = (dt->minute >> 4) * 0x0a + (dt->minute & 0x0f);

	// hour
	type = ADDR_HOUR;
	if (twi_sync_mtmr(addr, &type, 1, &dt->hour, 1) != TWI_OK)
		return PCF8583_ERROR;
	dt->hour = (dt->hour >> 4) * 0x0a + (dt->hour & 0x0f);

	// year/date
	type = ADDR_YEAR_DATE;
	uint8_t buf;
	if (twi_sync_mtmr(addr, &type, 1, &buf, 1) != TWI_OK)
		return PCF8583_ERROR;
	dt->year = buf >> 6; // 0 - 3
	dt->date = ((buf & 0x30) >> 4) * 0x0a + (buf & 0x0f); // 0 - 31

	// weekday/month
	type = ADDR_WEEKDAY_MONTH;
	if (twi_sync_mtmr(addr, &type, 1, &buf, 1) != TWI_OK)
		return PCF8583_ERROR;
	dt->weekday = buf >> 5; // 0 - 6
	dt->month = ((buf & 0x10) >> 4) * 0x0a + (buf & 0x0f); // 1 - 12

	return PCF8583_OK;
}


uint8_t pcf8583_set_datetime(uint8_t addr, pcf8583_datetime_t* dt) {
	uint8_t request[7];

	request[0] = 0x01;
	request[1] = ((dt->milisecond / 0xa) << 4) | (dt->milisecond - (dt->milisecond / 0xa) * 0xa);
	request[2] = ((dt->second / 0xa) << 4) | (dt->second - (dt->second / 0xa) * 0xa);
	request[3] = ((dt->minute / 0xa) << 4) | (dt->minute - (dt->minute / 0xa) * 0xa);
	request[4] = ((dt->hour / 0xa) << 4) | (dt->hour - (dt->hour / 0xa) * 0xa);
	request[5] = (dt->year << 6) | ((dt->date / 0xa) << 4) | (dt->date - (dt->date / 0xa) * 0xa);
	request[6] = (dt->weekday << 5) | ((dt->month / 0xa) << 4) | (dt->month - (dt->month / 0xa) * 0xa);

	if (twi_sync_mt(addr, request, 7) != TWI_OK)
		return PCF8583_ERROR;

	return PCF8583_OK;
}


pcf8583_datetime_t* pcf8583_new_datetime(void) {
	return (pcf8583_datetime_t*)malloc(sizeof(pcf8583_datetime_t));
}
