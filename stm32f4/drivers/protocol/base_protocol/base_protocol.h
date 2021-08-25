#ifndef __BASE_PROTOCOL_H
#define __BASE_PROTOCOL_H

#include "stm32f2xx.h"
#include "autoconf.h"

/* upload command */
#define    RSP_CMD_OK                    (0xa0)
#define    RSP_DATA_OK                   (0xa1)
#define    RSP_CMD_ERR                   (0xa2)
#define    RSP_DATA_ERR                  (0xa3)
#define    RSP_CHIP_RSP_ERR              (0xa4)

#define     LEN_CMD               (1)
#define     LEN_LEN               (2)
#define     LEN_CRC               (1)
#define     LEN_ADDR              (1)

typedef struct cmd_packet {
	uint8_t cmd;
	uint16_t data_len;
	uint8_t *data;
	uint8_t crc;
}packet_t;

void packet_init(packet_t *packet, uint8_t cmd, uint8_t *buf, uint16_t len);
int8_t check_crc(packet_t *packet);

#endif
