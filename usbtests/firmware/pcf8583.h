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
#define PCF8583_OK 0x00
#define PCF8583_ERROR 0x01

typedef struct {
	uint8_t milisecond;
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t date;
	uint8_t month;
	uint8_t year;
	uint8_t weekday;
} pcf8583TimeStruct;

uint8_t pcf8583GetTime(uint8_t addr, pcf8583TimeStruct* time);


#endif /* PCF8583_H_ */
