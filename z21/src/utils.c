/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return.
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

/*
 * Z21 Emulation for Roco WiFi Mouse
 */

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <net/if.h>
#include <netinet/in.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <linux/can.h>
#include <zlib.h>

#include "utils.h"
#include "z21.h"

#define MAX(a,b)		((a) > (b) ? (a) : (b))
#define MAXIPLEN		40	/* maximum IP string length */
#define PRIMARY_UDP_PORT	21105
#define SECONDARY_UDP_PORT	21106
#define MAERKLIN_PORT		15731
#define MAXSIZE			16384

#define MAXDG   256		/* maximum datagram size */

uint16_t CRCCCITT(unsigned char *data, size_t length, unsigned short seed);

extern struct z21_data_t z21_data;

uint16_t le16(uint8_t * u) {
    return (u[1] << 8) | u[0];
}

uint16_t be16(uint8_t * u) {
    return (u[0] << 8) | u[1];
}

uint32_t be32(uint8_t * u) {
    return (u[0] << 24) | (u[1] << 16) | (u[2] << 8) | u[3];
}

uint32_t le32(uint8_t * u) {
    return (u[3] << 24) | (u[2] << 16) | (u[1] << 8) | u[0];
}

void usec_sleep(int usec) {
    struct timespec to_wait;

    if (usec > 999999)
	usec = 999999;
    to_wait.tv_sec = 0;
    to_wait.tv_nsec = usec * 1000;
    nanosleep(&to_wait, NULL);
}

int time_stamp(char *timestamp) {
    struct timeval tv;
    struct tm *tm;

    gettimeofday(&tv, NULL);
    tm = localtime(&tv.tv_sec);

    sprintf(timestamp, "%02d:%02d:%02d.%03d", tm->tm_hour, tm->tm_min, tm->tm_sec, (int)tv.tv_usec / 1000);
    return 0;
}

uint8_t xor(unsigned char *data, int length) {
    uint8_t res;
    int i;

    res = 0;
    for (i = 0; i < length; i++)
	res ^= data[i];
    return (res);
}

void print_udp_frame(char *format, int udplength, unsigned char *udpframe) {
    int i;
    uint16_t length, header;
    char timestamp[16];

    time_stamp(timestamp);
    printf("%s ", timestamp);
    if (z21_data.foreground) {
	/* print timestamp */

	if (udplength >= 4) {
	    length = le16(&udpframe[0]);
	    header = le16(&udpframe[2]);
	    printf(format, length, header);
	    for (i = 4; i < udplength; i++)
		printf(" %02x", udpframe[i]);
	    /* printf("\n"); */
	    for (; i < 16; i++)
		printf("   ");
	}
    }
}

void print_net_frame(char *format, unsigned char *netframe, int verbose) {
    uint32_t canid;
    int i, dlc;
    char timestamp[16];

    if (!verbose)
	return;

    canid = be32(netframe);
    dlc = netframe[4];
    time_stamp(timestamp);
    printf("%s ", timestamp);
    printf(format, canid & CAN_EFF_MASK, netframe[4]);
    for (i = 5; i < 5 + dlc; i++) {
        printf(" %02x", netframe[i]);
    }
    if (dlc < 8) {
        printf("(%02x", netframe[i]);
        for (i = 6 + dlc; i < 13; i++) {
            printf(" %02x", netframe[i]);
        }
        printf(")");
    } else {
        printf(" ");
    }
    printf("  ");
    for (i = 5; i < 13; i++) {
        if (isprint(netframe[i]))
            printf("%c", netframe[i]);
        else
            putchar(46);
    }
    printf(" ");
}

struct node *insert_right(struct node *list, int id) {
    struct node *new_node = (struct node *)calloc(sizeof(struct node), 1);
    new_node->id = id;
    new_node->next = list->next;
    list->next = new_node;
    return new_node;
}

void free_list(struct node *list) {
    struct node *p, *next_node;
    for (p = list->next; p != NULL; p = next_node) {
	next_node = p->next;
	free(p);
    }
    list->next = NULL;
}

struct node *search_node(struct node *list, int id) {
    while (list != NULL) {
	if (list->id == id)
	    return list;
	list = list->next;
    }
    return NULL;
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

    assert(ret != Z_STREAM_ERROR);      /* state not clobbered */
    switch (ret) {
    case Z_NEED_DICT:
	ret = Z_DATA_ERROR;
	/* falls through */
    case Z_DATA_ERROR:
    case Z_MEM_ERROR:
	(void)inflateEnd(&strm);
	return ret;
    }
    (void)inflateEnd(&strm);
    return 0;
}

