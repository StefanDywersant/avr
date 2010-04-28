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
} pcf8583_datetime_t;

uint8_t pcf8583_get_datetime(uint8_t addr, pcf8583_datetime_t* dt);


/**
 * Sets current date/time on pcf8583 chip.
 *
 * @author Karol Maciaszek
 * @param addr Address of pcf8583 chip
 * @param time Pointer to DateTime struct
 * @return PCF8583_OK on success, PCF8583_ERROR otherwise
 */
uint8_t pcf8583_set_datetime(uint8_t addr, pcf8583_datetime_t* dt);

/**
 * Allocates memory for new pcf8583_datetime_t.
 *
 * @author Karol Maciaszek
 * @return Pointer to allocated memory
 */
pcf8583_datetime_t* pcf8583_new_datetime(void);

#endif /* PCF8583_H_ */
