#include "usart.h"
#include "stdio.h"
#include  <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include "include.h"
#include "delay.h"
#include "autoconf.h"


u32 rx_count = 0;
u8 usart_rx_buf[USART_REC_LEN];

s8 string[256];//调试信息缓冲，输出调试信息一次不可以大于256


//标准库需要的支持函数
struct __FILE
{
	int handle;

};

FILE __stdout;
int __io_putchar(int ch)
{
#if defined(CONFIG_SLAVE_FIRMWARE) && defined(CONFIG_NEW_BOARD)
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕
	USART1->DR = (u8) ch;
	return ch;
#elif (defined(CONFIG_MASTER_FIRMWARE) && defined(CONFIG_NEW_BOARD))
	while((UART4->SR&0X40)==0);//循环发送,直到发送完毕
	UART4->DR = (u8) ch;
	return ch;
#else
	while((UART5->SR&0X40)==0);//循环发送,直到发送完毕
	UART5->DR = (u8) ch;
	return ch;
#endif
}
int8_t uart_init(USART_TypeDef* USARTx, u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	if(USARTx == USART1)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  		GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
  		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  		GPIO_Init(GPIOA, &GPIO_InitStructure);

  		GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
  		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

  		//NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
		//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		//NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		//NVIC_Init(&NVIC_InitStructure);
  		//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);


		USART_InitStructure.USART_BaudRate = bound;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

  		USART_Init(USART1, &USART_InitStructure);
 		USART_Cmd(USART1, ENABLE);
		return RET_OK;
	} else if(USARTx == UART5) {
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);

  		GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
  		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  		GPIO_Init(GPIOD, &GPIO_InitStructure);

  		GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
  		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  		GPIO_Init(GPIOC, &GPIO_InitStructure);

		GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);

  		//NVIC_InitStructure.NVIC_IRQChannel = USART5_IRQn;
		//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
		//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		//NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		//NVIC_Init(&NVIC_InitStructure);


		USART_InitStructure.USART_BaudRate = bound;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

  		USART_Init(UART5, &USART_InitStructure);
  		//USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
 		USART_Cmd(UART5, ENABLE);
		return RET_OK;
	} else if(USARTx == UART4) {
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

  		GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
  		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  		GPIO_Init(GPIOC, &GPIO_InitStructure);

  		GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;
  		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  		GPIO_Init(GPIOC, &GPIO_InitStructure);

		GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);

		USART_InitStructure.USART_BaudRate = bound;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

  		USART_Init(UART4, &USART_InitStructure);
 		USART_Cmd(UART4, ENABLE);
		return RET_OK;
	} else {
		return RET_ERR;
	}
}

void USART1_IRQHandler(void)
{
	int Res;
	if (USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == SET) {
		Res = USART_ReceiveData(USART1);
		usart_rx_buf[rx_count++] = Res;
		//printf("\r\ndata:%d count %d \r\n", Res, rx_count);
	}
}

u32 mcu_uart_write (USART_TypeDef* USARTx, u8 *buf, s32 len)
{
    u32 t;
    u16 ch;

    if (len <= 0)
        return(-1);

    if(buf == NULL)
        return(-1);

    while(len != 0)
    {
        // 等待串口发送完毕
        t = 0;
       // while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
    	while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)
        {
            if(t++ >= 0x1000000) {
				printf("time out\r\n");
                return(-1);
			}
        }
        ch = (u16)(*buf & 0xff);
        USART_SendData(USARTx, ch);
        buf++;
        len--;
    }
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET) {
        if(t++ >= 0x1000000) {
			printf("time out\r\n");
            return(-1);
		}
	}

    return(0);
}

void cmd_uart_printf(char *fmt,...)
{
    s32 length = 0;
    va_list ap;

    s8 *pt;

    va_start(ap,fmt);
    vsprintf((char *)string,(const char *)fmt,ap);
    pt = &string[0];
    while(*pt!='\0')
    {
        length++;
        pt++;
    }

    mcu_uart_write(USART1, (u8*)&string[0], length);  //写串口

    va_end(ap);
}
s32 mcu_uart_read (USART_TypeDef* USARTx, u8 *buf, s32 len)
{
    s32 i;
	int Res;

	if (len > 1) return(-1);
    if(len <= 0) return(-1);
    if(buf == NULL) return(-1);
	if (rx_count>0) {
		*buf = usart_rx_buf[rx_count-1];
		rx_count--;
		return 1;
	} else {
  		return 0;
	}
}

int8_t uart_receive(USART_TypeDef* USARTx, uint32_t time_count, uint8_t *buf, uint32_t nbyte)
{
	int8_t ret;
	uint32_t bytes = nbyte;
	uint32_t count = time_count;
	if (time_count == 0)
		count = 1;

	while (--count) {
		if (USART_GetFlagStatus(USARTx,USART_FLAG_RXNE) == SET) {
			*buf++ = (uint8_t)USART_ReceiveData(USARTx);
			count = time_count;
			bytes--;
		} else {
			delay_us(1);
		}
		if (!bytes) {
			break;
		}
	}
	if (!count) {
		//while (bytes!=nbyte) {
		//	printf("%d\r\n", *(--buf));
		//	bytes++;
		//}
		//printf("%d\r\n", bytes);
		return RET_ERR;
	} else {
		return RET_OK;
	}
}
