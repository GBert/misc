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

#include "utils.h"
#include "z21.h"

#define MAXIPLEN		40	/* maximum IP string length */
#define PRIMARY_UDP_PORT	21105
#define SECONDARY_UDP_PORT	21106

#define MAXDG   256		/* maximum datagram size */

extern struct z21_data_t z21_data;

void usec_sleep(int usec) {
    struct timespec to_wait;

    if (usec > 999999)
	usec = 999999;
    to_wait.tv_sec = 0;
    to_wait.tv_nsec = usec * 1000;
    nanosleep(&to_wait, NULL);
}

uint8_t xor(unsigned char *data, int length) {
    uint8_t res;
    int i;

    res = 0;
    for (i = 0; i < length; i++)
	res ^= data[i];
    return (res);
}

void print_udp_frame(unsigned char *udpframe, char *format) {
    int i;
    uint16_t length, header;
    struct timeval tv;
    struct tm *tm;

    if (z21_data.foreground) {
	/* print timestamp */
	gettimeofday(&tv, NULL);
	tm = localtime(&tv.tv_sec);
	printf("%02d:%02d:%02d.%03d  ", tm->tm_hour, tm->tm_min, tm->tm_sec, (int)tv.tv_usec / 1000);

	length = udpframe[0] + (udpframe[1] << 8);
	header = udpframe[2] + (udpframe[3] << 8);
	printf(" 0x%04x 0x%04x", length, header);
	for (i = 4; i < length; i++)
	    printf(" %02x", udpframe[i]);
	printf("\n");
    }
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

