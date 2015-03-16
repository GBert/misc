/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <time.h>

#include <sys/time.h>
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

extern uint16_t CRCCCITT(uint8_t *data, size_t length, uint16_t seed);

unsigned char netframe[MAXDG];

char *CAN_FORMAT_STRG      = "      CAN->  CANID 0x%08X R [%d]";
char *TO_CAN_FORMAT_STRG   = "    ->CAN    CANID 0x%08X   [%d]";

unsigned char M_GLEISBOX_MAGIC_START_SEQUENCE[] = { 0x00, 0x36, 0x03, 0x01, 0x05, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00 };

unsigned char udpframe[MAXDG];
unsigned char udpframe_reply[MAXDG];

unsigned char *binfile;
int gb2_fsize, fsize;

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -u -d <port> -i <can interface>\n", prg);
    fprintf(stderr, "   Version 0.1\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "         -u                  use network (UDP) for update\n");
    fprintf(stderr, "         -d <port>           destination UDP port for the server - default 15731\n");
    fprintf(stderr, "         -i <can int>        can interface - default can0\n");
    fprintf(stderr, "         -v                  verbose output\n\n");
}

int time_stamp(char *timestamp){
    struct timeval  tv;
    struct tm      *tm;

    gettimeofday(&tv, NULL);
    tm = localtime(&tv.tv_sec);

    sprintf(timestamp,"%02d:%02d:%02d.%03d", tm->tm_hour, tm->tm_min, tm->tm_sec, (int) tv.tv_usec/1000);
    return 0;
}


void print_can_frame(char *format_string, unsigned char *netframe, int verbose) {
    uint32_t canid;
    int i, dlc;
    char timestamp[16];

    if (!verbose)
        return;

    memcpy(&canid, netframe, 4);
    dlc = netframe[4];
    time_stamp(timestamp);
    printf("%s   ",timestamp);
    printf(format_string, ntohl(canid) & CAN_EFF_MASK, netframe[4]);
    for (i = 5; i < 5 + dlc; i++) {
        printf(" %02x", netframe[i]);
    }
    if (dlc < 8) {
	printf("(%02x", netframe[i]);
	for (i = 6 + dlc ; i < 13 ; i++) {
	    printf(" %02x", netframe[i]);
	}
	printf(")");
    } else {
	printf(" ");
    }
    printf("  ");
    for (i = 5; i < 13; i++) {
	if(isprint(netframe[i]))
	    printf("%c",netframe[i]);
	else
	    putchar(46);
    }
    printf("\n");
}


int frame_to_net(int net_socket, struct sockaddr *net_addr, struct can_frame *frame) {
    int s;
    uint32_t canid;

    bzero(netframe, 13);
    frame->can_id &= CAN_EFF_MASK;
    canid=htonl(frame->can_id);
    memcpy(netframe,&canid,4);
    netframe[4] = frame->can_dlc;
    memcpy(&netframe[5], &frame->data, frame->can_dlc);

    /* send UDP frame */
    s = sendto(net_socket, netframe, 13, 0, net_addr, sizeof(*net_addr));
    if (s != 13) {
	fprintf(stderr, "%s: error sending UDP data %s\n", __func__, strerror(errno));
	return -1;
    }
    return 0;
}

int frame_to_can(int can_socket, unsigned char *netframe) {
    uint32_t canid;
    struct can_frame frame;
    /* Maerklin TCP/UDP Format: always 13 bytes
     *   byte 0 - 3  CAN ID
     *   byte 4      DLC
     *   byte 5 - 12 CAN data
     */
    bzero(&frame,sizeof(frame));
    memcpy(&canid, netframe, 4);
    /* CAN uses (network) big endian format */
    frame.can_id = ntohl(canid);
    frame.can_id &= CAN_EFF_MASK;
    frame.can_id |= CAN_EFF_FLAG;
    frame.can_dlc = netframe[4];
    memcpy(&frame.data, &netframe[5], 8);

    /* send CAN frame */
    if (write(can_socket, &frame, sizeof(frame)) != sizeof(frame)) {
	fprintf(stderr, "%s: error writing CAN frame: %s\n", __func__, strerror(errno));
	return -1;
    }
    /* TODO : it seems Gleisbox needs a short break after every CAN message */
    usleep(2000);
    return 0;
}

unsigned char *read_data(char *filename) {
    FILE *fp;
    unsigned char *data;

    fp = fopen(filename, "rb");
    if (fp == NULL) {
	fprintf(stderr, "%s: error fopen failed [%s]\n", __func__, filename);
	return NULL;
    }

    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    /* for padding */
    gb2_fsize = (fsize + 7) & 0xFFFFFFF8UL;

    if ((data = malloc(fsize)) == NULL) {
	fprintf(stderr, "%s: can't alloc %d bytes for data\n", __func__, fsize);
	return NULL;
    }
    /* padding with 0xff */
    memset(&data[gb2_fsize-8], 0xff, 8);
    if ((fread((void *)data, 1, fsize, fp)) != fsize) {
	fprintf(stderr, "%s: error: fread failed for [%s]\n",__func__, filename);
	return NULL;
    }
    fclose(fp);
    printf("Gleisbox File Version %d.%d\n", data[6], data[7]);
    return data;
}

int main(int argc, char **argv) {
    extern int optind, opterr, optopt;
    int crc, opt;
    struct can_frame frame;

    int sa, sc, sb;		/* UDP socket , CAN socket, UDP Broadcast Socket */
    struct sockaddr_in saddr, baddr;
    struct sockaddr_can caddr;
    struct ifreq ifr;

    /* socklen_t sin_size = sizeof(clientaddr); */
    socklen_t caddrlen = sizeof(caddr);

    fd_set readfds;

    int s, i;

    int local_port = 15731;
    int destination_port = 15730;
    int verbose = 0;
    int background = 1;
    int canid = 0;
    const int on = 1;
    const char broadcast_address[] = "255.255.255.255";
    char file_name[] = "016-gb2.bin";
    strcpy(ifr.ifr_name, "can0");

    bzero(&saddr, sizeof(saddr));
    bzero(&baddr, sizeof(baddr));
    bzero(&caddr, sizeof(caddr));
    bzero(&frame, sizeof(frame));
    bzero(udpframe, sizeof(udpframe));
    bzero(udpframe_reply, sizeof(udpframe_reply));

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

    while ((opt = getopt(argc, argv, "d:i:uv?")) != -1) {
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
    binfile=read_data(file_name);
    int blocks=gb2_fsize>>9;
    printf("%s: fsize 0x%04X gb2_fsize 0x%04X blocks 0x%02X last 0x%04X\n", file_name, fsize, gb2_fsize, blocks, gb2_fsize - blocks*512);
    for (int i=blocks; i>=0; i--) {
	if(i==blocks) { /* last block maybe smaller */
	    crc=CRCCCITT(&binfile[i*512], gb2_fsize - blocks*512, 0xFFFF);
	} else {
	    crc=CRCCCITT(&binfile[i*512], 512, 0xFFFF);
	}
	printf("block: 0x%02X address: 0x%04X crc: 0x%04X\n", i+2, i*512, crc); 
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

    if (frame_to_can(sc, M_GLEISBOX_MAGIC_START_SEQUENCE) < 0) {
	fprintf(stderr, "can't send CAN magic 60113 start sequence\n");
        return -1;
    }

    /* prepare UDP sending socket */
    if ((sb = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	fprintf(stderr, "UDP sending socket error %s\n", strerror(errno));
	exit(1);
    }
    if (setsockopt(sb, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0) {
	fprintf(stderr, "UDP set socket option error: %s\n", strerror(errno));
	exit(1);
    }

    /* start Maerklin 60113 box */
    /* send_magic_start_60113_frame(sc, verbose); */

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
	    } else if (frame.can_id & CAN_EFF_FLAG) {	/* only EFF frames are valid */
		/* prepare UDP frame */
		frame.can_id &= CAN_EFF_MASK;
		udpframe[0] = (frame.can_id >> 24) & 0x000000FF;
		udpframe[1] = (frame.can_id >> 16) & 0x000000FF;
		udpframe[2] = (frame.can_id >> 8) & 0x000000FF;
		udpframe[3] = frame.can_id & 0x000000FF;
		udpframe[4] = frame.can_dlc;
		memcpy(&udpframe[5], &frame.data, frame.can_dlc);

		/* send UDP frame */
		if (sendto(sb, udpframe, 13, 0, (struct sockaddr *) &baddr, sizeof(baddr)) !=13)
		    fprintf(stderr, "UDP write error: %s\n", strerror(errno));

		if (verbose && !background) {
		    printf("->CAN>UDP CANID 0x%06X R", frame.can_id);
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
		if (((frame.can_id & 0x00FF0000UL) ==
		     0x00310000UL) && (udpframe[11] = 0xEE)
		    && (udpframe[12] = 0xEE)) {
		    printf("  received CAN ping\n");
		    memcpy(udpframe_reply, udpframe, 13);
		    udpframe_reply[0] = 0x00;
		    udpframe_reply[1] = 0x30;
		    udpframe_reply[2] = 0x00;
		    udpframe_reply[3] = 0x00;
		    udpframe_reply[4] = 0x00;
		    if (sendto(sb, udpframe_reply, 13, 0, (struct sockaddr *) &baddr, sizeof(baddr)) != 13) 
			fprintf(stderr, "UDP write error: %s\n", strerror(errno));
		    else
			printf("  replied to CAN ping\n");
		}

		if (write(sc, &frame, sizeof(frame)) != sizeof(frame))
		    fprintf(stderr, "CAN write error: %s\n", strerror(errno));

		if (verbose && !background) {
		    printf("<-UDP>CAN CANID 0x%06X  ",
			   frame.can_id & CAN_EFF_MASK);
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
