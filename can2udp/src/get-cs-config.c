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

struct config_data {
    int deflated_stream_size;
    int deflated_size;
    int inflated_size;
    uint16_t crc;
    char *name;
    uint8_t *deflated_data;
    uint8_t *inflated_data;
};

uint16_t CRCCCITT(unsigned char *data, size_t length, unsigned short seed);

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
	ret = Z_DATA_ERROR;
    /* falls through */
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
    unsigned char netframe[FRAME_SIZE];
    unsigned char recvline[MAXSIZE];

    int ddi = 0;

    if (argc != 3) {
	fprintf(stderr, "usage:  %s <config> <IP address>\n", argv[0]);
	exit(EXIT_FAILURE);
    }

    if (strlen(argv[1]) > 7) {
	fprintf(stderr, "config name to long\n");
	exit(EXIT_FAILURE);
    } else {
	if ((config_data.name = malloc(strlen(argv[1] + 4)))) {
	    config_data.name[0] = '\0';
	    strcat(config_data.name, argv[1]);
	    strcat(config_data.name, ".cs2");
	} else {
	    fprintf(stderr, "can't malloc config %s.z file name\n", argv[1]);
	    exit(EXIT_FAILURE);
	}
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	fprintf(stderr, "can't create TCP socket: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    if (inet_aton((argv[2]), (struct in_addr *)&servaddr.sin_addr.s_addr) == 0) {
	fprintf(stderr, "invalid address: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    servaddr.sin_port = htons(15731);

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) {
	fprintf(stderr, "can't connect to TCP socket: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    memset(netframe, 0, FRAME_SIZE);
    memcpy(netframe, GETCONFIG, 5);
    memcpy(&netframe[5], argv[1], strlen(argv[1]));
    if (netframe_to_net(sockfd, netframe, FRAME_SIZE)) {
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
				    config_write(&config_data);
				    exit(EXIT_SUCCESS);
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
