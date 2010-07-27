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


void spi_init_slave(uint8_t (*func)(uint8_t));

void spi_init_master(void);

void spi_begin(void);

void spi_end(void);

uint8_t spi_read_write_byte(uint8_t wr);

void spi_read_data(uint8_t len, uint8_t* data);

void spi_write_data(uint8_t len, uint8_t* data);


#endif /* HWSPI_H_ */
