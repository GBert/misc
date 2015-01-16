/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

/* can2eth using a simple UDP format (always 13 Bytes)
 *
 *  FRAME ID (incl. flags) 4 bytes | DLC 1 byte | Data 8 bytes (filled)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <linux/can.h>

#define MAXDG   4096		/* maximum datagram size */
#define MAXUDP  16		/* maximum datagram size */

unsigned char udpframe[MAXDG];

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -l <port> -d <port> -i <can interface>\n", prg);
    fprintf(stderr, "   Version 0.1\n\n");
    fprintf(stderr, "         -l <port>           listening UDP port for the server - default 7654\n");
    fprintf(stderr, "         -d <port>           destination UDP port for the server - default 7655\n");
    fprintf(stderr, "         -b <broadcast_addr> broadcast address - default 255.255.255.255\n");
    fprintf(stderr, "         -i <can int>        can interface - default can0\n");
    fprintf(stderr, "         -v                  being verbose\n\n");
    fprintf(stderr, "         -f                  running in foreground\n\n");
}

int main(int argc, char **argv) {
    pid_t pid;
    extern int optind, opterr, optopt;
    int s, i, opt;
    struct can_frame frame;

    int sa, sc, sb;		/* UDP socket , CAN socket, UDP Broadcast Socket */
    struct sockaddr_in saddr, baddr;
    struct sockaddr_can caddr;
    struct ifreq ifr;
    socklen_t caddrlen = sizeof(caddr);

    fd_set readfds;

    int local_port = 7654;
    int destination_port = 7655;
    int verbose = 0;
    int background = 1;
    uint32_t canid = 0;
    const int on = 1;
    const char broadcast_address[] = "255.255.255.255";
    strcpy(ifr.ifr_name, "can0");

    bzero(&saddr, sizeof(saddr));
    bzero(&baddr, sizeof(baddr));
    bzero(&caddr, sizeof(caddr));
    bzero(&frame, sizeof(frame));
    bzero(udpframe, sizeof(udpframe));
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
        exit(1);
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
		exit(1);
	    }
	    break;
	case 'i':
	    strcpy(ifr.ifr_name, optarg);
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
	    exit(0);

	default:
	    fprintf(stderr, "Unknown option %c\n", opt);
	    print_usage(basename(argv[0]));
	    exit(1);
	}
    }

    if ((sa = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
	fprintf(stderr, "UDP socket error: %s\n", strerror(errno));
	exit(1);
    }

    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(local_port);

    if  (bind(sa, (struct sockaddr *) &saddr, sizeof(saddr)) < 0) {
	fprintf(stderr, "UDP bind error: %s\n", strerror(errno));
	exit(1);
    }

    if ((sc = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
	fprintf(stderr, "CAN socket error: %s\n", strerror(errno));
	exit(1);
    }

    caddr.can_family = AF_CAN;
    if (ioctl(sc, SIOCGIFINDEX, &ifr) < 0) {
	fprintf(stderr, "CAN setup error: %s\n", strerror(errno));
	exit(1);
    }
    caddr.can_ifindex = ifr.ifr_ifindex;

    if (bind(sc, (struct sockaddr *) &caddr, caddrlen) < 0) {
	fprintf(stderr, "CAN bind error: %s\n", strerror(errno));
	exit(1);
    }

    /* prepare UDP sending socket */
    if ((sb = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	fprintf(stderr, "Send UDP socket error %s\n", strerror(errno));
	exit(1);
    }
    if (setsockopt(sb, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0) {
	fprintf(stderr, "UDP set socket option error: %s\n", strerror(errno));
	exit(1);
    }

    if (background) {
	/* Fork off the parent process */
	pid = fork();
	if (pid < 0) {
	    exit(EXIT_FAILURE);
	}
	/* If we got a good PID, then we can exit the parent process */

	if (pid > 0) {
	    printf("Going into background ...\n");
	    exit(EXIT_SUCCESS);
	}
    }

    while (1) {
	FD_ZERO(&readfds);
	FD_SET(sc, &readfds);
	FD_SET(sa, &readfds);

	if (select((sc > sa) ? sc + 1 : sa + 1, &readfds, NULL, NULL, NULL) <0 ) {
            fprintf(stderr, "select error: %s\n", strerror(errno));
        };

	/* received a CAN frame */
	if (FD_ISSET(sc, &readfds)) {

	    if (read(sc, &frame, sizeof(struct can_frame)) < 0) {
		fprintf(stderr, "CAN read error: %s\n", strerror(errno));
	    } else {
		/* prepare UDP frame */
		canid = htonl(frame.can_id);
		memcpy(udpframe, &canid, 4);
		udpframe[4] = frame.can_dlc;
		memcpy(&udpframe[5], &frame.data, frame.can_dlc);

		/* send UDP frame */
		if (sendto(sb, udpframe, 13, 0, (struct sockaddr *) &baddr, sizeof(baddr)) !=13)
		    fprintf(stderr, "UDP write error: %s\n", strerror(errno));

		if (verbose && !background) {
		    if (frame.can_id & CAN_EFF_FLAG)
		        printf("->CAN>UDP CANID 0x%08X  ", frame.can_id & CAN_EFF_MASK);
                    else
		        printf("->CAN>UDP CANID 0x%03X       ", frame.can_id);
		    printf(" [%d]", udpframe[4]);
		    for (i = 5; i < 5 + frame.can_dlc; i++) {
			printf(" %02x", udpframe[i]);
		    }
		    printf("\n");
		}
	    }
	}
	/* received a UDP packet */
	if (FD_ISSET(sa, &readfds)) {
	    if (read(sa, udpframe, MAXDG) == 13) {
		memcpy(&canid, &udpframe[0], 4);
		frame.can_id = ntohl(canid);
		frame.can_dlc = udpframe[4];
		memcpy(&frame.data, &udpframe[5], 8);

		/* send CAN frame */
		memcpy(&frame.data, &udpframe[5], 8);

		if (write(sc, &frame, sizeof(frame)) != sizeof(frame))
		    fprintf(stderr, "CAN write error: %s\n", strerror(errno));

		if (verbose && !background) {
                    if (frame.can_id & CAN_EFF_FLAG)
			printf("<-UDP<CAN CANID 0x%08X  ", frame.can_id & CAN_EFF_MASK);
		    else
			printf("<-UDP<CAN CANID 0x%03X       ", frame.can_id);
		    printf(" [%d]", udpframe[4]);
		    for (i = 5; i < 5 + frame.can_dlc; i++) {
			printf(" %02x", udpframe[i]);
		    }
		    printf("\n");

		}
	    }
	}
    }
    close(sc);
    close(sa);
    close(sb);
    return 0;
}
