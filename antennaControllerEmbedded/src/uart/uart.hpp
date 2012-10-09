/*
 * uart.h
 *
 *  Created on: Jun 22, 2011
 *      Author: dinkem1
 */
#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>

void usartInit(uint16_t baud );
int  uart_putchar (char data, FILE *stream);
int  uart_getchar (FILE *stream);
void usartTransmit(uint8_t data);

#endif /* UART_H_ */
