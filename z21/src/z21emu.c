/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return.
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

/*
 * Z21 Emulation for
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

#include "z21type.h"

#define MAXIPLEN		40	/* maximum IP string length */
#define PRIMARY_UDP_PORT	21105
#define SECONDARY_UDP_PORT	21106

#define MAXDG   256		/* maximum datagram size */

unsigned char udpframe[MAXDG];
int sp, ss, sb;
struct sockaddr_in spaddr, ssaddr, sbaddr, *bsa;
int foreground = 1;
pthread_mutex_t lock;

static char *UDP_SRC_STRG = "->UDP   ";
static char *UDP_DST_STRG = "  UDP-> ";

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -p <port> -s <port>\n", prg);
    fprintf(stderr, "   Version 0.1\n\n");
    fprintf(stderr, "         -p <port>           primary UDP port for the server - default %d\n", PRIMARY_UDP_PORT);
    fprintf(stderr, "         -s <port>           secondary UDP port for the server - default %d\n", SECONDARY_UDP_PORT);
    fprintf(stderr, "         -b <bcast_addr/int> broadcast address or interface\n");
    fprintf(stderr, "         -f                  running in foreground\n\n");
}

void print_udp_frame(unsigned char *udpframe, char *format, int verbose) {
    int i;
    uint16_t length, header;
    struct timeval tv;
    struct tm *tm;

    if (verbose) {
	/* print timestamp */
	gettimeofday(&tv, NULL);
	tm = localtime(&tv.tv_sec);
	printf("%02d:%02d:%02d.%03d  ", tm->tm_hour, tm->tm_min, tm->tm_sec, (int)tv.tv_usec / 1000);

	length = udpframe[0] + (udpframe[1] << 8);
	header = udpframe[2] + (udpframe[3] << 8);
	printf(" 0x%04x 0x%04x", length, header);
	for (i = 4 ; i < length -4 ; i++)
	    printf(" %02x", udpframe[i]);
	printf("\n");
    }
}

int send_broadcast(unsigned char *udpframe, char *format, int verbose) {
    int s;
    uint16_t length;

    length = udpframe[0] + (udpframe[1] << 8);
    s = sendto(sb, udpframe, length, 0, (struct sockaddr *)&sbaddr, sizeof(sbaddr));

    if (s < 0) {
	fprintf(stderr, "UDP write error: %s\n", strerror(errno));
	return (EXIT_FAILURE);
    }
    if (s != length) {
    } else {
	print_udp_frame(udpframe, format, foreground);
    }
    return (EXIT_SUCCESS);
}

int check_data(unsigned char *udpframe) {
    return (EXIT_SUCCESS);
}

int main(int argc, char **argv) {
    pid_t pid;
    pthread_t pth;
    int ret, opt;
    /* primary UDP socket , secondary UDP socket, UDP broadcast socket */
    struct ifaddrs *ifap, *ifa;
    fd_set readfds;
    int primary_port = PRIMARY_UDP_PORT;
    int secondary_port = SECONDARY_UDP_PORT;
    const int on = 1;
    char *udp_dst_address;
    char *bcast_interface;

    memset(&spaddr, 0, sizeof(spaddr));
    memset(&ssaddr, 0, sizeof(ssaddr));
    memset(&sbaddr, 0, sizeof(sbaddr));
    memset(udpframe, 0, sizeof(udpframe));
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

    while ((opt = getopt(argc, argv, "p:s:b:hf?")) != -1) {
	switch (opt) {
	case 'p':
	    primary_port = strtoul(optarg, (char **)NULL, 10);
	    break;
	case 's':
	    secondary_port = strtoul(optarg, (char **)NULL, 10);
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
	case 'f':
	    foreground = 1;
	    break;
	case 'h':
	case '?':
	    print_usage(basename(argv[0]));
	    exit(EXIT_SUCCESS);
	    break;
	default:
	    fprintf(stderr, "Unknown option %c\n", opt);
	    print_usage(basename(argv[0]));
	    exit(EXIT_FAILURE);
	}
    }

    /* prepare primary UDP socket */
    sp = socket(PF_INET, SOCK_DGRAM, 0);
    if (sp < 0) {
	fprintf(stderr, "primary UDP socket error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    spaddr.sin_family = AF_INET;
    spaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    spaddr.sin_port = htons(primary_port);

    if (bind(sp, (struct sockaddr *)&spaddr, sizeof(spaddr)) < 0) {
	fprintf(stderr, "primary UDP bind error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    /* prepare secondary UDP socket */
    ss = socket(PF_INET, SOCK_DGRAM, 0);
    if (ss < 0) {
	fprintf(stderr, "secondary UDP socket error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    ssaddr.sin_family = AF_INET;
    ssaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    ssaddr.sin_port = htons(secondary_port);

    if (bind(ss, (struct sockaddr *)&ssaddr, sizeof(ssaddr)) < 0) {
	fprintf(stderr, "scondary UDP bind error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    /* prepare UDP sending socket */
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

    sb = socket(AF_INET, SOCK_DGRAM, 0);
    if (sb < 0) {
	fprintf(stderr, "sending UDP socket error %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }
    if (setsockopt(sb, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0) {
	fprintf(stderr, "UDP set socket option error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    if (pthread_mutex_init(&lock, NULL)) {
	fprintf(stderr, "can't init mutex %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }
    if (pthread_create(&pth, NULL, LEDMod, &trigger_data)) {
	fprintf(stderr, "can't create pthread %s\n", strerror(errno));
	exit(EXIT_FAILURE);

    }
    if (forground)
	printf("created peridic thread\n");
    }


    if (!foreground) {
	/* Fork off the parent process */
	pid = fork();
	if (pid < 0)
	    exit(EXIT_FAILURE);
	/* If we got a good PID, then we can exit the parent process */

	if (pid > 0) {
	    printf("Going into background ...\n");
	    exit(EXIT_SUCCESS);
	}
    }

    FD_ZERO(&readfds);
    while (1) {
	FD_SET(sp, &readfds);
	FD_SET(sp, &readfds);

	if (select((sp > ss) ? sp + 1 : ss + 1, &readfds, NULL, NULL, NULL) < 0) {
	    fprintf(stderr, "select error: %s\n", strerror(errno));
	    break;
	}

	/* received a UDP packet on primary */
	if (FD_ISSET(sp, &readfds)) {
	    ret = read(ss, udpframe, MAXDG);
	    if (ret < 0) {
		fprintf(stderr, "UDP read error: %s\n", strerror(errno));
		break;
	    } else {
		print_udp_frame(udpframe, UDP_SRC_STRG, foreground);
	    }
	    send_broadcast(udpframe, UDP_DST_STRG, foreground);
	}
	/* received a UDP packet on secondary */
	if (FD_ISSET(ss, &readfds)) {
	    ret = read(ss, udpframe, MAXDG);
	    if (ret < 0) {
		fprintf(stderr, "UDP read error: %s\n", strerror(errno));
		break;
	    } else {
		print_udp_frame(udpframe, UDP_SRC_STRG, foreground);
	    }
	    send_broadcast(udpframe, UDP_DST_STRG, foreground);
	}
    }
    close(sp);
    close(ss);
    close(sb);
    /* if we reach this point, there was an error */
    return EXIT_FAILURE;
}
