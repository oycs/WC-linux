#include "stdio.h"
#include "usart.h"
#include "base_protocol.h"
#include "include.h"
#include "uart_protocol.h"

USART_TypeDef* protocol_uart;

uint8_t new_protocol = 0;
uint32_t max_rec_data_len = 100;
int8_t send_packet(packet_t *packet)
{
	uint8_t data_len[2];
	int8_t ret;

	char head = 'H';
	ret = mcu_uart_write(protocol_uart, (uint8_t *)&head, 1);
	if (ret != RET_OK) {
		return RET_ERR;
	}

	ret = mcu_uart_write(protocol_uart, &(packet->cmd), LEN_CMD);
	if (ret != RET_OK) {
		return RET_ERR;
	}

	data_len[0] = (packet->data_len >> 8);
	data_len[1] = packet->data_len;
	ret = mcu_uart_write(protocol_uart, data_len, LEN_LEN);
	if (ret != RET_OK) {
		return RET_ERR;
	}

	if (packet->data_len) {
		ret = mcu_uart_write(protocol_uart, packet->data, packet->data_len);
		if (ret != RET_OK) {
			return RET_ERR;
		}
	}

	ret = mcu_uart_write(protocol_uart, &(packet->crc), LEN_CRC);
	if (ret != RET_OK) {
		return RET_ERR;
	}

	return RET_OK;
}

int8_t receive_packet(packet_t *packet, uint32_t wait_time, uint32_t receive_time)
{
	int8_t ret;
	uint8_t data_len[2];
	//GPIO_ResetBits(GPIOB,GPIO_Pin_15|GPIO_Pin_14|GPIO_Pin_13|GPIO_Pin_12);

	uint8_t head;

	ret = uart_receive(protocol_uart, receive_time, &head, 1);
	if (ret != RET_OK) {
		//printf("%s +%d\r\n", __FILE__, __LINE__);
		return RET_ERR;
	}
	if (((char)head) != 'H') {
		//printf("%s +%d\r\n", __FILE__, __LINE__);
		return RET_ERR;
	}

	ret = uart_receive(protocol_uart, receive_time, &(packet->cmd), LEN_CMD);
	if (ret != RET_OK) {
		//printf("%s +%d\r\n", __FILE__, __LINE__);
		return RET_ERR;
	}

#ifdef CONFIG_SLAVE_FIRMWARE
#else
	if (packet->cmd == 0x0) {
		new_protocol = 1;
		ret = uart_receive(protocol_uart, receive_time, &(packet->cmd), LEN_CMD);
		if (ret != RET_OK) {
			printf("%s +%d\r\n", __FILE__, __LINE__);
			return RET_ERR;
		}
	} else {
		new_protocol = 0;
	}
#endif

	ret = uart_receive(protocol_uart, receive_time, data_len, LEN_LEN);
	if (ret != RET_OK) {
		printf("%s +%d\r\n", __FILE__, __LINE__);
		return RET_ERR;
	}

	packet->data_len = (data_len[0] << 8) | data_len[1];
	if (packet->data_len > max_rec_data_len) {
		return RET_ERR;
	}
	ret = uart_receive(protocol_uart, receive_time, packet->data, packet->data_len);
	if (ret != RET_OK) {
		//printf("cmd = 0x%x\r\n",packet->cmd);
		//printf("data_len0 = 0x%x\r\n",data_len[0]);
		//printf("data_len1 = 0x%x\r\n",data_len[1]);
		//printf("%s +%d\r\n", __FILE__, __LINE__);
		return RET_ERR;
	}

	ret = uart_receive(protocol_uart, receive_time, &(packet->crc), LEN_CRC);
	if (ret != RET_OK) {
		printf("%s +%d\r\n", __FILE__, __LINE__);
		return RET_ERR;
	}

	ret = check_crc(packet);
	//GPIO_SetBits(GPIOB,GPIO_Pin_15|GPIO_Pin_14|GPIO_Pin_13|GPIO_Pin_12);
	return ret;
}

int8_t protocol_init(USART_TypeDef* USARTx, uint32_t bound)
{
	protocol_uart = USARTx;
	if (uart_init(protocol_uart, bound))
		return RET_ERR;
	else
		return RET_OK;
}
