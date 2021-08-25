/*
 * Copyright 2016 Gerhard Bertelsmann
 *
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Siegfried Lohberg wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 */

#include <ifaddrs.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <netinet/in.h>
#include <netdb.h>

#include <linux/can.h>
#include "s88udp-bpi.h"

#define BIT(x)		(1<<x)
#define MICRODELAY	15	/* clock frequency 1/MICRODELAY[us] */
#define MINDELAY	2	/* min delay in usec */
#define MAXMODULES	32	/* max numbers of S88 modules */
#define MAXIPLEN	40	/* maximum IP string length */
#define UDPPORT		15730

struct s88_t {
    struct sockaddr_in baddr;
    int socket;
    int verbose;
    int data;
    uint16_t sensor;
    uint16_t hash;
};

void usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -v [-b <bcast_addr/int>][-d <0|1>][-p <port>][-e <event id>]\n", prg);
    fprintf(stderr, "   Version 1.0\n\n");
    fprintf(stderr, "         -b <bcast_addr/int> broadcast address or interface - default 255.255.255.255/br-lan\n");
    fprintf(stderr, "         -d [0|1]            event data\n");
    fprintf(stderr, "         -e <event id>       using event id - default 0\n");
    fprintf(stderr, "         -p <port>           destination port of the server - default %d\n", UDPPORT);
    fprintf(stderr, "         -v                  be verbose\n\n");
}

void usec_sleep(int usec) {
    struct timespec to_wait;

    to_wait.tv_sec = 0;
    to_wait.tv_nsec = usec * 1000;
    nanosleep(&to_wait, NULL);
}

int time_stamp(void) {
    /* char *timestamp = (char *)malloc(sizeof(char) * 16); */
    struct timeval tv;
    struct tm *tm;

    gettimeofday(&tv, NULL);
    tm = localtime(&tv.tv_sec);

    printf("%02u:%02u:%02u.%06u ", tm->tm_hour, tm->tm_min, tm->tm_sec, (unsigned int)tv.tv_usec);
    return 0;
}

void print_net_frame(unsigned char *netframe) {
    uint32_t canid;
    int i, dlc;

    memcpy(&canid, netframe, 4);
    dlc = netframe[4];
    time_stamp();
    printf(" ->S88>UDP  CANID 0x%08X   [%u]", ntohl(canid), netframe[4]);
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
    printf("\n");
}

int create_event(struct s88_t *s88, uint16_t address, uint32_t value) {
    int s;
    uint32_t canid, temp32;
    uint8_t netframe[13];

    memset(netframe, 0, sizeof(netframe));
    /* allow usefull M*rklin hashes only */
    canid = 0x00230300 | (s88->hash & 0x0000ffff);

    temp32 = htonl(canid);
    memcpy(netframe, &temp32, 4);
    /* sensor event 8 bytes */
    netframe[4] = 8;

#if 0
    netframe[5] = 1;
    netframe[6] = 1;
    netframe[12] = 100;
#else
    netframe[5] = 0;
    netframe[6] = 0;
#endif

    address = htons(address);
    memcpy(&netframe[7], &address, 2);
    if (value)
	 netframe[10] = 1;
    else
	 netframe[9] = 1;


    s = sendto(s88->socket, netframe, 13, 0, (struct sockaddr *)&s88->baddr, sizeof(s88->baddr));
    if (s != 13) {
	fprintf(stderr, "%s: error sending UDP data: %s\n", __func__, strerror(errno));
	return -1;
    }
    if (s88->verbose)
	print_net_frame(netframe);
    return 0;
}

int main(int argc, char **argv) {
    int opt, ret, value = 0;
    struct sockaddr_in destaddr, *bsa;
    struct ifaddrs *ifap, *ifa;
    struct s88_t s88_data;
    char *udp_dst_address;
    char *bcast_interface;
    uint16_t sensor = 0;

    const int on = 1;

    int destination_port = UDPPORT;

    memset(&s88_data, 0, sizeof(s88_data));

    udp_dst_address = (char *)calloc(MAXIPLEN, 1);
    if (!udp_dst_address) {
	fprintf(stderr, "can't alloc memory for udp_dst_address: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    };

    bcast_interface = (char *)calloc(MAXIPLEN, 1);
    if (!bcast_interface) {
	fprintf(stderr, "can't alloc memory for bcast_interface: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    };

    /* printf ( stderr, "\ns88udp <modulcount>\n\n" ); */

    /* prepare udp sending socket struct */
    memset(&destaddr, 0, sizeof(destaddr));
    destaddr.sin_family = AF_INET;
    destaddr.sin_port = htons(destination_port);

    while ((opt = getopt(argc, argv, "b:e:s:p:d:t:fvh?")) != -1) {
	switch (opt) {
	case 'p':
	    destination_port = strtoul(optarg, (char **)NULL, 10);
	    destaddr.sin_port = htons(destination_port);
	    break;
	case 'b':
	    if (strnlen(optarg, MAXIPLEN) <= MAXIPLEN - 1) {
		/* broadcat IP begins with a number */
		if ((optarg[0] >= '0') && (optarg[0] <= '9')) {
		    strncpy(udp_dst_address, optarg, MAXIPLEN - 1);
		} else {
		    strncpy(bcast_interface, optarg, MAXIPLEN - 1);
		}
	    } else {
		fprintf(stderr, "UDP broadcast address or interface error: %s\n", optarg);
		exit(EXIT_FAILURE);
	    }
	    break;
	case 'd':
	    value = atoi(optarg) & 0x1;
	    break;
	case 'e':
	    s88_data.hash = atoi(optarg) & 0xffff;
	    break;
	case 's':
	    sensor = atoi(optarg) & 0xffff;
	    break;
	case 'v':
	    s88_data.verbose = 1;
	    break;
	case 'h':
	case '?':
	    usage(basename(argv[0]));
	    exit(EXIT_SUCCESS);
	default:
	    usage(basename(argv[0]));
	    exit(EXIT_FAILURE);
	}
    }

    /* get the broadcast address */
    getifaddrs(&ifap);
    for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
	if (ifa->ifa_addr) {
	    if (ifa->ifa_addr->sa_family == AF_INET) {
		bsa = (struct sockaddr_in *)ifa->ifa_broadaddr;
		if (strncmp(ifa->ifa_name, bcast_interface, strlen(bcast_interface)) == 0)
		    udp_dst_address = inet_ntoa(bsa->sin_addr);
	    }
	}
    }
    freeifaddrs(ifap);

    ret = inet_pton(AF_INET, udp_dst_address, &destaddr.sin_addr);
    if (ret <= 0) {
	if (ret == 0)
	    fprintf(stderr, "UDP IP invalid\n");
	else
	    fprintf(stderr, "invalid address family\n");
	exit(EXIT_FAILURE);
    }
    printf("using broadcast address %s\n", udp_dst_address);
    /* open udp socket */
    if ((s88_data.socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	fprintf(stderr, "UDP socket error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }
    if (setsockopt(s88_data.socket, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0) {
	fprintf(stderr, "UDP set broadcast option error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    s88_data.baddr = destaddr;

    create_event(&s88_data, sensor, value);
    free(bcast_interface);

    return 0;
}
