#ifndef _UART__PROTOCOL_H
#define _UART__PROTOCOL_H

#include "stm32f2xx.h"
#include "autoconf.h"
#include "base_protocol.h"

int8_t protocol_init(USART_TypeDef* USARTx, uint32_t bound);
int8_t send_packet(packet_t *packet);
int8_t receive_packet(packet_t *packet, uint32_t wait_time, uint32_t receive_time);
#endif
