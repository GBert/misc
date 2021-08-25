/*
 * Copyright 2013 - 2014 Siegfried Lohberg
 *                  2019 Gerhard Bertelsmann
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
#include "s88udp-rpi.h"

#define BIT(nr)		(1UL << (nr))
#define MICRODELAY	50	/* clock frequency 1/MICRODELAY[us] */
#define MINDELAY	2	/* min delay in usec */
#define MAXMODULES	32	/* max numbers of S88 modules */
#define MAXCON		65535-32/* max numbers of S88 connectors */
#define MAXIPLEN	40	/* maximum IP string length */
#define UDPPORT		15730
/* the maximum amount of pin buffer - assuming 32 bit*/
#define PIO_BUFFER	((MAXMODULES + 1) / 2)

uint32_t bus_ct0[PIO_BUFFER];
uint32_t bus_ct1[PIO_BUFFER];
uint32_t bus_actual[PIO_BUFFER];
uint32_t bus_state[PIO_BUFFER];

static char delimiters[] = " .,;:!-";

#define DATA_PIN_I      0
#define CLOCK_PIN_I     1
#define LOAD_PIN_I      2
#define RESET_PIN_I     3

struct s88_t {
    struct sockaddr_in baddr;
    int socket;
    int background;
    int verbose;
    int invert;
    int offset;
    uint32_t count;
    uint16_t deviceid;
    uint16_t hash;
    int pin[4];
};

void usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -vf [-b <bcast_addr/int>][-i <0|1>][-p <port>][-m <s88modules>][-o <offset>]\n", prg);
    fprintf(stderr, "   Version 2.0\n\n");
    fprintf(stderr, "         -b <bcast_addr/int> broadcast address or interface - default 255.255.255.255/br-lan\n");
    fprintf(stderr, "         -c <config_string>  set GPIOs like \"4,17,27,22\"\n");
    fprintf(stderr, "                              4  Data\n");
    fprintf(stderr, "                              17 Clock\n");
    fprintf(stderr, "                              27 Load\n");
    fprintf(stderr, "                              22 Reset\n");
    fprintf(stderr, "         -i [0|1]            invert signals - default 0 -> not inverting\n");
    fprintf(stderr, "         -d <event id>       using event id - default 0\n");
    fprintf(stderr, "         -e <hash>           using CAN <hash>\n");
    fprintf(stderr, "         -m <s88modules>     number of connected S88 modules - default 1\n");
    fprintf(stderr, "         -o <offset>         addressing offset - default 0\n");
    fprintf(stderr, "         -p <port>           destination port of the server - default %d\n", UDPPORT);
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
    printf("[0x%08X] ->S88>UDP  CANID 0x%08X   [%u]", count, ntohl(canid), netframe[4]);
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

int create_event(struct s88_t *s88, int bus, int offset, uint32_t changed_bits, uint32_t value) {
    int s;
    uint32_t i, mask, canid, temp32;
    uint16_t temp16;
    uint8_t netframe[13];

    /* allow usefull M*rklin hashes only */
    canid = 0x00230300 | (s88->hash & 0x0000ffff);

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
	    s = sendto(s88->socket, netframe, 13, 0, (struct sockaddr *)&s88->baddr, sizeof(s88->baddr));
	    if (s != 13) {
		fprintf(stderr, "%s: error sending UDP data: %s\n", __func__, strerror(errno));
		return -1;
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
    struct sockaddr_in destaddr, *bsa;
    struct ifaddrs *ifap, *ifa;
    struct s88_t s88_data;
    char *udp_dst_address;
    char *bcast_interface;
    uint32_t mask, s88_bit;
    uint8_t newvalue;
    char *config_string = NULL;
    char *token = NULL;

    const int on = 1;
    mask = 0;

    int destination_port = UDPPORT;
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

    while ((opt = getopt(argc, argv, "b:c:e:d:i:p:m:o:t:fvh?")) != -1) {
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
	case 'c':
	    config_string = strdup(optarg);
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

    /* set default pins */
    s88_data.pin[DATA_PIN_I]  = DATA_PIN;
    s88_data.pin[CLOCK_PIN_I] = CLOCK_PIN;
    s88_data.pin[LOAD_PIN_I]  = LOAD_PIN;
    s88_data.pin[RESET_PIN_I] = RESET_PIN;

    /* mini config string parser */
    if (config_string != NULL) {
	int j = 0;
	while ((token = strsep(&config_string, delimiters))) {
	    s88_data.pin[j] = (int)strtoul(token++, (char **)NULL, 10);
	    if (!s88_data.background && s88_data.verbose)
		printf("GPIO pin %d value %d\n", j, s88_data.pin[j]);
	    j++;
	}
    }

    ret = inet_pton(AF_INET, udp_dst_address, &destaddr.sin_addr);
    if (ret <= 0) {
	if (ret == 0)
	    fprintf(stderr, "UDP IP invalid\n");
	else
	    fprintf(stderr, "invalid address family\n");
	exit(EXIT_FAILURE);
    }
    if (!s88_data.background && s88_data.verbose)
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

    if (s88_data.background) {
	if (daemon(0, 0) < 0) {
	    fprintf(stderr, "Going into background failed: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
    }

    if (gpio_rpi_open("/dev/mem") < 0) {
	fprintf(stderr, "Can't open IO mem: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    gpio_rpi_select_output(s88_data.pin[CLOCK_PIN_I]);
    gpio_rpi_select_output(s88_data.pin[LOAD_PIN_I]);
    gpio_rpi_select_output(s88_data.pin[RESET_PIN_I]);
    gpio_rpi_select_input(s88_data.pin[DATA_PIN_I]);

    /* loop forever */
    while (1) {
	s88_bit = 0;
	gpio_rpi_set(s88_data.pin[LOAD_PIN_I], HIGH ^ s88_data.invert);
	usec_sleep(utime);
	gpio_rpi_set(s88_data.pin[CLOCK_PIN_I], HIGH ^ s88_data.invert);
	usec_sleep(utime);
	gpio_rpi_set(s88_data.pin[CLOCK_PIN_I], LOW ^ s88_data.invert);
	usec_sleep(utime);
	gpio_rpi_set(s88_data.pin[RESET_PIN_I], HIGH ^ s88_data.invert);
	usec_sleep(utime);
	gpio_rpi_set(s88_data.pin[RESET_PIN_I], LOW ^ s88_data.invert);
	usec_sleep(utime);
	gpio_rpi_set(s88_data.pin[LOAD_PIN_I], LOW ^ s88_data.invert);
	s88_data.count++;
	/* get sensor data */
	for (i = 0; i < modulcount; i++) {
	    if ((s88_bit & 0x1f) == 0)
		mask = BIT(31);
	    for (j = 0; j < 16; j++) {
		gpio_rpi_get(s88_data.pin[DATA_PIN_I], &newvalue);
		if (newvalue ^= s88_data.invert)
		    bus_actual[i >> 1] |= mask;
		else
		    bus_actual[i >> 1] &= ~mask;
		usec_sleep(utime);
		gpio_rpi_set(s88_data.pin[CLOCK_PIN_I], HIGH ^ s88_data.invert);
		usec_sleep(utime);
		gpio_rpi_set(s88_data.pin[CLOCK_PIN_I], LOW ^ s88_data.invert);
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
