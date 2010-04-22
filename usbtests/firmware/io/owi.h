/*
 * 1wire.h
 *
 *  Created on: 2010-04-02
 *      Author: Karol Maciaszek
 */

#ifndef OWI_H_
#define OWI_H_

#define OWI_PORT	D
#define OWI_PIN		3

uint8_t OwiInit();

void OwiWriteByte(uint8_t byte);

uint8_t OwiReadByte();

uint8_t OwiGetNextROM(uint8_t* prevROM, uint8_t* nextROM, uint8_t *lastDiff);

#endif /* OWI_H_ */
