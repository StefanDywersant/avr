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


#ifndef NRF905CONFIG_H_
#define NRF905CONFIG_H_

#include <avr/interrupt.h>

// PIN configuration
#define PORT				B
#define TX_EN_PIN			0
#define TRX_CE_PIN			3
#define PWR_UP_PIN			1
#define DR_PIN				2

// Data Ready Interrupt configuration
#define DR_SETUP()			{ \
								GICR |= 0x01 << INT2; \
								MCUCSR |= 0x01 << ISC2; \
								SREG |= 0x01 << SREG_I; \
							}
#define DR_INT_VECT			INT2_vect // DR interrupt vector name

// Sets center frequency
#define CH_NO				0x0100

// Sets PLL in 433 or 868/915MHz mode.
#define HFREQ_PLL_OFF		0x00 << 1 // 433MHz
#define HFREQ_PLL_ON		0x01 << 1 // 868/915MHz
#define HFREQ_PLL			HFREQ_PLL_OFF

// Output power
#define PA_PWR_M_10			0x00 << 2 // -10dBm
#define PA_PWR_M_2			0x01 << 2 // -2dBm
#define PA_PWR_P_6			0x02 << 2 // 6dBm
#define PA_PWR_P_10			0x03 << 2 // 10dBm
#define PA_PWR				PA_PWR_P_10

// Reduces current in RX mode by 1.6mA. Sensitivity is reduced.
#define RX_RED_PWR_OFF		0x00 << 4 // normal operation
#define RX_RED_PWR_ON		0x01 << 4 // reduced power
#define RX_RED_PWR			RX_RED_PWR_OFF

// Retransmit contents in TX register if TRX_CE and TXEN are high.
#define AUTO_RETRAN_OFF		0x00 << 5 // no retransmission
#define AUTO_RETRAN_ON		0x01 << 5 // retransmission
#define AUTO_RETRAN			AUTO_RETRAN_OFF

// RX-address width
#define RX_AFW_1B			0x01 // 1 byte
#define RX_AFW_2B			0x02 // 2 byte
#define RX_AFW_3B			0x03 // 3 byte
#define RX_AFW_4B			0x04 // 4 byte
#define RX_AFW				RX_AFW_4B

// TX-address width
#define TX_AFW_1B			0x01 << 4 // 1 byte
#define TX_AFW_2B			0x02 << 4 // 2 byte
#define TX_AFW_3B			0x03 << 4 // 3 byte
#define TX_AFW_4B			0x04 << 4// 4 byte
#define TX_AFW				TX_AFW_4B

// RX-payload width
#define RX_PW				0x20

// TX-payload width
#define TX_PW				0x20

// RX address
#define RX_ADDRESS			0xabababab

// Output clock frequency
#define UP_CLK_FREQ_4		0x00 // 4MHz
#define UP_CLK_FREQ_2		0x01 // 2MHz
#define UP_CLK_FREQ_1		0x02 // 1MHz
#define UP_CLK_FREQ_500		0x03 // 500kHz
#define UP_CLK_FREQ			UP_CLK_FREQ_500

// Output clock enable
#define UP_CLK_EN_OFF		0x00 << 2
#define UP_CLK_EN_ON		0x01 << 2
#define UP_CLK_EN			UP_CLK_EN_OFF

// Crystal oscillator frequency
#define XOF_4				0x00 << 3 // 4MHz
#define XOF_8				0x01 << 3 // 8MHz
#define XOF_12				0x02 << 3 // 12MHz
#define XOF_16				0x03 << 3 // 16MHz
#define XOF_20				0x04 << 3 // 20MHz
#define XOF					XOF_16

// CRC check enable
#define CRC_EN_OFF			0x00 << 6
#define CRC_EN_ON			0x01 << 6
#define CRC_EN				CRC_EN_ON

// CRC mode
#define CRC_MODE_8			0x00 << 7 // 8 bit CRC mode
#define CRC_MODE_16			0x01 << 7 // 16 bit CRC mode
#define CRC_MODE			CRC_MODE_16


#endif /* NRF905CONFIG_H_ */
