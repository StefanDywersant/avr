/*
 * nrf905.h
 *
 *  Created on: 2010-04-22
 *      Author: kharg
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

void nrf905_get_tx_payload(uint8_t* payload);

void nrf905_get_rx_payload(uint8_t* payload);

void nrf905_read_control_register(uint8_t* buf);

#endif /* NRF905_H_ */
