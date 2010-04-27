/**
 * spiconfig.h
 * 
 * @author Karol Maciaszek <karol.maciaszek@gmail.com>
 */

#ifndef SPICONFIG_H_
#define SPICONFIG_H_

// SPI frequency in kHz

#define SPI_FREQ		1000

// SPI port
#define SPI_PORT		B

// MISO pin (Master Input Slave Output)
#define SPI_MISO_PIN	4

// MOSI pin (Master Output Slave Input)
#define SPI_MOSI_PIN	5

// SCK pin (Clock)
#define SPI_SCK_PIN		6

// CSN pin (Chip Select)
#define SPI_CSN_PIN		7

#endif /* SPICONFIG_H_ */
