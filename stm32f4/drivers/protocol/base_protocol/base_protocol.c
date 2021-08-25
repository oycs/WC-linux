#include "stdio.h"
#include "usart.h"
#include "base_protocol.h"
#include "include.h"

static uint16_t crc8_gen(const uint8_t *ptr, uint32_t len, uint16_t old_crc)
{
	const uint8_t *data = ptr;
	uint16_t crc = old_crc;
	uint32_t i, j;

	for (j = len; j; j--, data++) {
		crc ^= (*data << 8 );
		for (i = 8; i; i--) {
			if (crc & 0x8000)
				crc ^= (0x1070 << 3);
			crc <<= 1;
		}
	}
	return crc;
}

static void caculate_crc(packet_t *packet)
{
	uint16_t crc = 0;
	uint8_t data_len[2];

	crc = crc8_gen(&(packet->cmd), LEN_CMD, 0);
	data_len[0] = (packet->data_len >> 8);
	data_len[1] = packet->data_len;
	crc = crc8_gen(data_len, LEN_LEN, crc);
	crc = crc8_gen(packet->data, packet->data_len, crc);
	packet->crc = (uint8_t)(crc >> 8);
}

int8_t check_crc(packet_t *packet)
{
	uint16_t crc;
	uint8_t data_len[2];

	crc = crc8_gen(&(packet->cmd), LEN_CMD, 0);
	data_len[0] = (packet->data_len >> 8);
	data_len[1] = packet->data_len;
	crc = crc8_gen(data_len, LEN_LEN, crc);
	crc = crc8_gen(packet->data, packet->data_len, crc);
	//printf("cal_crc = 0x%x\r\n", crc);
	crc = (uint8_t)(crc >> 8);
	if (packet->crc == crc) {
		return RET_OK;
	} else {
		printf("cmd = %d\r\n", packet->cmd);
		printf("len[0] = 0x%x\r\n", data_len[0]);
		printf("len[1] = 0x%x\r\n", data_len[1]);
		printf("packet->crc = 0x%x\r\n", packet->crc);
		printf("cal_crc = 0x%x\r\n", crc);
		printf("%s +%d\r\n",__FILE__, __LINE__);
		return RET_ERR;
	}
}

void packet_init(packet_t *packet, uint8_t cmd, uint8_t *buf, uint16_t len)
{
	packet->cmd = cmd;
	packet->data_len = len;
	packet->data = buf;
	caculate_crc(packet);
}
