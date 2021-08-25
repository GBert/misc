/* ------------------------------------------- ---------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

/* Contributions by Rainer Müller */

#include <stdlib.h>
#include <stdio.h>
#include <libgen.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <ctype.h>
#include <pcap.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#define  __FAVOR_BSD
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <time.h>
#include <linux/can.h>
#include <linux/can/raw.h>

#define RED     "\x1B[31m"
#define GRN     "\x1B[32m"
#define YEL     "\x1B[33m"
#define BLU     "\x1B[34m"
#define MAG     "\x1B[35m"
#define CYN     "\x1B[36m"
#define WHT     "\x1B[37m"
#define RESET   "\x1B[0m"

#define MAX_PAKETE      256
#define MAXSIZE         256
#define MAXDG           4096	/* maximum datagram size */
#define MAXUDP          16	/* maximum datagram size */
#define MAX(a,b)        ((a) > (b) ? (a) : (b))

struct knoten *statusdaten = NULL;
struct knoten *messwert = NULL;

unsigned char buffer[MAX_PAKETE * 8];
int verbose = 0, kanal = 0, expconf = 0;

static char *F_N_CAN_FORMAT_STRG = "  CAN  0x%08X  [%d]";
static char *F_N_SFF_FORMAT_STRG = "  CAN  <S>  0x%03X  [%d]";

uint16_t be16(uint8_t * u) {
    return (u[0] << 8) | u[1];
}

uint32_t be32(uint8_t * u) {
    return (u[0] << 24) | (u[1] << 16) | (u[2] << 8) | u[3];
}

uint32_t le32(uint8_t * u) {
    return (u[3] << 24) | (u[2] << 16) | (u[1] << 8) | u[0];
}

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -i <can interface>\n", prg);
    fprintf(stderr, "   Version 0.1\n\n");
    fprintf(stderr, "         -i <can int>      CAN interface - default can0\n");
    fprintf(stderr, "         -l <candump file> candump file\n");
    fprintf(stderr, "         -v                verbose output for TCP/UDP and errorframes\n\n");
    fprintf(stderr, "         -h                show this help\n\n");
}

int time_stamp(char *timestamp) {
    struct timeval tv;
    struct tm *tm;

    gettimeofday(&tv, NULL);
    tm = localtime(&tv.tv_sec);

    sprintf(timestamp, "%02d:%02d:%02d.%03d", tm->tm_hour, tm->tm_min, tm->tm_sec, (int)tv.tv_usec / 1000);
    return 0;
}

void frame_to_can(unsigned char *netframe, struct can_frame *frame) {
    frame->can_id = be32(netframe);
    frame->can_dlc = netframe[4];
    memcpy(&frame->data, &netframe[5], 8);
}

void canframe_to_can(unsigned char *netframe, struct can_frame *frame) {
    frame->can_id = le32(netframe);
    frame->can_dlc = netframe[4];
    memcpy(&frame->data, &netframe[8], 8);
}

void candump_to_can(char *s, struct can_frame *frame) {
    int i;
    unsigned int dat;
    char *candata;

    if (sscanf(s, "0x%08x  [%hhu]", &frame->can_id, &frame->can_dlc)) {
	candata = strstr(s, "]") + 1;

	for (i = 0; i < frame->can_dlc; i++) {
	    sscanf(&candata[i * 3], "%2X", &dat);
	    frame->data[i] = dat;
	}
    }
}

int print_can_frame(char *format_string, struct can_frame *frame) {
    int i;
    if (frame->can_dlc > 8) {
	printf(RED " Invalid DLC %d found\n" RESET, frame->can_dlc);
	return -1;
    }
    printf(format_string, frame->can_id & CAN_EFF_MASK, frame->can_dlc);
    if (frame->can_id & CAN_RTR_FLAG) {
	printf(" <RTR>                   ");
	return -2;
    }
    for (i = 0; i < frame->can_dlc; i++) {
	printf(" %02X", frame->data[i]);
    }
    if (frame->can_dlc < 8) {
	for (i = frame->can_dlc; i < 8; i++) {
	    printf("   ");
	}
    }
    printf(" ");
#if 1
    printf("  ");
    for (i = 0; i < frame->can_dlc; i++) {
	if (isprint(frame->data[i]))
	    printf("%c", frame->data[i]);
	else
	    putchar(46);
    }
    printf("\n");
#endif
    return 0;
}

int send_can_frame(int can_socket, struct can_frame *frame) {
    frame->can_id &= CAN_EFF_MASK;
    frame->can_id |= CAN_EFF_FLAG;
    /* send CAN frame */
    if (write(can_socket, frame, sizeof(*frame)) != sizeof(*frame)) {
        fprintf(stderr, "error writing CAN frame: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}


int main(int argc, char **argv) {
    int max_fds, opt, sc;
    struct can_frame frame;
    char pcap_file[256];
    char candump_file[256];
    char roctrc_file[256];
    struct sockaddr_can caddr;
    struct ifreq ifr;
    socklen_t caddrlen = sizeof(caddr);
    fd_set read_fds;
    char timestamp[32];

    strcpy(ifr.ifr_name, "can0");
    memset(pcap_file, 0, sizeof(pcap_file));
    memset(candump_file, 0, sizeof(candump_file));
    memset(roctrc_file, 0, sizeof(roctrc_file));

    while ((opt = getopt(argc, argv, "i:l:vh?")) != -1) {
	switch (opt) {
	case 'i':
	    strncpy(ifr.ifr_name, optarg, sizeof(ifr.ifr_name) - 1);
	    break;
	case 'l':
	    strncpy(candump_file, optarg, sizeof(candump_file) - 1);
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

    if (candump_file[0] != 0) {
	FILE *fp;
	char *line, *pos_r;
	char datum[MAXSIZE];
	size_t size = MAXSIZE;
	struct can_frame aframe;
	int hour, minute, second, milli;
	/*
	time_t rawtime;
	struct tm ts;
	*/
	unsigned int command, response, hash, dlc;

	fp = fopen(candump_file, "r");
	if (!fp) {
	    fprintf(stderr, "\ncan't open file %s for reading - error: %s\n", candump_file, strerror(errno));
	    exit(EXIT_FAILURE);
	}

	line = (char *)malloc(MAXSIZE);
	if (line == NULL) {
	    fprintf(stderr, "Unable to allocate buffer\n");
	    exit(EXIT_FAILURE);
	}

	memset(datum, 0, sizeof(datum));
	while (getline(&line, &size, fp) > 0) {
	    if (sscanf(line, "%d:%d:%d.%3d)", &hour, &minute, &second, &milli) == 4) {
		/* rawtime = time;
		   printf("# getline %s", line);
		   ts = *localtime(&rawtime);
		   strftime(datum, sizeof(datum), "%Y%m%d.%H%M%S", &ts); */
		pos_r = strstr(line, "CAN ");
		pos_r += 5;
		memset(&aframe, 0, sizeof(aframe));
		candump_to_can(pos_r, &aframe);
		printf(RESET "%s.%03d", datum, milli);
		print_can_frame(F_N_CAN_FORMAT_STRG, &aframe);
		send_can_frame(sc, &aframe);
		/* TODO */
		/* decode_frame(&aframe); */
	    }
	    if (sscanf(line, "C:%x R:%u H:%x D:%u ", &command, &response, &hash, &dlc) == 4) {
		/* int sleep = 100000; */
		memset(&aframe, 0, sizeof(aframe));
		aframe.can_id = (command << 17) | (response <<16) | hash;
		aframe.can_dlc = dlc;
		printf("ID 0x%08x DLC %u\n", aframe.can_id, aframe.can_dlc);
		for (int i=0; i < aframe.can_dlc; i++) {
		    sscanf(&line[26 + i*5], "%hhu ", &aframe.data[i]);
		}
		send_can_frame(sc, &aframe);
		print_can_frame(F_N_CAN_FORMAT_STRG, &aframe);
		usleep(100);
	    }
	}
    }

    memset(&caddr, 0, sizeof(caddr));

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
	    printf(RESET);
	    time_stamp(timestamp);
	    if (read(sc, &frame, sizeof(struct can_frame)) < 0) {
		fprintf(stderr, "error reading CAN frame: %s\n", strerror(errno));
	    } else if (frame.can_id & CAN_EFF_FLAG) {	/* only EFF frames are valid */
		printf("%s ", timestamp);
		print_can_frame(F_N_CAN_FORMAT_STRG, &frame);
		/* TODO */
		/* decode_frame(&frame);
		 */
	    } else {
		print_can_frame(F_N_SFF_FORMAT_STRG, &frame);
	    }
	    printf("\n");
	}
    }

    close(sc);
    return 0;
}
