/**
 *
 *
 * @author Karol Maciaszek <karol.maciaszek@gmail.com>
 * $Id$
 */


#ifndef USART_H_
#define USART_H_

void usart_init(void);

void usart_put_byte(char byte);

int usart_put_char(char chr, FILE* stream);

#endif /* USART_H_ */
