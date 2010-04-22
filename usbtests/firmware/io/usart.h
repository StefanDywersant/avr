/*
 * usart.h
 *
 *  Created on: 2010-04-09
 *      Author: kharg
 */

#ifndef USART_H_
#define USART_H_

void usartInit(uint32_t baud);

void usartPutByte(char byte);

int usartPutChar(char chr, FILE* stream);

#endif /* USART_H_ */
