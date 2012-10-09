/*
 * stdioFixup.cpp
 *
 *  Created on: Jun 14, 2011
 *      Author: dinkem1
 *
 * 	Add support for printf
 */
#include "stdioFixup.hpp"
#include "uart.hpp"

FILE *uart_str;


/*!Function     setupStdIo
*   \param      none
*   \return     void
*   \par Purpose
*   			attach USART to stdio
*   \note
*/
void
setupStdIo(void)
{
	uart_str = fdevopen(uart_putchar, uart_getchar);
	stdout = stdin = uart_str;
}


