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

#define MAXDG   	4096	/* maximum datagram size */
#define MAXUDP  	16	/* maximum datagram size */
#define TDEFAULT	100
#define MAX(a,b)	((a) > (b) ? (a) : (b))

static char *F_CAN_FORMAT_STRG = "      CAN->  CANID 0x%08X R [%d]";
static char *T_CAN_FORMAT_STRG = "      CAN<-  CANID 0x%08X   [%d]";

unsigned char netframe[MAXDG];

static unsigned char M_DEBUG[] = { 0x00, 0x64, 0x03, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -i <can interface>\n", prg);
    fprintf(stderr, "   Version 0.9\n\n");
    fprintf(stderr, "         -i <can int>        can interface - default vcan1\n");
    fprintf(stderr, "         -t <timeout>        time gap between frames in [ms] - default %d\n", TDEFAULT);
    fprintf(stderr, "\n\n");
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

int send_randomized_can_frame(int can_socket, unsigned char *data, int verbose) {
    struct can_frame frame;
    uint32_t i, can_id;
    memset(&frame, 0, sizeof(frame));
    memcpy(&can_id, &data[0], 4);
    frame.can_id = htonl(can_id);
    frame.can_dlc = data[4];
    /* memcpy(&frame.data, &data[5], 8); */

    for (i = 0; i < sizeof(frame.data); i++)
	frame.data[i] = (uint8_t) rand();

    send_can_frame(can_socket, &frame, verbose);
    return 0;
}

int main(int argc, char **argv) {
    int max_fds, opt;
    struct can_frame frame_q, frame_r;
    unsigned int tmsec;
    struct timeval timeout;

    int s, sc;
    struct sockaddr_can caddr;
    struct ifreq ifr;
    socklen_t caddrlen = sizeof(caddr);

    fd_set read_fds;

    int verbose = 0;
    tmsec = TDEFAULT;
    strcpy(ifr.ifr_name, "vcan1");
    memset(&timeout, 0, sizeof(timeout));
    timeout.tv_usec = tmsec * 1000;

    while ((opt = getopt(argc, argv, "i:t:vh?")) != -1) {
	switch (opt) {
	case 'i':
	    strncpy(ifr.ifr_name, optarg, sizeof(ifr.ifr_name) - 1);
	    break;
	case 't':
	    tmsec = strtoul(optarg, (char **)NULL, 10);
	    break;
	case 'v':
	    verbose = 1;
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

    FD_ZERO(&read_fds);
    FD_SET(sc, &read_fds);
    max_fds = sc;

    while (1) {
	s = select(max_fds + 1, &read_fds, NULL, NULL, &timeout);
	if (s < 0) {
	    fprintf(stderr, "select error: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	} else if (s == 0) {
	    send_randomized_can_frame(sc, M_DEBUG, verbose);
	    timeout.tv_sec = 0;
	    timeout.tv_usec = tmsec * 1000;
	} else {
	    /* received a CAN frame */
	    if (FD_ISSET(sc, &read_fds)) {
		if (read(sc, &frame_r, sizeof(struct can_frame)) < 0) {
		    fprintf(stderr, "error reading CAN frame: %s\n", strerror(errno));
		} else if (frame_r.can_id & CAN_EFF_FLAG) {	/* only EFF frames are valid */
		    if (verbose) {
			print_can_frame(F_CAN_FORMAT_STRG, &frame_q);
		    }

		    switch ((frame_r.can_id & 0x00FF0000UL) >> 16) {
		    case 0x65:
			print_can_frame(F_CAN_FORMAT_STRG, &frame_q);
			print_can_frame(F_CAN_FORMAT_STRG, &frame_r);
			break;
		    default:
			break;
		    }
		}
	    }
	}
    }
    close(sc);
    return 0;
}
