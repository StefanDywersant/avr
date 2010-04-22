/*
 * spi.h
 *
 *  Created on: 2010-04-21
 *      Author: kharg
 */

#include <avr/io.h>

#ifndef SPI_H_
#define SPI_H_

#define SPI_MISO_PIN 0
#define SPI_SCK_PIN 1
#define SPI_MOSI_PIN 2
#define SPI_CSN_PIN 3

void spiInit();
void spiOff();
uint8_t spiReadWriteByte(uint8_t byte);

#endif /* SPI_H_ */
