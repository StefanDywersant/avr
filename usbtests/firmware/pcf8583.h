/*
 * pcf8583.h
 *
 *  Created on: 2010-03-28
 *      Author: kharg
 */

#ifndef PCF8583_H_
#define PCF8583_H_

#include <stdint.h>
#include "twi.h"

#define PCF8583_ADDR_0 0xa0
#define PCF8583_ADDR_1 0xa1
#define PCF8583_OK 0x0
#define PCF8583_ERROR 0x1

typedef struct {
	uint8_t milisecond;
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t date;
	uint8_t month;
	uint8_t year;
	uint8_t weekday;
} pcf8583DateTimeStruct;

uint8_t pcf8583GetDateTime(uint8_t addr, pcf8583DateTimeStruct* time);


/**
 * Sets current date/time on pcf8583 chip.
 *
 * @author Karol Maciaszek
 * @param addr Address of pcf8583 chip
 * @param time Pointer to DateTime struct
 * @return PCF8583_OK on success, PCF8583_ERROR otherwise
 */
uint8_t pcf8583SetDateTime(uint8_t addr, pcf8583DateTimeStruct* time);


/**
 * Allocates memory for new pcf8583DateTimeStruct.
 *
 * @author Karol Maciaszek
 * @return Pointer to allocated memory
 */
pcf8583DateTimeStruct* pcf8583NewDateTimeStruct();

#endif /* PCF8583_H_ */
