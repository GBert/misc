/*
 * Copyright 2013 - 2014 Siegfried Lohberg
 *                  2016 Gerhard Bertelsmann
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

#ifdef USE_MRSYSTEM
#include <mr_ipc.h>
#include <mr_cs2ms2.h>
#include <cs2.h>
#endif

#define BIT(x)		(1<<x)
#define MICRODELAY	15		/* clock frequency 1/MICRODELAY[us] */
#define MINDELAY	2		/* min delay in usec */
#define MAXMODULES	32		/* max numbers of S88 modules */
#define MAXCON		65535-32	/* max numbers of S88 connectors */
#define MAXIPLEN	40		/* maximum IP string length */
#define UDPPORT		15730
/* the maximum amount of pin buffer - assuming 32 bit*/
#define PIO_BUFFER	((MAXMODULES + 1) / 2)

uint32_t bus_ct0[PIO_BUFFER];
uint32_t bus_ct1[PIO_BUFFER];
uint32_t bus_actual[PIO_BUFFER];
uint32_t bus_state[PIO_BUFFER];

struct s88_t {
    struct sockaddr_in baddr;
    int socket;
    int second_socket;
    int background;
    int verbose;
    int invert;
    int offset;
    uint32_t count;
    uint16_t deviceid;
    uint16_t hash;
};

void usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -vf [-b <bcast_addr/int>][-i <0|1>][-p <port>][-m <s88modules>][-o <offset>]\n", prg);
    fprintf(stderr, "   Version 1.4\n\n");
    fprintf(stderr, "         -b <bcast_addr/int> broadcast address or interface - default 255.255.255.255/br-lan\n");
    fprintf(stderr, "         -i [0|1]            invert signals - default 0 -> not inverting\n");
    fprintf(stderr, "         -d <event id>       using event id - default 0\n");
    fprintf(stderr, "         -e <hash>           using CAN <hash>\n");
    fprintf(stderr, "         -m <s88modules>     number of connected S88 modules - default 1\n");
    fprintf(stderr, "         -o <offset>         addressing offset - default 0\n");
    fprintf(stderr, "         -p <port>           destination port of the server - default %d\n", UDPPORT);
    fprintf(stderr, "         -s <port>           second destination port of the server\n");
    fprintf(stderr, "         -t <time in usec>   microtiming in usec - default %d usec\n", MICRODELAY);
    fprintf(stderr, "         -f                  run in foreground (for debugging)\n");
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

    printf("%02d:%02d:%02d.%06d ", tm->tm_hour, tm->tm_min, tm->tm_sec, (int)tv.tv_usec);
    return 0;
}

void print_net_frame(unsigned char *netframe, uint32_t count) {
    uint32_t canid;
    int i, dlc;

    memcpy(&canid, netframe, 4);
    dlc = netframe[4];
    time_stamp();
    printf("[0x%08X] ->S88>UDP  CANID 0x%08X   [%d]", count, ntohl(canid), netframe[4]);
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

#ifdef USE_MRSYSTEM
int sendipc(int socket, unsigned char *netframe)
{  MrIpcCmdType Cmd;
   MrCs2CanDataType CanMsg;

   MrEthCs2Decode(&CanMsg, (char *)netframe);
   MrIpcInit(&Cmd);
   MrIpcEncodeFromCan(&Cmd, &CanMsg);
   MrIpcSetSenderSocket(&Cmd, socket);
   MrIpcSetReceiverSocket(&Cmd, MR_IPC_SOCKET_ALL);
   if (MrIpcGetCanHash(&Cmd) == 0)
      MrIpcSetCanHash(&Cmd, Cs2CalcHash(MrIpcGetCanHash(&Cmd)));
   MrIpcSend(socket, &Cmd);
   MrIpcExit(&Cmd);
   return(13);
}
#endif

int create_event(struct s88_t *s88, int bus, int offset, uint32_t changed_bits, uint32_t value) {
    int s;
    uint32_t i, mask, canid, temp32;
    uint16_t temp16;
    uint8_t netframe[13];

    /* allow usefull M*rklin hashes only */
    canid = 0x00220300 | (s88->hash & 0x0000ffff);

    temp32 = htonl(canid);
    memcpy(netframe, &temp32, 4);
    /* sensor event 8 bytes */
    netframe[4] = 8;
    /* we don't set the time value in the S88 event as of today */
    netframe[11] = 0;
    netframe[12] = 0;

    mask = BIT(31);
    for (i = 0; i < 32; i++) {
	if (changed_bits & mask) {
	    temp16 = htons(s88->deviceid);
	    memcpy(&netframe[5], &temp16, 2);
	    /* TODO */
	    temp16 = htons(bus * 256 + offset + i + 1);
	    memcpy(&netframe[7], &temp16, 2);
	    if (value & mask) {
		netframe[9] = 0;
		netframe[10] = 1;
	    } else {
		netframe[9] = 1;
		netframe[10] = 0;
	    }
#ifdef USE_MRSYSTEM
	    s = sendipc(s88->socket, netframe);
#else
	    s = sendto(s88->socket, netframe, 13, 0, (struct sockaddr *)&s88->baddr, sizeof(s88->baddr));
#endif
	    if (s != 13) {
		fprintf(stderr, "%s: error sending UDP data: %s\n", __func__, strerror(errno));
		return -1;
	    }
	    if (s88->second_socket) {
		s = sendto(s88->second_socket, netframe, 13, 0, (struct sockaddr *)&s88->baddr, sizeof(s88->baddr));
		if (s != 13) {
		    fprintf(stderr, "%s: error sending UDP data (second socket): %s\n", __func__, strerror(errno));
		    return -1;
		}
	    }
	    if (!s88->background)
		print_net_frame(netframe, s88->count);
	}
	mask >>= 1;
    }
    return 0;
}

int analyze_data(struct s88_t *s88, int s88_bits) {
    int ret, i;
    uint32_t c;

    /* using Peter Daneggers 2 bit debouncing buffer code */
    for (i = 0; i <= ((s88_bits - 1) >> 5); i++) {
	c = bus_state[i] ^ ~bus_actual[i];
	bus_ct0[i] = ~(bus_ct0[i] & c);
	bus_ct1[i] = bus_ct0[i] ^ (bus_ct1[i] & c);
	/* 2 bit roll over */
	c &= bus_ct0[i] & bus_ct1[i];
	bus_state[i] ^= c;
	ret = create_event(s88, 0, i * 32 + s88->offset, c, bus_actual[i]);
	if (ret)
	    return -1;
    }
    return 0;
}

int main(int argc, char **argv) {
    int utime, i, j;
    int opt, ret;
    int modulcount = 1;
    struct sockaddr_in destaddr, destaddr_second, *bsa;
    struct ifaddrs *ifap, *ifa;
    struct s88_t s88_data;
    char *udp_dst_address;
    char *bcast_interface;
    uint32_t mask, s88_bit;
    uint8_t newvalue;

    const int on = 1;

    int destination_port = UDPPORT;
    int destination_second_port = 0;
    utime = MICRODELAY;

    memset(&s88_data, 0, sizeof(s88_data));
    s88_data.background = 1;
    /* prepare debouncing buffer */
    memset(bus_actual, 0, sizeof(bus_actual));
    memset(bus_state, 0xff, sizeof(bus_state));
    memset(bus_ct0, 0xff, sizeof(bus_ct0));
    memset(bus_ct1, 0xff, sizeof(bus_ct1));

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

    while ((opt = getopt(argc, argv, "b:e:d:s:i:p:m:o:t:fvh?")) != -1) {
	switch (opt) {
	case 'p':
	    destination_port = strtoul(optarg, (char **)NULL, 10);
	    destaddr.sin_port = htons(destination_port);
	    break;
	case 's':
	    destination_second_port = strtoul(optarg, (char **)NULL, 10);
	    destaddr_second.sin_port = htons(destination_second_port);
	    break;
	case 'b':
	    if (strnlen(optarg, MAXIPLEN) <= MAXIPLEN - 1) {
		/* broadcat IP begins with a number */
		if ((optarg[0] >= '0') && (optarg[0] <= '9')) {
		    memset(udp_dst_address, 0, MAXIPLEN);
		    strncpy(udp_dst_address, optarg, MAXIPLEN - 1);
		} else {
		    memset(bcast_interface, 0, MAXIPLEN);
		    strncpy(bcast_interface, optarg, MAXIPLEN - 1);
		}
	    } else {
		fprintf(stderr, "UDP broadcast address or interface error: %s\n", optarg);
		exit(EXIT_FAILURE);
	    }
	    break;
	case 'd':
	    s88_data.deviceid = atoi(optarg) & 0xffff;
	    break;
	case 'e':
	    s88_data.hash = atoi(optarg) & 0xffff;
	    break;
	case 'i':
	    s88_data.invert = atoi(optarg) & 1;
	    break;
	case 'm':
	    modulcount = atoi(optarg);
	    if (modulcount < 1 || modulcount > MAXMODULES) {
		usage(basename(argv[0]));
		exit(EXIT_FAILURE);
	    }
	    break;
	case 'o':
	    s88_data.offset = atoi(optarg);
	    if (s88_data.offset >= MAXCON) {
		usage(basename(argv[0]));
		exit(EXIT_FAILURE);
	    }
	    break;
	case 't':
	    utime = atoi(optarg);
	    if (utime < MINDELAY) {
		fprintf(stderr, "microtiming value to low: %d\n", utime);
		exit(EXIT_FAILURE);
	    }
	    break;
	case 'v':
	    s88_data.verbose = 1;
	    break;
	case 'f':
	    s88_data.background = 0;
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

    /* prepare second udp sending socket struct if requested */
    if (destination_second_port) {
	memset(&destaddr_second, 0, sizeof(destaddr_second));
	destaddr_second.sin_family = AF_INET;
	destaddr_second.sin_port = htons(destination_second_port);
	destaddr_second.sin_addr = destaddr.sin_addr;
    }

    if (!s88_data.background && s88_data.verbose)
	printf("using broadcast address %s\n", udp_dst_address);
    /* open udp socket */
#ifdef USE_MRSYSTEM
    if ((s88_data.socket = MrIpcConnectIf("lo0", 10234)) < 0) {
#else
    if ((s88_data.socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
#endif
    	fprintf(stderr, "UDP socket error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }
#ifndef USE_MRSYSTEM
    if (setsockopt(s88_data.socket, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0) {
	fprintf(stderr, "UDP set broadcast option error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }
#endif

    if (destination_second_port) {
	/* open second udp socket if requested */
	if ((s88_data.second_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	    fprintf(stderr, "UDP second socket error: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
	if (setsockopt(s88_data.second_socket, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0) {
	    fprintf(stderr, "second UDP set broadcast option error: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
    }

    s88_data.baddr = destaddr;

    if (s88_data.background) {
	pid_t pid;

	/* fork off the parent process */
	pid = fork();
	if (pid < 0) {
	    exit(EXIT_FAILURE);
	}
	/* if we got a good PID, then we can exit the parent process. */
	if (pid > 0) {
	    if (s88_data.verbose)
		printf("Going into background ...\n");
	    exit(EXIT_SUCCESS);
	}
    }

    if (gpio_aw_open("/dev/mem") < 0) {
	fprintf(stderr, "Can't open IO mem: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    gpio_aw_select_output(CLOCK_PIN);
    gpio_aw_select_output(LOAD_PIN);
    gpio_aw_select_output(RESET_PIN);
    gpio_aw_select_input(DATA_PIN);

    /* loop forever */
    while (1) {
	s88_bit = 0;
	gpio_aw_set(LOAD_PIN, HIGH ^ s88_data.invert);
	usec_sleep(utime);
	gpio_aw_set(CLOCK_PIN, HIGH ^ s88_data.invert);
	usec_sleep(utime);
	gpio_aw_set(CLOCK_PIN, LOW ^ s88_data.invert);
	usec_sleep(utime);
	gpio_aw_set(RESET_PIN, HIGH ^ s88_data.invert);
	usec_sleep(utime);
	gpio_aw_set(RESET_PIN, LOW ^ s88_data.invert);
	usec_sleep(utime);
	gpio_aw_set(LOAD_PIN, LOW ^ s88_data.invert);
	s88_data.count++;
	/* get sensor data */
	for (i = 0; i < modulcount; i++) {
	    if ((s88_bit & 0x1f) == 0)
		mask = BIT(31);
	    for (j = 0; j < 16; j++) {
		gpio_aw_get(DATA_PIN, &newvalue);
		if (newvalue ^= s88_data.invert)
		    bus_actual[i >> 1] |= mask;
		else
		    bus_actual[i >> 1] &= ~mask;
		usec_sleep(utime);
		gpio_aw_set(CLOCK_PIN, HIGH ^ s88_data.invert);
		usec_sleep(utime);
		gpio_aw_set(CLOCK_PIN, LOW ^ s88_data.invert);
		s88_bit++;
		mask >>= 1;
	    }
	}
	/* now check data */
	ret = analyze_data(&s88_data, modulcount * 16);
	if (ret < 0) {
	    fprintf(stderr, "problem sending event data - terminating\n");
	    exit(EXIT_FAILURE);
	}
	usec_sleep(100 * utime);
    }
    return 0;
}
