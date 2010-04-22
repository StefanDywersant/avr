/*
 * spi.h
 *
 *  Created on: 2010-04-21
 *      Author: kharg
 */

#include <avr/io.h>

#ifndef SPI_H_
#define SPI_H_

#define SPI_PORT	A
#define SPI_MISO_PIN	0
#define SPI_SCK_PIN	1
#define SPI_MOSI_PIN	2
#define SPI_CSN_PIN	3

void spiInit();

void spiBegin();

void spiEnd();

uint8_t spiReadWriteByte(uint8_t byte);

void spiReadData(uint8_t len, uint8_t* data);

void spiWriteData(uint8_t len, uint8_t* data);

#endif /* SPI_H_ */
