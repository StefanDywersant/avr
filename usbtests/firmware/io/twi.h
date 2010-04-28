/**
 *
 *
 * @author Karol Maciaszek <karol.maciaszek@gmail.com>
 * $Id$
 */


#ifndef TWI_H_
#define TWI_H_

#include <stdint.h>
#include <util/twi.h>
#include "twiconfig.h"

#define TWI_OK 0x0
#define TWI_ERROR 0x1

void twi_init(void);

uint8_t twi_sync_mt(uint8_t address, uint8_t *req_data, uint8_t req_len);

uint8_t twi_sync_mtmr(uint8_t address, uint8_t *req_data, uint8_t req_len, uint8_t *res_data, uint8_t res_len);

#endif /* TWI_H_ */
;
