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


#ifndef SOFTSPICONFIG_H_
#define SOFTSPICONFIG_H_

// SPI frequency in kHz

#define SOFTSPI_FREQ		1000

// SPI port
#define SOFTSPI_PORT		A

// MISO pin (Master Input Slave Output)
#define SOFTSPI_MISO_PIN	0

// MOSI pin (Master Output Slave Input)
#define SOFTSPI_MOSI_PIN	2

// SCK pin (Clock)
#define SOFTSPI_SCK_PIN		1

// CSN pin (Chip Select)
#define SOFTSPI_CSN_PIN		3

#endif /* SOFTSPICONFIG_H_ */
