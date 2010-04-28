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


#ifndef USART_H_
#define USART_H_

void usart_init(void);

void usart_put_byte(char byte);

int usart_put_char(char chr, FILE* stream);

#endif /* USART_H_ */
