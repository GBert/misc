/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "can4linux.h"

#define MAXDG   4096		/* maximum datagram size */
#define MAXUDP  16		/* maximum datagram size */

unsigned char udpframe[MAXDG];
unsigned char udpframe_reply[MAXDG];

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -l <port> -d <port> -i <can device>\n", prg);
    fprintf(stderr, "   Version 0.1\n\n");
    fprintf(stderr, "         -l <port>           listening UDP port for the server - default 15730\n");
    fprintf(stderr, "         -d <port>           destination UDP port for the server - default 15731\n");
    fprintf(stderr, "         -b <broadcast_addr> broadcast address - default 255.255.255.255\n");
    fprintf(stderr, "         -i <can int>        can device - default can0\n");
    fprintf(stderr, "         -f                  running in foreground\n\n");
}

void print_can_frame(char *str, canmsg_t *frame, int verbose) {
    int i;
    if (verbose) {
	printf(str, frame->id);
	printf(" [%d]", frame->length);
	for (i = 0; i < frame->length; i++)
	    printf(" %02x", frame->data[i]);
	printf("\n");
    }
}


int main(int argc, char **argv) {
    int opt;
    canmsg_t frame;
    int sa,sb;			/* UDP socket UDP Broadcast Socket */
    int fdc;			/* can4linux FD */
    struct sockaddr_in saddr, baddr;
    fd_set readfds;
    int s;

    int local_port = 15730;
    int destination_port = 15731;
    int verbose = 0;
    int background = 1;
    int canid = 0;
    char can_interface[64];
    const int on = 1;
    const char broadcast_address[] = "255.255.255.255";

    memset(&saddr, 0, sizeof(saddr));
    memset(&baddr, 0, sizeof(baddr));
    memset(&frame, 0, sizeof(frame));
    memset(udpframe, 0, sizeof(udpframe));
    memset(udpframe_reply, 0, sizeof(udpframe_reply));

    /* prepare udp destination struct with defaults */
    baddr.sin_family = AF_INET;
    baddr.sin_port = htons(destination_port);
    s = inet_pton(AF_INET, broadcast_address, &baddr.sin_addr);
    if (s <= 0) {
        if (s == 0) {
            fprintf(stderr, "UDP IP invalid\n");
        } else {
            fprintf(stderr, "invalid address family\n");
        }
        exit(EXIT_FAILURE);
    }

    while ((opt = getopt(argc, argv, "l:d:b:i:hfv?")) != -1) {
	switch (opt) {
	case 'l':
	    local_port = strtoul(optarg, (char **) NULL, 10);
	    break;
	case 'd':
	    destination_port = strtoul(optarg, (char **) NULL, 10);
	    baddr.sin_port = htons(destination_port);
	    break;
	case 'b':
	    s = inet_pton(AF_INET, optarg, &baddr.sin_addr);
	    if (s <= 0) {
		if (s == 0) {
		    fprintf(stderr, "invalid IP address: %s\n", strerror(errno));
		} else {
		    fprintf(stderr, "inet_pton error: %s\n", strerror(errno));
		}
		exit(EXIT_FAILURE);
	    }
	    break;
	case 'i':
	    strncpy(can_interface, optarg, sizeof(can_interface) - 1);
	    break;

	case 'v':
	    verbose = 1;
	    break;
	case 'f':
	    background = 0;
	    break;

	case 'h':
	case '?':
	    print_usage(basename(argv[0]));
	    exit(EXIT_SUCCESS);

	default:
	    fprintf(stderr, "Unknown option %c\n", opt);
	    print_usage(basename(argv[0]));
	    exit(EXIT_FAILURE);
	}
    }

    if ((sa = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
	fprintf(stderr, "UDP socket error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(local_port);

    if  (bind(sa, (struct sockaddr *) &saddr, sizeof(saddr)) < 0) {
	fprintf(stderr, "UDP bind error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    if ((fdc = open(can_interface, O_RDWR)) < 0 ) {
	fprintf(stderr, "can't open CAN interface: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* prepare UDP sending socket */
    if ((sb = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	fprintf(stderr, "Send UDP socket error %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }
    if (setsockopt(sb, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0) {
	fprintf(stderr, "UDP set socket option error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    if (background) {
	if (daemon(0, 0) < 0) {
	    fprintf(stderr, "Going into background failed: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
    }

    while (1) {
	FD_ZERO(&readfds);
	FD_SET(fdc, &readfds);
	FD_SET(sa, &readfds);

	if (select((fdc > sa) ? fdc + 1 : sa + 1, &readfds, NULL, NULL, NULL) <0 ) {
            fprintf(stderr, "select error: %s\n", strerror(errno));
        };

	/* received a CAN frame */
	if (FD_ISSET(fdc, &readfds)) {

	    if (read(fdc, &frame, 1) < 0)
		fprintf(stderr, "CAN read error: %s\n", strerror(errno));

	    /* prepare UDP packet - extended format */
	    canid = htonl(frame.id);
	    memcpy(udpframe, &canid, 4);
	    udpframe[4] = frame.length;
	    memcpy(&udpframe[5], &frame.data, 8);

	    /* send UDP frame */
	    if (sendto(sb, udpframe, 13, 0, (struct sockaddr *) &baddr, sizeof(baddr)) !=13)
		fprintf(stderr, "UDP write error: %s\n", strerror(errno));

	    print_can_frame("->CAN>UDP CANID 0x%08lX R", &frame, verbose && !background);
	}
	/* received a UDP packet */
	if (FD_ISSET(sa, &readfds)) {
	    if (read(sa, udpframe, MAXDG) == 13) {
		/* prepare CAN frame - extended format only */
		memcpy(&canid, &udpframe[0], 4);
		frame.id = ntohl(canid);
		frame.id &= CAN_EFF_MASK;
		frame.flags |= MSG_EXT;
		frame.length = udpframe[4];
		memcpy(&frame.data, &udpframe[5], 8);

		/* send CAN frame */
		if (write(fdc, &frame, 1) != 1)
		    fprintf(stderr, "CAN write error: %s\n", strerror(errno));

		print_can_frame("->UDP>CAN CANID 0x%08lX  ", &frame, verbose && !background);
	    }
	}
    }
    close(sa);
    close(sb);
    return 0;
}
