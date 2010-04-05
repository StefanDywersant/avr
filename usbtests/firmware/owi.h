/*
 * 1wire.h
 *
 *  Created on: 2010-04-02
 *      Author: Karol Maciaszek
 */

#ifndef OWI_H_
#define OWI_H_

#define OWI_PORT	D
#define OWI_PIN		1

uint8_t OwiInit();

void OwiWriteByte(uint8_t byte);

uint8_t OwiReadByte();

#endif /* OWI_H_ */
