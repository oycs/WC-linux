#ifndef __USART_H__
#define __USART_H__

#include "stm32f4xx.h"

#define USART_REC_LEN (12*1024)
#define PC_PORT UART5
int8_t uart_init(USART_TypeDef* USARTx, u32 bound);
void cmd_uart_printf(char *fmt,...);
u32 mcu_uart_write (USART_TypeDef* USARTx, u8 *buf, s32 len);
s32 mcu_uart_read (USART_TypeDef* USARTx, u8 *buf, s32 len);
int8_t uart_receive(USART_TypeDef* USARTx, uint32_t time_count, uint8_t *buf, uint32_t nbyte);
#endif
