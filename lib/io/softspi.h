/**
 * AVRLibs
 * Atmel AVR libraries
 *
 * Copyright: (c) 2010 by Karol Maciaszek
 * E-mail   : <karol.maciaszek@gmail.com>
 * WWW      : http://www.czystybeton.pl/
 *
 * $Id$
 */


#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>
#include "softspiconfig.h"

void softspi_init();

void softspi_begin();

void softspi_end();

uint8_t softspi_read_write_byte(uint8_t byte);

void softspi_read_data(uint8_t len, uint8_t* data);

void softspi_write_data(uint8_t len, uint8_t* data);

#endif /* SPI_H_ */
