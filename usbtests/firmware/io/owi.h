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


#ifndef OWI_H_
#define OWI_H_

#include "owiconfig.h"

uint8_t owi_init(void);

void owi_write_byte(uint8_t byte);

uint8_t owi_read_byte(void);

uint8_t owi_get_next_rom(uint8_t* rom, uint8_t *last_diff);

#endif /* OWI_H_ */
