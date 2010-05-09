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

#ifndef RC2_H_
#define RC2_H_

void rc2_set_key(uint8_t* k);

void rc2_encrypt(uint8_t len, uint8_t* payload);

void rc2_decrypt(uint8_t len, uint8_t* payload);

#endif /* RC2_H_ */
