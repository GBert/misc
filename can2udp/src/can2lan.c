/*
 * ----------------------------------------------------------------------------
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

#include <linux/can.h>

#define MAXDG   4096		/* maximum datagram size */
#define MAXUDP  16		/* maximum datagram size */

static const int MAXPENDING = 16; /* max outstanding tcp connections */
unsigned char netframe[MAXDG];

void Signal_Handler(sig) {	/* signal handler function */
    switch (sig) {
    case SIGHUP:
	/* rehash the server */
	break;
    case SIGTERM:
	/* finalize the server */
	exit(0);
	break;
    }
}

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -u <udp_port> -t <tcp_port> -d <udp_dest_port> -i <can interface>\n", prg);
    fprintf(stderr, "   Version 0.3\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "         -u <port>           listening UDP port for the server - default 15731\n");
    fprintf(stderr, "         -t <port>           listening TCP port for the server - default 15731\n");
    fprintf(stderr, "         -d <port>           destination UDP port for the server - default 15730\n");
    fprintf(stderr, "         -b <broadcast_addr> broadcast address - default 255.255.255.255\n");
    fprintf(stderr, "         -i <can int>        can interface - default can0\n");
    fprintf(stderr, "         -f                  running in foreground\n");
    fprintf(stderr, "\n");
}

void send_magic_start_60113_frame(int can_socket, int verbose) {
    struct can_frame frame;
    int nbytes;

    frame.can_id = 0x360301UL;
    /* use EFF */
    frame.can_id &= CAN_EFF_MASK;
    frame.can_id |= CAN_EFF_FLAG;
    frame.can_dlc = 5;
    frame.data[0] = 0;
    frame.data[1] = 0;
    frame.data[2] = 0;
    frame.data[3] = 0;
    frame.data[4] = 0x11;
    if ((nbytes =
	write(can_socket, &frame, sizeof(frame))) != sizeof(frame)) {
	perror("error CAN magic 60113 start write\n");
    } else {
	if (verbose)
	    printf("CAN magic 60113 start write\n");
    }
}

int main(int argc, char **argv) {
    pid_t pid;
    extern int optind, opterr, optopt;
    int opt;
    struct can_frame frame;

    int sa, sc, sb, st;		/* UDP socket , CAN socket, UDP Broadcast Socket, TCP Socket */
    struct sockaddr_in saddr, baddr, tcp_addr;
    struct sockaddr_can caddr;
    struct ifreq ifr;
    socklen_t caddrlen = sizeof(caddr);

    fd_set readfds;

    int i, s, nbytes, ret;

    int local_udp_port = 15731;
    int local_tcp_port = 15731;
    int destination_port = 15730;
    int verbose = 1;
    int background = 1;
    int canid = 0;
    const int on = 1;
    char udp_dst_address[] = "255.255.255.255";
    strcpy(ifr.ifr_name, "can0");

    while ((opt = getopt(argc, argv, "u:t:d:b:i:vf?")) != -1) {
	switch (opt) {
	case 'u':
	    local_udp_port = strtoul(optarg, (char **) NULL, 10);
	    break;
	case 't':
	    local_tcp_port = strtoul(optarg, (char **) NULL, 10);
	    break;
	case 'd':
	    destination_port = strtoul(optarg, (char **) NULL, 10);
	    break;
	case 'b':
	    if ( strlen(optarg) <=15 ) {
		strcpy(udp_dst_address, optarg);
	    } else {
		fprintf(stderr, "UDP broadcast address error: %s\n", optarg);
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
	case '?':
	    print_usage(basename(argv[0]));
	    exit(0);
	    break;
	default:
	    fprintf(stderr, "Unknown option %c\n", opt);
	    print_usage(basename(argv[0]));
	    exit(1);
	    break;
	}
    }

    /* prepare udp sending socket struct */
    bzero(&baddr, sizeof(baddr));
    baddr.sin_family = AF_INET;
    baddr.sin_port = htons(destination_port);
    s = inet_pton(AF_INET, udp_dst_address, &baddr.sin_addr);
	perror("UDP inet_pton error\n");
    if (s <= 0) {
	if (s == 0) {
	    fprintf(stderr, "error: UDP sending port not in presentation format\n");
	} else {
	    perror("inet_pton error\n");
	}
    	exit(1);
    }

    /* prepare UDP sending socket */
    if ((sb = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	perror("error creating UDP sending socket");
	exit(1);
    }
    if (setsockopt(sb, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0) {
	perror("error setup UDP broadcast option\n");
	exit(1);
    }

    /* prepare reading UDP socket */
    bzero(&saddr, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(local_udp_port);
    if ((sa = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
	perror("error creating UDP reading socket\n");
	exit(1);
    }

    while (bind(sa, (struct sockaddr *) &saddr, sizeof(saddr)) < 0) {
	printf(".");
	fflush(NULL);
	usleep(100000);
    }

    /* prepare TCP socket */
    if ((st = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
	perror("error creating TCP socket\n");
	exit(1);
    }
    tcp_addr.sin_family = AF_INET;
    tcp_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    tcp_addr.sin_port = htons(local_tcp_port);

    if (bind(st, (struct sockaddr *)&tcp_addr, sizeof(tcp_addr)) < 0) {
	perror("error binding TCP socket\n");	
	exit(1);
    }
    if (listen(st, MAXPENDING) < 0) {
	perror("error starting TCP listener\n");
	exit(1);
    }

    /* prepare CAN socket */
    if ((sc = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
	perror("error creating CAN socket\n");
	exit(1);
    }

    caddr.can_family = AF_CAN;
    if (ioctl(sc, SIOCGIFINDEX, &ifr) < 0) {
	perror("SIOCGIFINDEX error\n");
	exit(1);
    }
    caddr.can_ifindex = ifr.ifr_ifindex;

    if (bind(sc, (struct sockaddr *) &caddr, caddrlen) < 0) {
	perror("error binding CAN socket\n");
	exit(1);
    }


    /* start Maerklin 60113 box */
    send_magic_start_60113_frame(sc, verbose);

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
	FD_SET(st, &readfds);

	ret = select(MAX(sc, sa, st) + 1 , &readfds, NULL, NULL, NULL);

	/* received a CAN frame */
	if (FD_ISSET(sc, &readfds)) {
            if ((nbytes = read(sc, &frame, sizeof(struct can_frame))) < 0) {
		perror("error readind CAN frame");
	    } else if (frame.can_id & CAN_EFF_FLAG) {	/* only EFF frames are valid */
		/* prepare UDP frame */
		bzero(netframe, 13);
		frame.can_id &= CAN_EFF_MASK;
		netframe[0] = (frame.can_id >> 24) & 0x000000FF;
		netframe[1] = (frame.can_id >> 16) & 0x000000FF;
		netframe[2] = (frame.can_id >> 8) & 0x000000FF;
		netframe[3] = frame.can_id & 0x000000FF;
		netframe[4] = frame.can_dlc;
		memcpy(&netframe[5], &frame.data, frame.can_dlc);

		/* send UDP frame */
		s = sendto(sb, netframe, 13, 0, (struct sockaddr *) &baddr, sizeof(baddr));
		if (s != 13)
		    perror("error sending UDP data\n");

		if (verbose && !background) {
		    printf("->CAN>UDP CANID 0x%06X R", frame.can_id);
		    printf(" [%d]", netframe[4]);
		    for (i = 5; i < 5 + frame.can_dlc; i++) {
			printf(" %02x", netframe[i]);
		    }
		    printf("\n");
		}
	    }
	}
	/* received a UDP packet */
	if (FD_ISSET(sa, &readfds)) {
	    if (read(sa, netframe, MAXDG) == 13) {
		/* Maerklin TCP/UDP Format: always 13 bytes
		 *   byte 0 - 3  CAN ID
		 *   byte 4      DLC
		 *   byte 5 - 12 CAN data
		 */
		memcpy(&canid, &netframe[0], 4);
		/* CAN is stored in network big endian format */
		frame.can_id = ntohl(canid);
		frame.can_id &= CAN_EFF_MASK;
		frame.can_id |= CAN_EFF_FLAG;
		frame.can_dlc = netframe[4];
		memcpy(&frame.data, &netframe[5], 8);

		/* send CAN frame */
		if ((nbytes = write(sc, &frame, sizeof(frame))) != sizeof(frame))
		    perror("error writing CAN frame\n");

		if (verbose && !background) {
		    printf("<-UDP>CAN CANID 0x%06X  ", frame.can_id & CAN_EFF_MASK);
		    printf(" [%d]", netframe[4]);
		    for (i = 5; i < 5 + frame.can_dlc; i++) {
			printf(" %02x", netframe[i]);
		    }
		    printf("\n");
		}

		/* answer to CAN ping from LAN to LAN */
		if (((frame.can_id & 0x00FF0000UL) == 0x00310000UL) 
		      && (netframe[11] = 0xEE) && (netframe[12] = 0xEE)) {
		    printf("  received CAN ping\n");
		    netframe[0] = 0x00;
		    netframe[1] = 0x30;
		    netframe[2] = 0x00;
		    netframe[3] = 0x00;
		    netframe[4] = 0x00;
		    s = sendto(sb, netframe, 13, 0, (struct sockaddr *) &baddr, sizeof(baddr));
		    if (s != 13) {
			perror("error sending UDP data (CAN Ping)\n");
		    } else {
			printf("  replied to CAN ping\n");
		    }
		}
	    }
	}
	/* received a TCP packet */
        if (FD_ISSET(st, &readfds)) {
            if (read(st, netframe, MAXDG) == 13) {
            }
	}
    }
    close(sc);
    close(sa);
    close(sb);
    close(st);
    return 0;
}
