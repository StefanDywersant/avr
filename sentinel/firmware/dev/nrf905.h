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


#ifndef NRF905_H_
#define NRF905_H_

void nrf905_init(void);

uint32_t nrf905_get_rx_address(void);

uint32_t nrf905_get_tx_address(void);

void nrf905_set_rx_address(uint32_t addr);

uint16_t nrf905_get_channel_no(void);

void nrf905_set_channel_no(uint16_t no);

void nrf905_tx_packet(uint32_t addr, uint8_t len, uint8_t* payload);

void nrf905_rx_packet(uint8_t len, uint8_t* payload);

void nrf905_get_tx_payload(uint8_t* payload);

uint8_t nrf905_get_status_register(void);

void nrf905_enable_rx(void);

void nrf905_disable_rx(void);

void nrf905_read_control_register(uint8_t* buf);

void nrf905_set_packet_rx_callback(void (*func)(void));

void nrf905_set_packet_tx_callback(void (*func)(void));


#endif /* NRF905_H_ */
