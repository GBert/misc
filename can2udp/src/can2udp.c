/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

/* Thanks to Stefan Krauss and the SocketCAN team
 */

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <linux/can.h>

#define MAXDG   4096		/* maximum datagram size */
#define MAXUDP  16		/* maximum datagram size */

unsigned char udpframe[MAXDG];
unsigned char udpframe_reply[MAXDG];

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -l <port> -d <port> -i <can interface>\n", prg);
    fprintf(stderr, "   Version 0.93\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "         -l <port>           listening UDP port for the server - default 15731\n");
    fprintf(stderr, "         -d <port>           destination UDP port for the server - default 15730\n");
    fprintf(stderr, "         -b <broadcast_addr> broadcast address - default 255.255.255.255\n");
    fprintf(stderr, "         -i <can int>        can interface - default can0\n");
    fprintf(stderr, "         -f                  running in foreground\n\n");
    fprintf(stderr, "         -v                  verbose output (in foreground)\n\n");
}

uint32_t be32(uint8_t *u) {
    return (u[0] << 24) | (u[1] << 16) | (u[2] << 8) | u[3];
}

/*
  CS2 CAN hash generation

  hash' = highword ^ lowword

  xxxxxxx11 0xxxxxxxx
  ^^^^^^^    ^^^^^^^^
   \\\\\\\   ||||||||
    \\\\\\-- ||||||||
     \\\\\\ \||||||||
  000xxxxxx xxxxxxxxx
 */

uint16_t generateHash(uint32_t uid) {
    uint16_t hash, highword, lowword;

    highword = uid >> 16;
    lowword = uid & 0xFFFF;
    hash = highword ^ lowword;
    hash = (((hash << 3) & 0xFF00) | 0x0300) | (hash & 0x7F);
    return hash;
}

void send_magic_start_60113_frame(int can_socket, int verbose) {
    struct can_frame frame;
    memset(&frame, 0, sizeof(frame));

    frame.can_id = 0x360301UL;
    /* use EFF */
    frame.can_id &= CAN_EFF_MASK;
    frame.can_id |= CAN_EFF_FLAG;
    frame.can_dlc = 5;
    frame.data[4] = 0x11;
    if (write(can_socket, &frame, sizeof(frame)) != sizeof(frame)) {
	fprintf(stderr, "CAN magic 60113 start write error: %s\n", strerror(errno));
    } else {
	if (verbose)
	    printf("CAN magic 60113 start write\n");
    }
}

int main(int argc, char **argv) {
    int s, i, opt;
    struct can_frame frame;
    int sa, sc, sb;		/* UDP socket , CAN socket, UDP Broadcast Socket */
    struct sockaddr_in saddr, baddr;
    struct sockaddr_can caddr;
    struct ifreq ifr;
    /* socklen_t sin_size = sizeof(clientaddr); */
    socklen_t caddrlen = sizeof(caddr);
    fd_set readfds;
    int local_port = 15731;
    int destination_port = 15730;
    int verbose = 0;
    int background = 1;
    int canid = 0;
    const int on = 1;
    const char rocrail_server[] = "255.255.255.255";
    strcpy(ifr.ifr_name, "can0");

    memset(&saddr, 0, sizeof(saddr));
    memset(&baddr, 0, sizeof(baddr));
    memset(&caddr, 0, sizeof(caddr));
    memset(&frame, 0, sizeof(frame));
    memset(udpframe, 0, sizeof(udpframe));
    memset(udpframe_reply, 0, sizeof(udpframe_reply));

    /* prepare udp destination struct with defaults */
    baddr.sin_family = AF_INET;
    baddr.sin_port = htons(destination_port);
    s = inet_pton(AF_INET, rocrail_server, &baddr.sin_addr);
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
	    local_port = strtoul(optarg, (char **)NULL, 10);
	    break;
	case 'd':
	    destination_port = strtoul(optarg, (char **)NULL, 10);
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
	    strncpy(ifr.ifr_name, optarg, sizeof(ifr.ifr_name) - 1);
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

    if (bind(sa, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
	fprintf(stderr, "UDP bind error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    if ((sc = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
	fprintf(stderr, "CAN socket error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    caddr.can_family = AF_CAN;
    if (ioctl(sc, SIOCGIFINDEX, &ifr) < 0) {
	fprintf(stderr, "CAN setup error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }
    caddr.can_ifindex = ifr.ifr_ifindex;

    if (bind(sc, (struct sockaddr *)&caddr, caddrlen) < 0) {
	fprintf(stderr, "CAN bind error: %s\n", strerror(errno));
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

    /* start Maerklin 60113 box */
    send_magic_start_60113_frame(sc, verbose);

    if (background) {
	if (daemon(0, 0) < 0) {
	    fprintf(stderr, "Going into background failed: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
    }

    while (1) {
	FD_ZERO(&readfds);
	FD_SET(sc, &readfds);
	FD_SET(sa, &readfds);

	if (select((sc > sa) ? sc + 1 : sa + 1, &readfds, NULL, NULL, NULL) < 0) {
	    fprintf(stderr, "select error: %s\n", strerror(errno));
	};

	/* received a CAN frame */
	if (FD_ISSET(sc, &readfds)) {

	    if (read(sc, &frame, sizeof(struct can_frame)) < 0) {
		fprintf(stderr, "CAN read error: %s\n", strerror(errno));
	    } else if (frame.can_id & CAN_EFF_FLAG) {	/* only EFF frames are valid */
		/* prepare UDP frame */
		frame.can_id &= CAN_EFF_MASK;
		canid = htonl(frame.can_id);
		memcpy(udpframe, &canid, 4);
		udpframe[4] = frame.can_dlc;
		memcpy(&udpframe[5], &frame.data, frame.can_dlc);

		/* send UDP frame */
		if (sendto(sb, udpframe, 13, 0, (struct sockaddr *)&baddr, sizeof(baddr)) != 13)
		    fprintf(stderr, "UDP write error: %s\n", strerror(errno));

		if (verbose && !background) {
		    printf("->CAN>UDP CANID 0x%06X R", frame.can_id);
		    printf(" [%u]", udpframe[4]);
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
		/* Maerklin UDP Format: always 13 bytes
		 *   byte 0-3 CAN ID
		 *   byte 4 DLC
		 *   byte 5-12 CAN data
		 */
		memcpy(&canid, &udpframe[0], 4);
		/* CAN is stored in network Big Endian format */
		frame.can_id = ntohl(canid);
		frame.can_id &= CAN_EFF_MASK;
		frame.can_id |= CAN_EFF_FLAG;
		frame.can_dlc = udpframe[4];
		memcpy(&frame.data, &udpframe[5], 8);
		/* send CAN frame */
		memcpy(&frame.data, &udpframe[5], 8);
		/* answer to CAN ping from LAN to LAN */
		if (((frame.can_id & 0x00FF0000UL) == 0x00310000UL) &&
		    (udpframe[11] == 0xEE) && (udpframe[12] == 0xEE)) {
		    printf("  received CAN ping\n");
		    memcpy(udpframe_reply, udpframe, 13);
		    udpframe_reply[0] = 0x00;
		    udpframe_reply[1] = 0x30;
		    udpframe_reply[2] = 0x00;
		    udpframe_reply[3] = 0x00;
		    udpframe_reply[4] = 0x00;
		    if (sendto(sb, udpframe_reply, 13, 0, (struct sockaddr *)&baddr, sizeof(baddr)) != 13)
			fprintf(stderr, "UDP write error: %s\n", strerror(errno));
		    else
			printf("  replied to CAN ping\n");
		}

		if (write(sc, &frame, sizeof(frame)) != sizeof(frame))
		    fprintf(stderr, "CAN write error: %s\n", strerror(errno));

		if (verbose && !background) {
		    printf("<-UDP>CAN CANID 0x%06X  ", frame.can_id & CAN_EFF_MASK);
		    printf(" [%u]", udpframe[4]);
		    for (i = 5; i < 5 + frame.can_dlc; i++)
			printf(" %02x", udpframe[i]);
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
