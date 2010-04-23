/*
 * nrf905.h
 *
 *  Created on: 2010-04-22
 *      Author: kharg
 */

#ifndef NRF905_H_
#define NRF905_H_

void nrf905_init(void);

uint32_t nrf905_get_address(void);

uint16_t nrf905_get_channel_no(void);

void nrf905_set_channel_no(uint16_t no);

void nrf905_set_address(uint32_t addr);

void nrf905_read_control_register(uint8_t* buf);

#endif /* NRF905_H_ */
