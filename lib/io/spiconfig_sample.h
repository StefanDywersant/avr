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


#ifndef SPICONFIG_SAMPLE_H_
#define SPICONFIG_SAMPLE_H_


// SPI Serial Transfer Complete interrupt vector
#define SPI_STC_VECT	SPI_STC_vect

// SPI port
#define SPI_PORT		B

// MISO pin (Master Input Slave Output)
#define SPI_MISO_PIN	4

// MOSI pin (Master Output Slave Input)
#define SPI_MOSI_PIN	3

// SCK pin (Clock)
#define SPI_SCK_PIN	5

// CSN pin (Chip Select)
#define SPI_CSN_PIN	2


#endif /* SPICONFIG_SAMPLE_H_ */
