/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */


/* Maerklin TCP/UDP Format: always 13 bytes
 *   byte 0 - 3  CAN ID
 *   byte 4      DLC
 *   byte 5 - 12 CAN data
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
#define BLOCK_SIZE	512
#define BOOT_BLOCK_SIZE	2

extern uint16_t CRCCCITT(uint8_t * data, size_t length, uint16_t seed);

unsigned char netframe[MAXDG];

char *CAN_FORMAT_STRG    = "      CAN->  CANID 0x%08X R [%d]";
char *TO_CAN_FORMAT_STRG = "    ->CAN    CANID 0x%08X   [%d]";
char *CECK_FORMAT_STRG   = "  CHEKCAN    CANID 0x%08X   [%d]";

unsigned char M_GLEISBOX_MAGIC_START_SEQUENCE[] = { 0x00, 0x36, 0x03, 0x01, 0x05, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00 };
unsigned char M_CAN_PING[]       = { 0x00, 0x30, 0x47, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
unsigned char M_GB2_RESET[]      = { 0x00, 0x00, 0x47, 0x11, 0x06, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff, 0x00, 0x00 };
unsigned char M_GB2_BOOTLOADER[] = { 0x00, 0x36, 0x47, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
unsigned char M_GB2_BLOCK[]      = { 0x00, 0x36, 0x47, 0x11, 0x06, 0x00, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00 };
unsigned char M_GB2_DATA[]       = { 0x00, 0x36, 0x03, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00 };
unsigned char M_GB2_CRC[]        = { 0x00, 0x36, 0x47, 0x11, 0x07, 0x00, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00 };

unsigned char udpframe[MAXDG];

unsigned char *binfile;
int gb2_fsize, fsize;
int force = 0, verbose = 0;
uint16_t version = 0;
unsigned int gb2_id = 0;
int sc, sb;		/* CAN socket, UDP Broadcast Socket */
int can_mode = 0;
unsigned char lastframe[13];
unsigned char checkframe[13];
int gb2_bin_blocks;
int last_bin_block;
int finished;

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -l <port> -d <port> -b <broacast_addr> -i <can interface> <update file>\n", prg);
    fprintf(stderr, "   Version 0.1\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "         -d <port>           destination UDP port - default 15731\n");
    fprintf(stderr, "         -l <port>           listening UDP port   - default 15730\n");
    fprintf(stderr, "         -b <broacast_addr>  broadcast address - default 255.255.255.255\n");
    fprintf(stderr, "         -i <can int>        switch to can using interface <can int>\n");
    fprintf(stderr, "         -f                  force update even if devie has already the same Versio\n");
    fprintf(stderr, "         -v                  verbose output\n\n");
}

int time_stamp(char *timestamp) {
    struct timeval tv;
    struct tm *tm;

    gettimeofday(&tv, NULL);
    tm = localtime(&tv.tv_sec);

    sprintf(timestamp, "%02d:%02d:%02d.%03d", tm->tm_hour, tm->tm_min, tm->tm_sec, (int)tv.tv_usec / 1000);
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
    printf("%s   ", timestamp);
    printf(format_string, ntohl(canid) & CAN_EFF_MASK, netframe[4]);
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
    printf("  ");
    for (i = 5; i < 13; i++) {
	if (isprint(netframe[i]))
	    printf("%c", netframe[i]);
	else
	    putchar(46);
    }
    printf("\n");
}

void format_can_to_netframe(struct can_frame *frame, unsigned char *netframe) {
    uint32_t canid;

    bzero(netframe, 13);
    frame->can_id &= CAN_EFF_MASK;
    canid = htonl(frame->can_id);
    memcpy(netframe, &canid, 4);
    netframe[4] = frame->can_dlc;
    memcpy(&netframe[5], &frame->data, frame->can_dlc);
}

int netframe_to_net(int net_socket, unsigned char *netframe, int length) {
    if (send(net_socket, netframe, length, 0) != length) {
	fprintf(stderr, "couldn't send on UDP socket: %s\n", strerror(errno));
	return -1;
    }
    return 0;
}

int netframe_to_can(int can_socket, unsigned char *netframe) {
    uint32_t canid;
    struct can_frame frame;
    bzero(&frame, sizeof(frame));
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

int send_frame(unsigned char *netframe) {
    int ret;
    if(can_mode)
	ret = netframe_to_can(sc, netframe);
    else
	ret = netframe_to_net(sb, netframe, 13);
    return ret;
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
	close(fp);
	return NULL;
    }
    /* padding with 0xff */
    memset(&data[gb2_fsize - 8], 0xff, 8);
    if ((fread((void *)data, 1, fsize, fp)) != fsize) {
	fprintf(stderr, "%s: error: fread failed for [%s]\n", __func__, filename);
	close(fp);
	free(data);
	return NULL;
    }
    fclose(fp);
    printf("Gleisbox File Version %d.%d\n", data[6], data[7]);
    return data;
}

int send_next_block_id(int block, unsigned char *netframe) {
    memcpy(netframe, M_GB2_BLOCK, 13);
    memcpy(&netframe[5], &gb2_id,4);
    netframe[10]= block;
    send_frame(netframe);
    return 0;
}

int send_block(unsigned char *binfile, int length, unsigned char *netframe) {
    int part = 0;
    uint16_t crc;
    for (int i = 0; i < length; i+=8 ) {
	memcpy(netframe, M_GB2_DATA, 5);
	netframe[3]=part;
	part++;
	memcpy(&netframe[5], &binfile[i] , 8);
	send_frame(netframe);
    }
    memcpy(netframe, &M_GB2_CRC, 10);
    memcpy(&netframe[5], &gb2_id, 4);
    crc = htons(CRCCCITT(binfile, length, 0xFFFF));
    memcpy(&netframe[10], &crc, 2);
    send_frame(netframe);
    return 0;
}

void fsm(unsigned char *netframe) {
    unsigned int canid;
    unsigned char next_frame[13];
    memcpy(&canid, netframe, 4);
    canid = ntohl(canid);
    switch (canid & 0xFFFF0000UL) {
    case (0x00310000UL):
	printf("received CAN Ping answer\n");
	print_can_frame(" ", netframe, 1);
	/* if ((netframe[5] == 0x47 ) && (netframe[6] == 0x42 )) { */
	if ((netframe[4] == 8) && (netframe[5] == 0x47 )) {
	    memcpy(&gb2_id, &netframe[5], 4);
	    printf("found Gleisbox : 0x%08X  Version %d.%d\n", ntohl(gb2_id), netframe[9], netframe[10]);
	    printf("Start update ...\n");
	    memcpy(next_frame, M_GB2_RESET, 13);
	    memcpy(&next_frame[5], &gb2_id, 4);
	    send_frame(next_frame);
	    /* delay for boot ? */
	    usleep(500000);
	    memcpy(next_frame, M_GB2_BOOTLOADER, 13);
	    send_frame(next_frame);
	}
	break;
    case (0x00370000UL):
	/* should always be true */
	if (gb2_id != 0 ) {
	    if ((netframe[4] == 8) && (memcmp(&netframe[5], &gb2_id, 4) == 0) && (netframe[12] == 0x10)) {
		/* prepare test frama for later use */
		memcpy(checkframe, netframe, 10);
		bzero(&checkframe[10], 3);
		checkframe[1]=0x37;
		checkframe[4]=5;
		checkframe[9]=0x88;
		print_can_frame(CECK_FORMAT_STRG, checkframe, 1);
		last_bin_block = gb2_bin_blocks;
		send_next_block_id(last_bin_block + BOOT_BLOCK_SIZE, lastframe);
	    } else {
		/* first data block */
		if ((memcmp(&netframe[4], &lastframe[4] , 9) == 0) && (last_bin_block == gb2_bin_blocks)) {
		    if (last_bin_block == gb2_bin_blocks) {
			printf("sending block 0x%02X 0x%04X\n", last_bin_block + BOOT_BLOCK_SIZE,
				last_bin_block * BLOCK_SIZE);
			send_block(&binfile[((last_bin_block) * BLOCK_SIZE)],
				gb2_fsize - gb2_bin_blocks * BLOCK_SIZE, lastframe);
			last_bin_block--;
		    }
		}
		if ((memcmp(netframe, checkframe , 10) == 0) && (last_bin_block >= 0)) {
		    printf("sending block 0x%02X 0x%04X\n", last_bin_block + BOOT_BLOCK_SIZE, last_bin_block * BLOCK_SIZE);
		    send_next_block_id(last_bin_block + BOOT_BLOCK_SIZE, lastframe);
		    send_block(&binfile[((last_bin_block) * BLOCK_SIZE)], BLOCK_SIZE, lastframe);
		    last_bin_block--;
		    if (last_bin_block < 0)
			finished = 1;
		}
	    }
	}
	break;
    default:
	break;
    }
}

int main(int argc, char **argv)
{
    extern int optind, opterr, optopt;
    int s, opt;
    struct can_frame frame;

    int sa;		/* UDP socket */
    struct sockaddr_in saddr, baddr;
    struct sockaddr_can caddr;
    struct ifreq ifr;

    /* socklen_t sin_size = sizeof(clientaddr); */
    socklen_t caddrlen = sizeof(caddr);

    fd_set readfds;

    int local_port = 15731;
    int destination_port = 15730;
    const int on = 1;
    const char broadcast_address[] = "255.255.255.255";
    const char default_file_name[] = "016-gb2.bin";
    char *file_name;

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

    while ((opt = getopt(argc, argv, "d:l:b:i:fv?")) != -1) {
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
		exit(1);
	    }
	    break;
	case 'i':
	    strcpy(ifr.ifr_name, optarg);
	    can_mode = 1;
	    break;

	case 'f':
	    force = 1;
	    break;

	case 'v':
	    verbose = 1;
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
	if (optind < argc) {
	    file_name=(char *)malloc(strlen(argv[optind]+1));
	    strcpy(file_name, argv[optind]);
	} else {
	    file_name = (char *)default_file_name;
	}
    }

    binfile = read_data(file_name);
    gb2_bin_blocks = gb2_fsize >> 9;
    printf("%s: fsize 0x%04X gb2_fsize 0x%04X blocks 0x%02X last 0x%04X\n", file_name, fsize, gb2_fsize,
	   gb2_bin_blocks, gb2_fsize - gb2_bin_blocks * BLOCK_SIZE);

    if (can_mode) {
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

	if (bind(sc, (struct sockaddr *)&caddr, caddrlen) < 0) {
	    fprintf(stderr, "CAN bind error: %s\n", strerror(errno));
	    exit(1);
	}
	/* start Maerklin 60113 box */
	if (netframe_to_can(sc, M_GLEISBOX_MAGIC_START_SEQUENCE) < 0) {
	    fprintf(stderr, "can't send CAN magic 60113 start sequence: %s\n", strerror(errno));
	    return -1;
	}
	/* send CAN Ping */
	if (netframe_to_can(sc, M_CAN_PING) < 0) {
	    fprintf(stderr, "can't send CAN Ping: %s\n", strerror(errno));
	    return -1;
	}
	sa=0;
	sb=0;

    } else {
	if ((sa = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
	    fprintf(stderr, "UDP socket error: %s\n", strerror(errno));
	    exit(1);
	}

	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(local_port);

	if (bind(sa, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
	    fprintf(stderr, "UDP bind error: %s\n", strerror(errno));
	    exit(1);
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
	sc=0;
	/* send CAN Ping */
	if (netframe_to_net(sb, M_CAN_PING, 13) < 0) {
	    fprintf(stderr, "can't send CAN Ping: %s\n", strerror(errno));
	    return -1;
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
		format_can_to_netframe(&frame, netframe);
		fsm(netframe);
		if (finished)
		    return 0;
	    }
	}
	/* received a UDP packet */
	if (FD_ISSET(sa, &readfds)) {
	    if (read(sa, udpframe, MAXDG) == 13) {
		fsm(udpframe);
		if (finished)
		    return 0;
	    }
	}
    }
    close(sc);
    close(sa);
    close(sb);
    return 0;
}
