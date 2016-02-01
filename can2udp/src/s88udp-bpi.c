/*
 * Copyright 2013 - 2014 Siegfried Lohberg
 *                  2016 Gerhard Bertelsmann
 *
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Siegfried Lohberg wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 */

/*
 * Credit: 
 * In dieses Programm flossen Ideen von Gerhard Bertelsmann 
 * und seinem can2udp Projekt ebenso wie aus dem railuino 
 * Projekt von Joerg Pleumann.
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

#define MICRODELAY	15	/* clock frequency 1/MICRODELAY[us] */
#define MINDELAY	5	/* min delay in usec */
#define MAXMODULES	16	/* max numbers of S88 modules */
#define MAXIPLEN	40	/* maximum IP string length */
#define UDPPORT		15730

void usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -vf [-b <bcast_addr/int>][-i <0|1>][-p <port>][-m <s88modules>][-o <offset>]\n", prg);
    fprintf(stderr, "   Version 1.1\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "         -b <bcast_addr/int> broadcast address or interface - default 255.255.255.255/br-lan\n");
    fprintf(stderr, "         -i [0|1]            invert signals - default 0 -> not inverting\n");
    fprintf(stderr, "         -m <s88modules>     number of connected S88 modules - default 1\n");
    fprintf(stderr, "         -o <offset>         number of S88 modules to skip in addressing - default 0\n");
    fprintf(stderr, "         -p <port>           destination port of the server - default %d\n", UDPPORT);
    fprintf(stderr, "         -t <time in usec>   microtiming in usec - default %d usec\n", MICRODELAY);
    fprintf(stderr, "         -f                  run in foreground (for debugging)\n");
    fprintf(stderr, "         -v                  be verbose\n");
    fprintf(stderr, "\n");
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

void send_sensor_event(int sock, const struct sockaddr *destaddr, int verbose, int offset, int address, int value) {
    unsigned char udpframe[32];
    unsigned long can_id;
    int i;

    can_id = 0x00220B01 + offset;
    udpframe[0] = (can_id >> 24) & 0x000000FF;
    udpframe[1] = (can_id >> 16) & 0x000000FF;
    udpframe[2] = (can_id >> 8) & 0x000000FF;
    udpframe[3] = can_id & 0x000000FF;
    udpframe[4] = 8;
    udpframe[5] = 0;
    udpframe[6] = 0;
    udpframe[7] = ((16 * offset + address) >> 8) & 0x000000FF;
    udpframe[8] = (16 * offset + address) & 0x000000FF;

    if (value) {
	udpframe[9] = 0;
	udpframe[10] = 1;
    } else {
	udpframe[9] = 1;
	udpframe[10] = 0;
    }
    udpframe[11] = 0;
    udpframe[12] = 0;

    if (verbose) {
	time_stamp();
	printf("->S88>UDP CANID 0x%06lX R", can_id);
	printf(" [%d]", udpframe[4]);
	for (i = 5; i < 13; i++)
	    printf(" %02x", udpframe[i]);
	printf("\n");
    }
    if (sendto(sock, udpframe, 13, 0, destaddr, sizeof(*destaddr)) != 13) {
	fprintf(stderr, "UDP write error: %s\n", strerror(errno));
	exit(1);
    }
}

int main(int argc, char **argv) {
    int utime, i, j;
    int opt, ret;
    int offset = 0;
    int verbose = 0;
    int modulcount = 1;
    int background = 1;
    int invert_signal = 0;
    int sensors[MAXMODULES * 16];
    int udpsock;
    struct sockaddr_in destaddr, *bsa;
    struct ifaddrs *ifap, *ifa;
    char *udp_dst_address;
    char *bcast_interface;

    const int on = 1;

    int destination_port = UDPPORT;
    utime = MICRODELAY;

    udp_dst_address = (char *)calloc(MAXIPLEN, 1);
    if (!udp_dst_address) {
	fprintf(stderr, "can't alloc memory for udp_dst_address: %s\n", strerror(errno));
	exit(-1);
    };

    bcast_interface = (char *)calloc(MAXIPLEN, 1);
    if (!bcast_interface) {
	fprintf(stderr, "can't alloc memory for bcast_interface: %s\n", strerror(errno));
	exit(-1);
    };

    /* printf ( stderr, "\ns88udp <modulcount>\n\n" ); */

    /* prepare udp sending socket struct */
    memset(&destaddr, 0, sizeof(destaddr));
    destaddr.sin_family = AF_INET;
    destaddr.sin_port = htons(destination_port);

    while ((opt = getopt(argc, argv, "b:i:p:m:o:fvh?")) != -1) {
	switch (opt) {
	case 'p':
	    destination_port = strtoul(optarg, (char **)NULL, 10);
	    destaddr.sin_port = htons(destination_port);
	    break;
	case 'b':
	    if (strnlen(optarg, MAXIPLEN) <= MAXIPLEN - 1) {
		/* IP address begins with a number */
		if ((optarg[0] >= '0') && (optarg[0] <= '9')) {
		    memset(udp_dst_address, 0, MAXIPLEN);
		    strncpy(udp_dst_address, optarg, MAXIPLEN - 1);
		} else {
		    memset(bcast_interface, 0, MAXIPLEN);
		    strncpy(bcast_interface, optarg, MAXIPLEN - 1);
		}
	    } else {
		fprintf(stderr, "UDP broadcast address or interface error: %s\n", optarg);
		exit(-1);
	    }
	    break;
	case 'i':
	    invert_signal = atoi(optarg) & 1;
	    break;
	case 'm':
	    modulcount = atoi(optarg);
	    if (modulcount < 1 || modulcount > MAXMODULES) {
		usage(basename(argv[0]));
		exit(-1);
	    }
	    break;
	case 'o':
	    offset = atoi(optarg);
	    if (offset >= MAXMODULES) {
		usage(basename(argv[0]));
		exit(-1);
	    }
	    break;
	case 't':
	    utime = atoi(optarg);
	    if (utime < MINDELAY) {
		fprintf(stderr, "microtiming value to low: %d\n", utime);
		exit(-1);
	    }
	    break;
	case 'v':
	    verbose = 1;
	    break;
	case 'f':
	    background = 0;
	    break;
	case 'h':
	case '?':
	    usage(basename(argv[0]));
	    exit(0);
	default:
	    usage(basename(argv[0]));
	    exit(1);
	}
    }

    for (i = 0; i < (modulcount * 16); i++) {
	sensors[i] = 0;
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
	exit(1);
    }
    if (!background && verbose)
	printf("using broadcast address %s\n", udp_dst_address);

    /* open udp socket */
    if ((udpsock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	fprintf(stderr, "UDP socket error: %s\n", strerror(errno));
	exit(1);
    }

    if (setsockopt(udpsock, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0) {
	fprintf(stderr, "UDP set broadcast option error: %s\n", strerror(errno));
	exit(1);
    }

    if (background) {
	pid_t pid;

	/* fork off the parent process */
	pid = fork();
	if (pid < 0) {
	    exit(EXIT_FAILURE);
	}
	/* if we got a good PID, then we can exit the parent process. */
	if (pid > 0) {
	    if (verbose)
		printf("Going into background ...\n");
	    exit(0);
	}
    }

    if (gpio_bpi_open("/dev/mem") < 0) {
	fprintf(stderr, "Can't open IO mem: %s\n", strerror(errno));
	exit(-1);
    }

    gpio_bpi_select_output(CLOCK_PIN);
    gpio_bpi_select_output(LOAD_PIN);
    gpio_bpi_select_output(RESET_PIN);
    gpio_bpi_select_input(DATA_PIN);

    /* preset sensor values */
    /* loop forever */
    while (1) {
	uint8_t oldvalue, newvalue;

	gpio_bpi_set(LOAD_PIN, HIGH ^ invert_signal);
	usec_sleep(MICRODELAY);
	gpio_bpi_set(CLOCK_PIN, HIGH ^ invert_signal);
	usec_sleep(MICRODELAY);
	gpio_bpi_set(CLOCK_PIN, LOW ^ invert_signal);
	usec_sleep(MICRODELAY);
	gpio_bpi_set(RESET_PIN, HIGH ^ invert_signal);
	usec_sleep(MICRODELAY);
	gpio_bpi_set(RESET_PIN, LOW ^ invert_signal);
	usec_sleep(MICRODELAY);
	gpio_bpi_set(LOAD_PIN, LOW ^ invert_signal);

	/* get sensor data */
	for (i = 0; i < modulcount; i++) {
	    for (j = 0; j < 16; j++) {
		usec_sleep(MICRODELAY / 2);

		oldvalue = sensors[i * 16 + j];
		gpio_bpi_get(DATA_PIN, &newvalue);
		newvalue ^= invert_signal;
		if (!background && verbose && modulcount == 1)
		    printf("%02x ", sensors[i * 16 + j]);

		if (newvalue != oldvalue) {
		    if (verbose && modulcount > 1) {
			time_stamp();
			printf("sensor %d changed value to %d\n", i * 16 + j + 1, newvalue);
		    }

		    send_sensor_event(udpsock, (struct sockaddr *)&destaddr, verbose, offset, i * 16 + j + 1, newvalue);

		    sensors[i * 16 + j] = newvalue;
		}

		usec_sleep(MICRODELAY / 2);
		gpio_bpi_set(CLOCK_PIN, HIGH ^ invert_signal);
		usec_sleep(MICRODELAY);
		gpio_bpi_set(CLOCK_PIN, LOW ^ invert_signal);
	    }
	}
	if (!background && verbose && modulcount == 1)
	    printf("\r");
	fflush(stdout);
	usec_sleep((MAXMODULES + 4 - modulcount + 1) * 16 * MICRODELAY);
    }
    return 0;
}
