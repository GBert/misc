/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 *
 */

#include <stdlib.h>
#include <stdio.h>
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
#define MAX(a,b)	((a) > (b) ? (a) : (b))

static char *F_CAN_FORMAT_STRG = "      CAN->  0x%08X   [%d]";
static char *F_S_CAN_FORMAT_STRG = "short CAN->  0x%08X   [%d]";
static char *T_CAN_FORMAT_STRG = "      CAN<-  0x%08X   [%d]";

const char *subCmdNames[] = {
    "Stopp",
    "Go",
    "Halt",
    "Lok-Nothalt",
    "Lok Zyklus Stopp",
    "Lok Datenprotokoll",
    "Schaltzeit Zubehoerdecoder",
    "Fast Read fuer",
    "Gleisprotokoll freischalten",
    "MFX Neuanmeldezaehler setzen",
    "Ueberlast",
    "Status",
    "Kennung"
};

unsigned char netframe[MAXDG];

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -i <can interface>\n", prg);
    fprintf(stderr, "   Version 0.1\n\n");
    fprintf(stderr, "         -i <can int>        can interface - default can0\n");
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

int CS1(int hash) {
    if ((hash & (1 << 7)) == 0 && (hash & (1 << 8)) != 0 && (hash & (1 << 9)) != 0)
	return 0;
    else
	return 1;
}

const char *getLoco(uint8_t *data) {
    uint16_t locID = (data[2] << 8) + data[3];
    char prot[256];
    int addrs;

    if (locID <= 0x03ff) {
	prot = "Motorola";
	addrs = locID;
    } else if (locID >= 0xC000 && locID <= 0xFFFF) {
	prot = "DCC";
	addrs = locID - 0xc000;
    } else {
	prot = "unbekannt";
	addrs = 0;
    }
    return prot + "-" + addrs;
}

int main(int argc, char **argv) {
    int max_fds, opt;
    struct can_frame frame;

    int sc;
    struct sockaddr_can caddr;
    struct ifreq ifr;
    socklen_t caddrlen = sizeof(caddr);

    fd_set read_fds;

    strcpy(ifr.ifr_name, "can0");

    while ((opt = getopt(argc, argv, "i:dh?")) != -1) {
	switch (opt) {
	case 'i':
	    strncpy(ifr.ifr_name, optarg, sizeof(ifr.ifr_name) - 1);
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
	if (select(max_fds + 1, &read_fds, NULL, NULL, NULL) < 0) {
	    fprintf(stderr, "select error: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
	/* received a CAN frame */
	if (FD_ISSET(sc, &read_fds)) {
	    if (read(sc, &frame, sizeof(struct can_frame)) < 0) {
		fprintf(stderr, "error reading CAN frame: %s\n", strerror(errno));
	    } else if ((frame.can_id & CAN_EFF_FLAG) && !(!CS1(frame.can_id & 0x1FFFFFFF))) {	/* only EFF frames are valid */
		switch ((frame.can_id & 0x00FF0000UL) >> 16) {
		case 0x00:
		    break;
		case 0x01:
		    float v = (frame.data[4] << 8) + frame.data[5];
		    v = v / 10;
		    printf("Lok: %s, Geschwindigkeit: %3.1f\n", getLoco(&frame.data), v);
		    break;
		case 0x30:
		    break;
		case 0x36:
		    break;
		case 0x40:
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
