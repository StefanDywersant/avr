/*
 * nrf905.h
 *
 *  Created on: 2010-04-22
 *      Author: kharg
 */

#ifndef NRF905_H_
#define NRF905_H_

void nrf905_init();

uint32_t nrf905_get_address();

uint16_t nrf905_get_channel_no();

void nrf905_read_control_register();

#endif /* NRF905_H_ */
