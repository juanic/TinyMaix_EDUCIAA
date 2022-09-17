/*
 * printf_retarget.c
 *
 *  Created on: 16 de mar. de 2021
 *      Author: Usuario
 */

#include "chip.h"
extern LPC_USART_T* UART;
void _putchar(char character)
{
//   write(1, &character, 1);
	while ((Chip_UART_ReadLineStatus(UART) & UART_LSR_THRE) == 0){}
	Chip_UART_Send(UART, &character, 1);

   //Delay(10);
}
