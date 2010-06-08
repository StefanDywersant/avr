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


#ifndef RFPROTO_H_
#define RFPROTO_H_

#include <stdint.h>

void rfproto_async_tx(uint8_t len, uint8_t* msg, uint8_t type);

uint8_t rfproto_rx(uint8_t len, uint8_t* msg, uint8_t* type);

#endif /* RFPROTO_H_ */
