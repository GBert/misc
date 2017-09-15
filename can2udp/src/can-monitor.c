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

#define RED	"\x1B[31m"
#define GRN	"\x1B[32m"
#define YEL	"\x1B[33m"
#define BLU	"\x1B[34m"
#define MAG	"\x1B[35m"
#define CYN	"\x1B[36m"
#define WHT	"\x1B[37m"
#define RESET	"\x1B[0m"

#define MAXDG   	4096	/* maximum datagram size */
#define MAXUDP  	16	/* maximum datagram size */
#define MAX(a,b)	((a) > (b) ? (a) : (b))

unsigned char netframe[MAXDG];

static char *F_S_CAN_FORMAT_STRG = "S CAN  0x%08X  [%d]";
static char *F_N_CAN_FORMAT_STRG = "  CAN  0x%08X  [%d]";

const char *subCmdNames[] = {
    "Stopp",
    "Go",
    "Halt",
    "Lok-Nothalt",
    "Lok Zyklus Stopp",
    "Lok Datenprotokoll",
    "Schaltzeit Zubehördecoder",
    "Fast Read für",
    "Gleisprotokoll freischalten",
    "MFX Neuanmeldezähler setzen",
    "Überlast",
    "Status",
    "Kennung"
};

void writeRed(const char *s) {
    printf(RED "%s", s);
    printf(RESET);
}

void writeGreen(const char *s) {
    printf(GRN "%s", s);
    printf(RESET);
}

void writeCyan(const char *s) {
    printf(CYN "%s\n", s);
}

void writeYellow(const char *s) {
    printf(YEL "%s\n", s);
}


void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -i <can interface>\n", prg);
    fprintf(stderr, "   Version 0.1\n\n");
    fprintf(stderr, "         -i <can int>  CAN interface - default can0\n");
    fprintf(stderr, "         -h            show this help\n\n");
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
    printf(" ");
#if 0
    printf("  ");
    for (i = 0; i < frame->can_dlc; i++) {
	if (isprint(frame->data[i]))
	    printf("%c", frame->data[i]);
	else
	    putchar(46);
    }
    printf("\n");
#endif
}

int CS1(int hash) {
    if ((hash & (1 << 7)) == 0 && (hash & (1 << 8)) != 0 && (hash & (1 << 9)) != 0)
	return 0;
    else
	return 1;
}

char *getLoco(uint8_t * data) {
    uint16_t locID = (data[2] << 8) + data[3];
    static char prot[32];
    int addrs;

    if (locID <= 0x03ff) {
	strncpy(prot, "Motorola-", sizeof(prot));
	addrs = locID;
    } else if (locID >= 0x4000 && locID < 0xC000) {
	strncpy(prot, "mfx-", sizeof(prot));
	addrs = locID - 0x4000;
    } else if (locID >= 0xC000) {
	strncpy(prot, "DCC-", sizeof(prot));
	addrs = locID - 0xC000;
    } else {
	strncpy(prot, "unbekannt-", sizeof(prot));
	addrs = 0;
    }

    sprintf(prot, "%s%d", prot, addrs);
    return prot;
}

int main(int argc, char **argv) {
    int max_fds, opt, sc, i;
    float v;
    struct can_frame frame;
    uint32_t kennung, uid;

    struct sockaddr_can caddr;
    struct ifreq ifr;
    socklen_t caddrlen = sizeof(caddr);

    fd_set read_fds;
    char dir[32];

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
	    } else if (frame.can_id & CAN_EFF_FLAG) {	/* only EFF frames are valid */
		printf(RESET);
		print_can_frame(F_N_CAN_FORMAT_STRG, &frame);
		if (frame.can_id & 0x00010000UL)
		   printf(CYN);
		else
		   printf(YEL);
		switch ((frame.can_id & 0x00FF0000UL) >> 16) {
		case 0x01:
		case 0x00:
		    for (i = 0; i < 13; i++) {
			if (frame.data[4] == i) {
			    if (i == 0 || i == 2) {
				printf("System-Befehl, Sub-Befehl: ");
				writeRed(subCmdNames[i]);
			    } else if (i == 1) {
				printf("System-Befehl, Sub-Befehl: ");
				writeGreen(subCmdNames[i]);
			    } else if (i == 3 && (frame.data[2] + frame.data[3]) == 0)
				writeRed("Nothalt an alle Loks");
			    else if (i == 3)
				printf("Lok: %s, Nothalt", getLoco(frame.data));
			    else if (i == 5)
				printf("Lok: %s, Protokollparameter: %d", getLoco(frame.data), frame.data[5]);
			    else
				printf("0x%02X %s", frame.data[4], subCmdNames[i]);
			printf("\n");
			}
		    }

		    break;
		    /* Lok Geschwindigkeit */
		case 0x09:
		case 0x08:
		    v = (frame.data[4] << 8) + frame.data[5];
		    v = v / 10;
		    printf("Lok: %s, Geschwindigkeit: %3.1f\n", getLoco(frame.data), v);
		    break;
		    /* Lok Richtung */
		case 0x0B:
		case 0x0A:
		    memset(dir, 0, sizeof(dir));

		    if (frame.can_dlc == 4)
			strncat(dir, " wird abgefragt", sizeof(dir));
		    else if (frame.data[4] == 0)
			strncat(dir, " bleibt gleich", sizeof(dir));
		    else if (frame.data[4] == 1)
			strncat(dir, ": vorwärts", sizeof(dir));
		    else if (frame.data[4] == 2)
			strncat(dir, ": rückwärts", sizeof(dir));
		    else if (frame.data[4] == 3)
			strncat(dir, " wechseln", sizeof(dir));
		    else
			strncat(dir, "unbekannt", sizeof(dir));

		    printf("Lok: %s, Richtung %s\n", getLoco(frame.data), dir);

		    break;
		case 0x0D:
		case 0x0C:
		    if (frame.can_dlc == 5)
			printf("Lok %s Funktion %d\n", getLoco(frame.data), frame.data[4]);
		    else if (frame.can_dlc == 6)
			printf("Lok %s Funktion %d Wert %d\n", getLoco(frame.data), frame.data[4], frame.data[5]);
		    else if (frame.can_dlc == 7)
			printf("Lok %s Funktion %d Wert %d Funktionswert %d\n",
				getLoco(frame.data), frame.data[4], frame.data[5], (frame.data[6] << 8) + frame.data[7]);
		    break;

		case 0x30:
		    printf("Ping Anfrage\n");
		    printf(RESET);
		    break;
		case 0x31:
		    kennung = (frame.data[6] << 8) + frame.data[7];
		    uid = ntohl(*(uint32_t *) & frame.data);
		    printf("Ping Antwort von ");
		    switch (kennung) {
		    case 0x0010:
			printf("Gleisbox");
			break;
		    case 0x0030:
		    case 0x0031:
		    case 0x0032:
		    case 0x0033:
			printf("Mobile Station 2");
			break;
		    case 0x1234:
			printf("MäCAN-Weichendecoder");
			break;
		    case 0xEEEE:
			printf("CS2 Software");
			break;
		    case 0xFFFF:
			printf("CS2-GUI (Master)");
			break;
		    default:
			printf("unbekannt");
			break;
		    }
		    printf(" mit 0x%04X UID 0x%08X, Software Version %d.%d\n", kennung, uid, frame.data[4], frame.data[5]);
		    printf(RESET);
		    break;
		case 0x36:
		    printf("Bootloader Anfrage\n");
		    break;
		case 0x37:
		    kennung = (frame.data[6] << 8) + frame.data[7];
		    uid = ntohl(*(uint32_t *) & frame.data);
		    printf("Bootloader Antwort von ");
		    switch (kennung) {
		    case 0x0010:
			printf("Gleisbox");
			break;
		    case 0x0030:
		    case 0x0033:
			printf("Mobile Station 2");
			break;
		    case 0x1234:
			printf("MäCAN-Weichendecoder");
			break;
		    case 0xEEEE:
			printf("CS2 Software");
			break;
		    case 0xFFFF:
			printf("CS2-GUI (Master)");
			break;
		    default:
			printf("unbekannt");
			break;
		    }
		    printf(" mit 0x%04X UID 0x%08X, Software Version %d.%d\n", kennung, uid, frame.data[4], frame.data[5]);
		    printf(RESET);
		    break;
		default:
		    printf("\n");
		    break;
		}
	    } else {
		print_can_frame(F_S_CAN_FORMAT_STRG, &frame);
		printf("\n");
	    }
	}
    }
    close(sc);
    return 0;
}
