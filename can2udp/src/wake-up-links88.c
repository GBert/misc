/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 *
 * wake up M*rklin LinkS88
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

#define SLEEPING	10000
#define MAXDG   	4096	/* maximum datagram size */
#define MAXUDP  	16	/* maximum datagram size */
#define MAX(a,b)	((a) > (b) ? (a) : (b))

char *F_CAN_FORMAT_STRG		= "      CAN->  CANID 0x%08X R [%d]";
char *T_CAN_FORMAT_STRG		= "      CAN<-  CANID 0x%08X   [%d]";

unsigned char M_LINKS88_ID[]		= { 0x00, 0x31, 0x03, 0x00, 0x08, 0x53, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, 0x10 };
unsigned char M_LINKS88_WAKE_I[]	= { 0x00, 0x36, 0x03, 0x00, 0x05, 0x53, 0x38, 0x38, 0x00, 0xE4, 0x00, 0x00, 0x00 };
unsigned char M_LINKS88_WAKE_II[]	= { 0x00, 0x36, 0x03, 0x00, 0x05, 0x53, 0x38, 0x38, 0x00, 0x11, 0x00, 0x00, 0x00 };
unsigned char M_LINKS88_WAKE_III[]	= { 0x00, 0x01, 0x03, 0x00, 0x07, 0x53, 0x38, 0x38, 0x00, 0x0C, 0x00, 0x00, 0x00 };

unsigned char netframe[MAXDG];

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -i <can interface>\n", prg);
    fprintf(stderr, "   Version 0.2\n\n");
    fprintf(stderr, "         -i <can int>        can interface - default can0\n");
    fprintf(stderr, "         -d                  daemonize\n\n");
    fprintf(stderr, "         -e                  exit after wake up\n\n");
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
    bzero(&frame, sizeof(frame));
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
    int exit_on_wake_up = 0;
    unsigned char links88_id = 0;
    unsigned char raw_frame[13];

    strcpy(ifr.ifr_name, "can0");

    while ((opt = getopt(argc, argv, "i:deh?")) != -1) {
	switch (opt) {
	case 'i':
	    strcpy(ifr.ifr_name, optarg);
	    break;
	case 'd':
	    verbose = 0;
	    background = 1;
	    break;
	case 'e':
	    exit_on_wake_up= 1;
	    break;
	case 'h':
	case '?':
	    print_usage(basename(argv[0]));
	    exit(0);
	    break;
	default:
	    fprintf(stderr, "Unknown option %c\n", opt);
	    print_usage(basename(argv[0]));
	    exit(1);
	}
    }

    bzero(&caddr, sizeof(caddr));

    /* prepare CAN socket */
    if ((sc = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
	fprintf(stderr, "error creating CAN socket: %s\n", strerror(errno));
	exit(1);
    }
    caddr.can_family = AF_CAN;
    if (ioctl(sc, SIOCGIFINDEX, &ifr) < 0) {
	fprintf(stderr, "setup CAN socket error: %s\n", strerror(errno));
	exit(1);
    }
    caddr.can_ifindex = ifr.ifr_ifindex;

    if (bind(sc, (struct sockaddr *)&caddr, caddrlen) < 0) {
	fprintf(stderr, "error binding CAN socket: %s\n", strerror(errno));
	exit(1);
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
	    exit(1);
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
		case 0x31:
		case 0x37:
		    if (frame.can_dlc == 8) {
			/* check if there is a response from LinkS88 */
			if (memcmp(&frame.data[0], &M_LINKS88_ID[5], 3) == 0) {
			    links88_id = frame.data[3];
			    if (verbose) {
				printf("Found LinkS88 ID: 0x%02x\n", links88_id);
				printf("   sending wake-up sequence\n");
			    }
			    memcpy(raw_frame, M_LINKS88_WAKE_I, 13);
			    raw_frame[8] = links88_id;
			    send_defined_can_frame(sc, raw_frame, verbose);
			    usleep(SLEEPING);
			    memcpy(raw_frame, M_LINKS88_WAKE_II, 13);
			    raw_frame[8] = links88_id;
			    send_defined_can_frame(sc, raw_frame, verbose);
			    usleep(SLEEPING);
			    memcpy(raw_frame, M_LINKS88_WAKE_III, 13);
			    raw_frame[8] = links88_id;
			    raw_frame[11] = links88_id;
			    send_defined_can_frame(sc, raw_frame, verbose);
			    if (exit_on_wake_up) {
				close(sc);
				exit(0);
			    }
			}
		    }
		    break;
		default:
		    break;
		}
	    }
	}
    }
    return 0;
}
