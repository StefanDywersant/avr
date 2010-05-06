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


#ifndef SPICONFIG_H_
#define SPICONFIG_H_

// SPI frequency in kHz

#define SPI_FREQ		1000

// SPI port
#define SPI_PORT		A

// MISO pin (Master Input Slave Output)
#define SPI_MISO_PIN	0

// MOSI pin (Master Output Slave Input)
#define SPI_MOSI_PIN	2

// SCK pin (Clock)
#define SPI_SCK_PIN		1

// CSN pin (Chip Select)
#define SPI_CSN_PIN		3

#endif /* SPICONFIG_H_ */
