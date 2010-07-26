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


#ifndef HWSPI_H_
#define HWSPI_H_


void hwspi_init_slave(uint8_t (*func)(uint8_t));

void hwspi_init_master(void);

void hwspi_begin(void);

void hwspi_end(void);

uint8_t hwspi_read_write_byte(uint8_t wr);

void hwspi_read_data(uint8_t len, uint8_t* data);

void hwspi_write_data(uint8_t len, uint8_t* data);


#endif /* HWSPI_H_ */
