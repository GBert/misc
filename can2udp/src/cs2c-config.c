/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <assert.h>

#include <zlib.h>

#define FRAME_SIZE	13
#define MAXSIZE		16384

unsigned char GETCONFIG[]	   = { 0x00, 0x40, 0x03, 0x00, 0x08 };
unsigned char GETCONFIG_DATA[]	   = { 0x00, 0x42, 0x03, 0x00, 0x08 };
unsigned char GETCONFIG_RESPONSE[] = { 0x00, 0x42, 0x03, 0x00, 0x06 };

unsigned char ADDON1[] = {0x00, 0x31, 0x63, 0x4A, 0x08, 0x00, 0x00, 0x00, 0x00, 0x04, 0x02, 0xFF, 0xF0};
unsigned char ADDON2[] = {0x00, 0x00, 0x03, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x01, 0x00, 0x00};

char CONFIG1[] = "gbs";
char CONFIG2[] = "fs";
char CONFIG3[] = "loks";

struct config_data {
    int deflated_stream_size;
    int deflated_size;
    int inflated_size;
    uint16_t crc;
    char *name;
    uint8_t *deflated_data;
    uint8_t *inflated_data;
};

static uint16_t crc_table[256] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
    0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
    0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
    0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
    0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
    0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
    0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
    0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
    0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
    0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
    0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
    0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
    0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
    0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
    0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
    0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
    0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
    0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
    0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
    0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
    0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
    0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};

/* Maerklin uses CCIT CRC, which is polynom 0x1021 -> x^16 + x^12 +x^5 + 1 */
uint16_t CRCCCITT(unsigned char *data, size_t length, unsigned short seed)
{

    size_t count;
    uint16_t crc = seed;

    for (count = 0; count < length; ++count) {
        uint16_t temp = (*data++ ^ (crc >> 8)) & 0xff;
        crc = crc_table[temp] ^ (crc << 8);
    }

    return crc;
}

int netframe_to_net(int net_socket, unsigned char *netframe, int length) {
    if (send(net_socket, netframe, length, 0) != length)
	return 1;
    return 0;
}

int inflate_data(struct config_data *config_data) {
    int ret;
    z_stream strm;

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK)
	return ret;
    strm.avail_in = config_data->deflated_size;
    strm.avail_out = config_data->inflated_size;
    strm.next_in = config_data->deflated_data + 4;
    strm.next_out = config_data->inflated_data;
    ret = inflate(&strm, Z_NO_FLUSH);

    assert(ret != Z_STREAM_ERROR);	/* state not clobbered */
    switch (ret) {
    case Z_NEED_DICT:
	ret = Z_DATA_ERROR;	/* and fall through */
    case Z_DATA_ERROR:
    case Z_MEM_ERROR:
	(void)inflateEnd(&strm);
	return ret;
    }

    return 0;
}

int config_write(struct config_data *config_data) {
    FILE *config_fp;
    uint16_t crc;
    int i;

    crc = CRCCCITT(config_data->deflated_data, config_data->deflated_stream_size, 0xFFFF);

    printf("\n  writing to %s - size 0x%04x crc 0x%04x 0x%04x\n", config_data->name, config_data->deflated_stream_size,
	   config_data->crc, crc);

    config_fp = fopen(config_data->name, "wb");
    if (!config_fp) {
	fprintf(stderr, "\ncan't open file %s for writing - error: %s\n", config_data->name, strerror(errno));
	exit(EXIT_FAILURE);
    } else {
	for (i = 0; i < config_data->deflated_stream_size; i++) {
	    if ((i % 8) == 0)
		printf("\n");
	    printf("%02x ", config_data->deflated_data[i]);
	}
	printf("\n");
    }
    inflate_data(config_data);
    fwrite(config_data->inflated_data, 1, config_data->inflated_size, config_fp);
    fclose(config_fp);
    return 1;
}

int main(int argc, char **argv) {
    int i, sockfd, tcp_packet_nr, n = 1;
    int temp, config_data_start, config_data_stream, deflated_size = 0;
    struct config_data config_data;
    struct sockaddr_in servaddr;
    fd_set rset;
    unsigned char netframe[FRAME_SIZE*5];
    unsigned char recvline[MAXSIZE];

    int ddi = 0;

    if (argc != 2) {
	fprintf(stderr, "usage:  %s <IP address>\n", argv[0]);
	exit(EXIT_FAILURE);
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	fprintf(stderr, "can't create TCP socket: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    if (inet_aton((argv[1]), (struct in_addr *)&servaddr.sin_addr.s_addr) == 0) {
	fprintf(stderr, "invalid address: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    servaddr.sin_port = htons(15731);

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) {
	fprintf(stderr, "can't connect to TCP socket: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    memset(netframe, 0, FRAME_SIZE*5);
    memcpy(netframe, GETCONFIG, 5);
    memcpy(&netframe[13], GETCONFIG, 5);
    memcpy(&netframe[26], GETCONFIG, 5);
    memcpy(&netframe[5],  CONFIG1, sizeof(CONFIG1));
    memcpy(&netframe[18], CONFIG2, sizeof(CONFIG2));
    memcpy(&netframe[31], CONFIG3, sizeof(CONFIG3));
    memcpy(&netframe[39], ADDON1, sizeof(ADDON1));
    memcpy(&netframe[52], ADDON2, sizeof(ADDON2));

    if (netframe_to_net(sockfd, netframe, FRAME_SIZE*5)) {
	fprintf(stderr, "can't send data on TCP socket: %st\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    FD_ZERO(&rset);
    tcp_packet_nr = 0;
    config_data_start = 0;
    config_data_stream = 0;

    for (;;) {
	FD_SET(sockfd, &rset);

	if (select(sockfd + 1, &rset, NULL, NULL, NULL) < 0) {
	    fprintf(stderr, "socket error: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
	tcp_packet_nr++;
	if (FD_ISSET(sockfd, &rset)) {
	    if ((n = recv(sockfd, recvline, MAXSIZE, 0)) > 0) {
		if (memcmp(recvline, GETCONFIG_RESPONSE, 5) == 0) {
		    memcpy(&temp, &recvline[5], 4);
		    deflated_size = ntohl(temp);
		    config_data.deflated_size = deflated_size;
		    memcpy(&temp, &recvline[9], 2);
		    config_data.crc = ntohs(temp);
		    printf("\nstart of config - deflated size: 0x%08x crc 0x%04x", deflated_size, config_data.crc);
		    config_data_start = 1;
		    /* we alloc 8 bytes more to be sure that it fits */
		    config_data.deflated_data = malloc(deflated_size + 16);
		    if (config_data.deflated_data == NULL) {
			fprintf(stderr, "can't malloc deflated config data buffer - size 0x%04x\n", deflated_size + 8);
			exit(EXIT_FAILURE);
		    }
		    /* deflated data index */
		    ddi = 0;
		}
		for (i = 0; i < n; i++) {
		    if ((i % FRAME_SIZE) == 0) {
			if (memcmp(&recvline[i], GETCONFIG_DATA, 5) == 0) {
			    memcpy(&config_data.deflated_data[ddi], &recvline[i + 5], 8);
			    ddi += 8;
			    if (config_data_start) {
				memcpy(&temp, &recvline[i + 5], 4);
				config_data.inflated_size = ntohl(temp);
				printf("\ninflated size: 0x%08x", config_data.inflated_size);
				config_data.inflated_data = malloc(config_data.inflated_size);
				if (config_data.inflated_data == NULL) {
				    fprintf(stderr, "can't malloc inflated config data buffer - size 0x%04x\n",
					    config_data.inflated_size);
				    exit(EXIT_FAILURE);
				}
				config_data_start = 0;
				config_data_stream = 1;
				deflated_size -= 8;
			    } else if (config_data_stream) {
				if (deflated_size <= 8) {
				    config_data_stream = 0;
				    config_data.deflated_stream_size = ddi;
				    // config_write(&config_data);
				    // exit(EXIT_SUCCESS);
				} else {
				    deflated_size -= 8;
				}
			    }
			}
			printf("\n %04d: ", tcp_packet_nr);
		    }
		    printf("%02x ", recvline[i]);
		}
		printf("\n");
	    }
	}
    }
    return 1;
}
