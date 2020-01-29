/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 *
 * purpose: wake up M*rklin connect6020 aka 60128
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <ctype.h>
#include <endian.h>

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

#define MAXDG   	4096	/* maximum datagram size */
#define MAXUDP  	16	/* maximum datagram size */
#define MAX(a,b)	((a) > (b) ? (a) : (b))
#define TCYC_MAX	1000	/* max cycle time in ms */

static char *F_CAN_FORMAT_STRG	= "      CAN->   0x%08X   [%d]";
static char *T_CAN_FORMAT_STRG	= "      CAN<-   0x%08X   [%d]";

static unsigned char M_CAN_BOOTLOADER[]	= { 0x00, 0x36, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static unsigned char M_CAN_PING[]	= { 0x00, 0x30, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static unsigned char M_C6020_WAKE_I[]	= { 0x00, 0x36, 0x03, 0x00, 0x05, 0x53, 0x38, 0x38, 0x00, 0xE4, 0x00, 0x00, 0x00 };
static unsigned char M_C6020_WAKE_II[]	= { 0x00, 0x36, 0x03, 0x00, 0x05, 0x53, 0x38, 0x38, 0x00, 0x11, 0x00, 0x00, 0x00 };
static unsigned char M_C6020_WAKE_III[]	= { 0x00, 0x44, 0x03, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static unsigned char M_C6020_WAKE_IV[]	= { 0x00, 0x44, 0x03, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00 };

unsigned char netframe[MAXDG];

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -i <can interface>\n", prg);
    fprintf(stderr, "   Version 1.0\n\n");
    fprintf(stderr, "         -i <can int>        can interface - default can0\n");
    fprintf(stderr, "         -d                  daemonize\n");
}

uint16_t be16(uint8_t *u) {
    return (u[0] << 8) | u[1];
}

uint32_t be32(uint8_t *u) {
    return (u[0] << 24) | (u[1] << 16) | (u[2] << 8) | u[3];
}

void ms_sleep(unsigned int ms) {
    struct timespec to_wait;

    to_wait.tv_sec = ms / 1000;
    to_wait.tv_nsec = (ms % 1000) * 1E6;
    nanosleep(&to_wait, NULL);
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
    int sc, max_fds, opt;
    uint8_t c6020_id[4];
    struct can_frame frame;
    struct sockaddr_can caddr;
    struct ifreq ifr;
    socklen_t caddrlen = sizeof(caddr);
    fd_set read_fds;

    int background = 0;
    int verbose = 1;
    uint16_t function;
    unsigned char raw_frame[13];

    strcpy(ifr.ifr_name, "can0");

    while ((opt = getopt(argc, argv, "i:d:h?")) != -1) {
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
	if (daemon(0, 0) < 0) {
	    fprintf(stderr, "Going into background failed: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
    }

    FD_ZERO(&read_fds);
    FD_SET(sc, &read_fds);
    max_fds = sc;

    /* initiate a CAN Ping */
    memcpy(raw_frame, M_CAN_PING, 13);
    send_defined_can_frame(sc, raw_frame, verbose);
    ms_sleep(20);
    memcpy(raw_frame, M_CAN_BOOTLOADER, 13);
    send_defined_can_frame(sc, raw_frame, verbose);

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
		case 0x31:
		    /* looking for already known conect6020 */
		    function = be16(&frame.data[6]);
		    if ((function == 0x0020) && (be16(frame.data) == 0x3630) && (frame.can_dlc == 8)) {
			memcpy(c6020_id, frame.data, 4);
			printf("awoken conect6020 ID: 0x%08X\n", be32(c6020_id));
			return (EXIT_SUCCESS);
		    }
		    break;
		case 0x37:
		    function = be16(&frame.data[6]);
		    if ((function == 0x0020) && (be16(frame.data) == 0x3630) && (frame.can_dlc == 8)) {
			memcpy(c6020_id, frame.data, 4);
			if (verbose) {
			    printf("Found conect6020 ID: 0x%08x\n", be32(c6020_id));
			    printf("   sending wake-up sequence\n");
			}

			memcpy(raw_frame, M_C6020_WAKE_I, 13);
			memcpy(&raw_frame[5], c6020_id, 4);
			send_defined_can_frame(sc, raw_frame, verbose);
			ms_sleep(20);

			memcpy(raw_frame, M_C6020_WAKE_II, 13);
			memcpy(&raw_frame[5], c6020_id, 4);
			send_defined_can_frame(sc, raw_frame, verbose);
			ms_sleep(1000);

			memcpy(raw_frame, M_C6020_WAKE_III, 13);
			memcpy(&raw_frame[5], c6020_id, 4);
			send_defined_can_frame(sc, raw_frame, verbose);
			ms_sleep(20);

			memcpy(raw_frame, M_C6020_WAKE_IV, 13);
			memcpy(&raw_frame[5], c6020_id, 4);
			send_defined_can_frame(sc, raw_frame, verbose);
			printf("Wake-up sequence completed for ID: 0x%08x\n", be32(c6020_id));
			return (EXIT_SUCCESS);
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
