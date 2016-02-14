/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 *
 * this code emulates the M*rklin Gleisbox to some extend . Only for testing
 *  the M*rklinApp and gateway (can2lan) code
 *
 */

/*
 *  Test setup:

  sudo modprobe can
  sudo modprobe vcan
  sudo modprobe can-raw
  sudo modprobe can-gw
  sudo ip link add dev vcan0 type vcan
  sudo ip link add dev vcan1 type vcan
  sudo ifconfig vcan0 up
  sudo ifconfig vcan1 up
  sudo cangw -A -s vcan0 -d vcan1 -e
  sudo cangw -A -s vcan1 -d vcan0 -e
  ./virtual_gfp -f -i vcan1
  ./can2lan -f -i vcan0

 */

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <time.h>
#include <linux/can.h>

#include "virtual_links88.h"

#define MAXDG   	4096	/* maximum datagram size */
#define MAXUDP  	16	/* maximum datagram size */
#define MAX(a,b)	((a) > (b) ? (a) : (b))

static char *F_CAN_FORMAT_STRG   = "      CAN->  CANID 0x%08X R [%d]";
static char *F_S_CAN_FORMAT_STRG = "short CAN->  CANID 0x%08X R [%d]";
static char *T_CAN_FORMAT_STRG   = "      CAN<-  CANID 0x%08X   [%d]";

unsigned char netframe[MAXDG];

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -i <can interface>\n", prg);
    fprintf(stderr, "   Version 0.9\n\n");
    fprintf(stderr, "         -i <can int>        can interface - default vcan1\n");
    fprintf(stderr, "         -d                  daemonize\n\n");
}

int time_stamp(char *timestamp) {
    struct timeval tv;
    struct tm *tm;

    gettimeofday(&tv, NULL);
    tm = localtime(&tv.tv_sec);

    sprintf(timestamp, "%02d:%02d:%02d.%03d", tm->tm_hour, tm->tm_min, tm->tm_sec, (int)tv.tv_usec / 1000);
    return 0;
}

void print_can_frame(char *format_string, struct can_frame *frame) {
    int i;
    char timestamp[16];
    time_stamp(timestamp);
    printf("%s ", timestamp);
    printf(format_string, frame->can_id & CAN_EFF_MASK, frame->can_dlc);
    for (i = 0; i < frame->can_dlc; i++) {
	printf(" %02x", frame->data[i]);
    }
    if (frame->can_dlc < 8) {
	for (i = frame->can_dlc; i < 8; i++) {
	    printf("   ");
	}
    }
    printf("  ");
    for (i = 0; i < frame->can_dlc; i++) {
	if (isprint(frame->data[i]))
	    printf("%c", frame->data[i]);
	else
	    putchar(46);
    }
    printf("\n");
}

int send_can_frame(int can_socket, struct can_frame *frame, int verbose) {
    frame->can_id &= CAN_EFF_MASK;
    frame->can_id |= CAN_EFF_FLAG;
    /* send CAN frame */
    if (write(can_socket, frame, sizeof(*frame)) != sizeof(*frame)) {
	fprintf(stderr, "error writing CAN frame: %s\n", strerror(errno));
	return -1;
    }
    if (verbose)
	print_can_frame(T_CAN_FORMAT_STRG, frame);
    return 0;
}

int send_defined_can_frame(int can_socket, unsigned char *data, int verbose) {
    struct can_frame frame;
    uint32_t can_id;
    memset(&frame, 0, sizeof(frame));
    memcpy(&can_id, &data[0], 4);
    frame.can_id = htonl(can_id);
    frame.can_dlc = data[4];
    memcpy(&frame.data, &data[5], 8);
    send_can_frame(can_socket, &frame, verbose);
    return 0;
}

int main(int argc, char **argv) {
    pid_t pid;
    int max_fds, opt;
    struct can_frame frame;

    int sc;
    struct sockaddr_can caddr;
    struct ifreq ifr;
    socklen_t caddrlen = sizeof(caddr);

    fd_set read_fds;

    int background = 0;
    int verbose = 1;
    strcpy(ifr.ifr_name, "vcan1");

    while ((opt = getopt(argc, argv, "i:dh?")) != -1) {
	switch (opt) {
	case 'i':
	    strncpy(ifr.ifr_name, optarg, sizeof(ifr.ifr_name) - 1);
	    break;
	case 'd':
	    verbose = 0;
	    background = 1;
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

    memset(&caddr, 0, sizeof(caddr));

    /* prepare CAN socket */
    if ((sc = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
	fprintf(stderr, "error creating CAN socket: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }
    caddr.can_family = AF_CAN;
    if (ioctl(sc, SIOCGIFINDEX, &ifr) < 0) {
	fprintf(stderr, "setup CAN socket error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }
    caddr.can_ifindex = ifr.ifr_ifindex;

    if (bind(sc, (struct sockaddr *)&caddr, caddrlen) < 0) {
	fprintf(stderr, "error binding CAN socket: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    /* daemonize the process if requested */
    if (background) {
	/* fork off the parent process */
	pid = fork();
	if (pid < 0) {
	    exit(EXIT_FAILURE);
	}
	/* if we got a good PID, then we can exit the parent process */
	if (pid > 0) {
	    printf("Going into background ...\n");
	    exit(EXIT_SUCCESS);
	}
    }

    FD_ZERO(&read_fds);
    FD_SET(sc, &read_fds);
    max_fds = sc;

    while (1) {
	if (select(max_fds + 1, &read_fds, NULL, NULL, NULL) < 0) {
	    fprintf(stderr, "select error: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
	/* received a CAN frame */
	if (FD_ISSET(sc, &read_fds)) {
	    if (read(sc, &frame, sizeof(struct can_frame)) < 0) {
		fprintf(stderr, "error reading CAN frame: %s\n", strerror(errno));
	    } else if (frame.can_id & CAN_EFF_FLAG) {	/* only EFF frames are valid */
		if (verbose) {
		    print_can_frame(F_CAN_FORMAT_STRG, &frame);
		}

		switch ((frame.can_id & 0x00FF0000UL) >> 16) {
		case 0x30:
		    if (frame.can_dlc == 0)
			send_defined_can_frame(sc, M_LINKS88_ID, verbose);
		    break;
		case 0x36:
		    if (frame.can_dlc == 0)
			send_defined_can_frame(sc, M_LINKS88_BL_INIT, verbose);
		    break;
		case 0x3A:
		    if ((memcmp(&frame.data[0], &M_LINKS88_ID[5], 4) == 0)) {
			switch (frame.data[4]) {
			case 0x00:
			    send_defined_can_frame(sc, M_LINKS88_DATA0_1, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA0_2, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA0_3, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA0_4, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA0_5, verbose);
			    break;
			case 0x01:
			    send_defined_can_frame(sc, M_LINKS88_DATA1_1, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA1_2, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA1_3, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA1_4, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA1_5, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA1_6, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA1_7, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA1_8, verbose);
			    break;
			case 0x02:
			    send_defined_can_frame(sc, M_LINKS88_DATA2_1, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA2_2, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA2_3, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA2_4, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA2_5, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA2_6, verbose);
			    break;
			case 0x03:
			    send_defined_can_frame(sc, M_LINKS88_DATA3_1, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA3_2, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA3_3, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA3_4, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA3_5, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA3_6, verbose);
			    break;
			case 0x04:
			    send_defined_can_frame(sc, M_LINKS88_DATA4_1, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA4_2, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA4_3, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA4_4, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA4_5, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA4_6, verbose);
			    break;
			case 0x05:
			    send_defined_can_frame(sc, M_LINKS88_DATA5_1, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA5_2, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA5_3, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA5_4, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA5_5, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA5_6, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA5_7, verbose);
			    break;
			case 0x06:
			    send_defined_can_frame(sc, M_LINKS88_DATA6_1, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA6_2, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA6_3, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA6_4, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA6_5, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA6_6, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA6_7, verbose);
			    break;
			case 0x07:
			    send_defined_can_frame(sc, M_LINKS88_DATA7_1, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA7_2, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA7_3, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA7_4, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA7_5, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA7_6, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA7_7, verbose);
			    break;
			case 0x08:
			    send_defined_can_frame(sc, M_LINKS88_DATA8_1, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA8_2, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA8_3, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA8_4, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA8_5, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA8_6, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA8_7, verbose);
			    break;
			case 0x09:
			    send_defined_can_frame(sc, M_LINKS88_DATA9_1, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA9_2, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA9_3, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA9_4, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA9_5, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATA9_6, verbose);
			    break;
			case 0x0A:
			    send_defined_can_frame(sc, M_LINKS88_DATAA_1, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATAA_2, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATAA_3, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATAA_4, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATAA_5, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATAA_6, verbose);
			    break;
			case 0x0B:
			    send_defined_can_frame(sc, M_LINKS88_DATAB_1, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATAB_2, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATAB_3, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATAB_4, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATAB_5, verbose);
			    break;
			case 0x0C:
			    send_defined_can_frame(sc, M_LINKS88_DATAC_1, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATAC_2, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATAC_3, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATAC_4, verbose);
			    send_defined_can_frame(sc, M_LINKS88_DATAC_5, verbose);
			    break;
			}
		    }
		    break;
		default:
		    break;
		}
	    } else
		print_can_frame(F_S_CAN_FORMAT_STRG, &frame);
	}
    }
    close(sc);
    return 0;
}
