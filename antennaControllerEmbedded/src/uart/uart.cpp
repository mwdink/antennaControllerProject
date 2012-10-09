/*
 * uart.cpp
 *
 *  Created on: Jun 22, 2011
 *      Author: dinkem1
 */
/*----------------------------- Nested includes -----------------------------*/
#include "uart.hpp"

/*----------------- Symbolic Constants and Macros (defines) -----------------*/
/*-------------------------- Typedefs and structs ---------------------------*/
/*----------------------- Declarations (externs only) -----------------------*/
/*------------------------------ Declarations -------------------------------*/
/*--------------------------- Function prototypes ---------------------------*/


/*!Function     usartInit
*   \param      uint16_t baud
*   \return     void
*   \par Purpose
*               initialize the USART
*   \note
*/
void
usartInit(uint16_t baud)
{
    UCSR0A = (1 << U2X0);
    baud = (F_CPU / 4 / baud - 1) / 2;

    /*Set baud rate */
    UBRR0H = (unsigned char)(baud>>8);
    UBRR0L = (unsigned char)baud;

    /* Enable receiver and transmitter */
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);

    /* Set frame format: 8data, 1stop bit */
    UCSR0C = (3<<UCSZ00);
}

/*!Function     uart_putchar
*   \param      char data
*   \param      FILE *stream
*   \return     int
*   \par Purpose
*               send a char out the UART
*   \note
*/
int
uart_putchar (char data, FILE *stream)
{
    if (data == '\n')
    {
        uart_putchar('\r', stream);
    }

    /* Wait for empty transmit buffer */
    while (!( UCSR0A & (1<<UDRE0)));

    /* Put our character into the transmit buffer */
    UDR0 = data;

    return 0;
}

/*!Function     uart_getchar
*   \param      FILE *stream
*   \return     int
*   \par Purpose
*               not implemented - get a char from the UART
*   \note
*/
int
uart_getchar (FILE *stream)
{
    return 0;
}


/*!Function     usartTransmit
*   \param      char data
*   \return     int
*   \par Purpose
*               send a char out the UART
*   \note
*/
void
usartTransmit(uint8_t data)
{
    /* Wait for empty transmit buffer */
    while ( !( UCSR0A & (1<<UDRE0)));

    /* Put data into buffer, sends the data */
    UDR0 = data;
}
