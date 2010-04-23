/*
 * spi.h
 *
 *  Created on: 2010-04-21
 *      Author: kharg
 */

#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>
#include "spiconfig.h"

void spi_init();

void spi_begin();

void spi_end();

uint8_t spi_read_write_byte(uint8_t byte);

void spi_read_data(uint8_t len, uint8_t* data);

void spi_write_data(uint8_t len, uint8_t* data);

#endif /* SPI_H_ */
