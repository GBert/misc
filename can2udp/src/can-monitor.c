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

void INThandler(int sig) {
    signal(sig, SIG_IGN);
    fputs(RESET, stdout);
    exit(0);
}

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
    fprintf(stderr, "   Version 1.1\n\n");
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
	printf(" %02X", frame->data[i]);
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

char *getLoco(uint8_t * data, char *s) {
    uint16_t locID = (data[2] << 8) + data[3];
    char prot[32];
    int addrs;

    memset(prot, 0, sizeof(prot));

    if (locID <= 0x03ff) {
	strncpy(prot, " mm-", sizeof(prot));
	addrs = locID;
    } else if (locID >= 0x4000 && locID < 0xC000) {
	strncpy(prot, "mfx-", sizeof(prot));
	addrs = locID - 0x4000;
    } else if (locID >= 0xC000) {
	strncpy(prot, "dcc-", sizeof(prot));
	addrs = locID - 0xC000;
    } else {
	strncpy(prot, "unbekannt-", sizeof(prot));
	addrs = 0;
    }

    sprintf(s, "%s%d", prot, addrs);
    return s;
}

void command_system(struct can_frame *frame) {
    uint32_t uid, response;
    uint16_t sid, wert;
    char s[32];

    memset(s, 0, sizeof(s));
    response = frame->can_id & 0x00010000;

    /* CdB extension) */
    if (frame->can_dlc == 0) {
	printf("System-Befehl: CdB Suche\n");
	return;
    }

    uid = ntohl(*(uint32_t *) frame->data);
    switch (frame->data[4]) {
    case 0x00:
	if (uid)
	    printf("System-Befehl: UID 0x%08X ", uid);
	else
	    printf("System-Befehl: alle ");
	writeRed("Stopp");
	break;
    case 0x01:
	if (uid)
	    printf("System-Befehl: UID 0x%08X ", uid);
	else
	    printf("System-Befehl: alle ");
	writeGreen("Go");
	break;
    case 0x02:
	if (uid)
	    printf("System-Befehl: UID 0x%08X ", uid);
	else
	    printf("System-Befehl: alle ");
	writeRed("Halt");
	break;
    case 0x03:
	printf("System-Befehl: ");
	if (uid)
	    printf("Lok %s Nothalt", getLoco(frame->data, s));
	else
	    writeRed("Nothalt alle Loks");
	break;
    case 0x04:
	printf("System-Befehl: Lok %s Zyklus Ende", getLoco(frame->data, s));
	break;
    case 0x05:
	printf("System-Befehl: Lok %s Gleisprotokoll: %d", getLoco(frame->data, s), frame->data[5]);
	break;
    case 0x06:
	wert = ntohs(*(uint16_t *) &frame->data[5]);
	printf("System-Befehl: System Schaltzeit Zubehör UID 0x%08X Zeit 0x%04X", uid, wert);
	break;
    case 0x07:
	sid = ntohs(*(uint16_t *) &frame->data[5]);
	printf("System-Befehl: Fast Read mfx UID 0x%08X SID %d", uid, sid);
	break;
    case 0x08:
	printf("System-Befehl: Gleisprotokoll freischalten -");
	if (frame->data[5] & 1)
	    printf(" MM2");
	if (frame->data[5] & 2)
	    printf(" MFX");
	if (frame->data[5] & 4)
	    printf(" DCC");
	if (frame->data[5] & 8)
	    printf(" SX1");
	if (frame->data[5] & 16)
	    printf(" SX2");
	break;
    case 0x09:
	wert = ntohs(*(uint16_t *) &frame->data[5]);
	printf("System-Befehl: Neuanmeldezähler setzen UID 0x%08X Zähler 0x%04X", uid, wert);
	break;
    case 0x0a:
	printf("System-Befehl: Überlast UID 0x%08X Kanal 0x%04X", uid, frame->data[5]);
	break;
    case 0x0b:
	if (frame->can_dlc == 6)
	    printf("System-Befehl: Statusabfrage UID 0x%08X Kanal 0x%02X", uid, frame->data[5]);
	if (frame->can_dlc == 7)
	    printf("System-Befehl: Statusabfrage UID 0x%08X Kanal 0x%02X Wert %d", uid, frame->data[5], frame->data[6]);
	if (frame->can_dlc == 8) {
	    wert = ntohs(*(uint16_t *) &frame->data[6]);
	    if (response)
		printf("System-Befehl: Statusabfrage UID 0x%08X Kanal 0x%02X Messwert 0x%04X",
		       uid, frame->data[5], wert);
	    else
		printf("System-Befehl: Statusabfrage UID 0x%08X Kanal 0x%02X Konfigurationswert 0x%04X",
		       uid, frame->data[5], wert);
	}
	break;
    case 0x0c:
	wert = ntohs(*(uint16_t *) &frame->data[5]);
	if (frame->can_dlc == 6)
	    printf("System-Befehl: Statusabfrage UID 0x%08X Kanal 0x%02X", uid, frame->data[5]);
	if (frame->can_dlc == 7)
	    printf("System-Befehl: Statusabfrage UID 0x%08X Kanal 0x%02X Wert %d", uid, frame->data[5], frame->data[6]);
	break;
    case 0x20:
	printf("System-Befehl: Systemzeit UID 0x%08X Stunde %d Minute %d Faktor %d",
	       uid, frame->data[5], frame->data[6], frame->data[7]);
	break;
    case 0x30:
	printf("System-Befehl: mfx Seek 0x%08X", uid);
	break;
    case 0x80:
	printf("System-Befehl: System Reset UID 0x%08X Ziel 0x%02X", uid, frame->data[5]);
	break;
    default:
	printf("System-Befehl: unbekannt 0x%02X", frame->data[4]);
	break;
    }
    printf("\n");
}

void cdb_extension(struct can_frame *frame) {
    uint8_t kontakt, index;
    uint16_t wert;

    kontakt = *frame->data;
    index = frame->data[1];

    if (kontakt) {
	if (frame->can_dlc == 2) {
	    printf("CdB: Abfrage Kontakt %d ", kontakt);
	    switch (index) {
	    case 0x01:
		printf("Version\n");
		break;
	    case 0x02:
		printf("Einschaltverzögerung\n");
		break;
	    case 0x03:
		printf("Ausschaltverzögerung\n");
		break;
	    default:
		printf("unbekannt\n");
		break;
	    }
	}
	if (frame->can_dlc == 4) {
	    wert = ntohs(*(uint16_t *) &frame->data[2]);
	    printf("CdB: Antwort Kontakt %d ", kontakt);
	    switch (index) {
	    case 0x01:
		printf("Version %d.%d\n", frame->data[2], frame->data[3]);
		break;
	    case 0x02:
		printf("Einschaltverzögerung %d ms\n", wert);
		break;
	    case 0x03:
		printf("Ausschaltverzögerung %d ms\n", wert);
		break;
	    default:
		printf("unbekannt: %d\n", wert);
		break;
	    }
	}
    /* System */
    } else {
	if (frame->can_dlc == 2) {
	    switch (index) {
	    case 0x01:
		printf("CdB: Abfrage Version\n");
		break;
	    default:
		printf("CdB: Abfrage unbekannter Index %d\n", index);
		break;
	    }
	}
	if (frame->can_dlc == 4) {
	    switch (index) {
	    case 0x01:
		printf("CdB: Antwort Version %d.%d\n", frame->data[2], frame->data[3]);
		break;
	    default:
		wert = ntohs(*(uint16_t *) &frame->data[2]);
		printf("CdB: Antwort unbekannter Index %d Wert 0x%04X\n", index, wert);
		break;
	    }
	}
    }
}

int main(int argc, char **argv) {
    int max_fds, opt, sc;
    float v;
    struct can_frame frame;
    uint32_t kennung, function, uid, cv_number, cv_index, stream_size;
    uint16_t crc, kenner, kontakt, wert;
    char s[32];

    struct sockaddr_can caddr;
    struct ifreq ifr;
    socklen_t caddrlen = sizeof(caddr);

    fd_set read_fds;

    strcpy(ifr.ifr_name, "can0");

    signal(SIGINT, INThandler);

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
	    printf(RESET);
	    if (read(sc, &frame, sizeof(struct can_frame)) < 0) {
		fprintf(stderr, "error reading CAN frame: %s\n", strerror(errno));
	    } else if (frame.can_id & CAN_EFF_FLAG) {	/* only EFF frames are valid */
		print_can_frame(F_N_CAN_FORMAT_STRG, &frame);
		if (frame.can_id & 0x00010000UL)
		    printf(CYN);
		else
		    printf(YEL);
		switch ((frame.can_id & 0x00FF0000UL) >> 16) {
		/* System Befehle */
		case 0x00:
		case 0x01:
		    command_system(&frame);
		    break;
		/* Lok Discovery */
		case 0x02:
		case 0x03:
		    if (frame.can_dlc == 0)
			printf("Lok Discovery - Erkennen alle Protokolle\n");
		    if (frame.can_dlc == 1)
			printf("Lok Discovery - Protokoll Kennung 0x%02X\n", frame.data[0]);
		    if (frame.can_dlc == 5) {
			uid = ntohl(*(uint32_t *) frame.data);
			printf("Lok Discovery - 0x%04X Protokoll Kennung 0x%02X\n", uid, frame.data[4]);
		    }
		    break;
		/* MFX Bind */
		case 0x04:
		case 0x05:
		    if ((frame.can_dlc == 2) || (frame.can_dlc == 4))
			cdb_extension(&frame);
		    if (frame.can_dlc == 6) {
			uid = ntohl(*(uint32_t *) frame.data);
			printf("MFX Bind: MFX UID 0x%08X MFX SID %d\n", uid, (frame.data[4] << 8) + frame.data[5]);
		    }
		    break;
		/* MFX Verify */
		case 0x06:
		case 0x07:
		    uid = ntohl(*(uint32_t *) frame.data);
		    if (frame.can_dlc == 2) {
		        kenner = ntohs(*(uint16_t *) frame.data);
			if (kenner == 0x00ff)
			    printf("CdB: Reset\n");
			else
			    printf("CdB: unbekannt 0x%04x\n", kenner);
		    }
		    if (frame.can_dlc == 4) {
		        kenner = ntohs(*(uint16_t *) frame.data);
		        wert = ntohs(*(uint16_t *) &frame.data[2]);
			printf("CdB: unbekannt 0x%04x 0x%04x\n", kenner, wert);
		    }
		    if (frame.can_dlc == 6)
			printf("MFX Verify: MFX UID 0x%08X MFX SID %d\n", uid, (frame.data[4] << 8) + frame.data[5]);
		    if (frame.can_dlc == 7)
			printf("MFX Verify: MFX UID 0x%08X MFX SID %d ASK-Verhältnis %d\n",
			       uid, (frame.data[4] << 8) + frame.data[5], frame.data[6]);
		    break;
		/* Lok Geschwindigkeit */
		case 0x08:
		case 0x09:
		    v = (frame.data[4] << 8) + frame.data[5];
		    v = v / 10;
		    printf("Lok: %s, Geschwindigkeit: %3.1f\n", getLoco(frame.data, s), v);
		    break;
		/* Lok Richtung */
		case 0x0A:
		case 0x0B:
		    memset(s, 0, sizeof(s));

		    printf("Lok: %s, ", getLoco(frame.data, s));
		    if (frame.can_dlc == 4)
			strncat(s, " wird abgefragt", sizeof(s));
		    else if (frame.data[4] == 0)
			strncat(s, " bleibt gleich", sizeof(s));
		    else if (frame.data[4] == 1)
			strncat(s, ": vorwärts", sizeof(s));
		    else if (frame.data[4] == 2)
			strncat(s, ": rückwärts", sizeof(s));
		    else if (frame.data[4] == 3)
			strncat(s, " wechseln", sizeof(s));
		    else
			strncat(s, "unbekannt", sizeof(s));

		    printf("Richtung %s\n", s);

		    break;
		/* Lok Funktion */
		case 0x0C:
		case 0x0D:
		    if (frame.can_dlc == 5)
			printf("Lok %s Funktion %d\n", getLoco(frame.data, s), frame.data[4]);
		    else if (frame.can_dlc == 6)
			printf("Lok %s Funktion %d Wert %d\n", getLoco(frame.data, s), frame.data[4], frame.data[5]);
		    else if (frame.can_dlc == 7)
			printf("Lok %s Funktion %d Wert %d Funktionswert %d\n",
			       getLoco(frame.data, s), frame.data[4], frame.data[5], (frame.data[6] << 8) + frame.data[7]);
		    break;
		/* Read Config */
		case 0x0E:
		    if (frame.can_dlc == 7) {
			cv_number = ((frame.data[4] & 0x3) << 8) + frame.data[5];
			cv_index = frame.data[4] >> 2;
			printf("Read Config Lok %s CV Nummer %d Index %d Anzahl %d\n",
			       getLoco(frame.data, s), cv_number, cv_index, frame.data[6]);
		    }
		    break;
		case 0x0F:
		    cv_number = ((frame.data[4] & 0x3) << 8) + frame.data[5];
		    cv_index = frame.data[4] >> 2;
		    if (frame.can_dlc == 6)
			printf("Read Config Lok %s CV Nummer %d Index %d\n", getLoco(frame.data, s), cv_number, cv_index);
		    if (frame.can_dlc == 7)
			printf("Read Config Lok %s CV Nummer %d Index %d Wert %d\n",
			       getLoco(frame.data, s), cv_number, cv_index, frame.data[6]);
		    break;
		/* Zubehör schalten */
		case 0x16:
		case 0x17:
		    if (frame.can_dlc == 6)
			printf("Zubehör Schalten Lok %s Stellung %d Strom %d\n",
			       getLoco(frame.data, s), frame.data[4], frame.data[5]);
		    if (frame.can_dlc == 8)
			printf("Zubehör Schalten Lok %s Stellung %d Strom %d Schaltzeit/Sonderfunktionswert %d\n",
			       getLoco(frame.data, s), frame.data[4], frame.data[5],
			       (frame.data[6] << 8) + frame.data[7]);
		    break;
		/* S88 Polling */
		case 0x20:
		    uid = ntohl(*(uint32_t *) frame.data);
		    printf("S88 Polling 0x%04X Modul Anzahl %d\n", uid, frame.data[4]);
		    break;
		case 0x21:
		    uid = ntohl(*(uint32_t *) frame.data);
		    printf("S88 Polling 0x%04X Modul %d Zustand %d\n",
			   uid, frame.data[4], (frame.data[5] << 8) + frame.data[6]);
		    break;
		/* S88 Event */
		case 0x22:
		    kenner = ntohs(*(uint16_t *) frame.data);
		    kontakt = ntohs(*(uint16_t *) &frame.data[2]);
		    if (frame.can_dlc == 4)
			printf("S88 Event: Kennung %d Kontakt %d\n", kennung, kontakt);
		    if (frame.can_dlc == 5)
			printf("S88 Event: Kennung %d Kontakt %d Parameter %d\n", kenner, kontakt, frame.data[4]);
		    break;
		case 0x23:
		    kenner = ntohs(*(uint16_t *) frame.data);
		    kontakt = ntohs(*(uint16_t *) &frame.data[2]);
		    if (frame.can_dlc == 8)
			printf("S88 Event: Kennung %d Kontakt %d Zusand alt %d Zusand neu %d Zeit %d\n",
			       kenner, kontakt, frame.data[4], frame.data[5], (frame.data[6] << 8) + frame.data[7]);
		    break;
		/* SX1 Event */
		case 0x24:
		case 0x25:
		    uid = ntohl(*(uint32_t *) frame.data);
		    if (frame.can_dlc == 5)
			printf("SX1 Event: UID 0x%08X SX1-Adresse %d\n", uid, frame.data[4]);
		    if (frame.can_dlc == 5)
			printf("SX1 Event: UID 0x%08X SX1-Adresse %d Zustand %d\n", uid, frame.data[4], frame.data[5]);
		    break;
		/* Ping */
		case 0x30:
		    printf("Ping Anfrage\n");
		    break;
		case 0x31:
		    uid = ntohl(*(uint32_t *) frame.data);
		    kennung = ntohs(*(uint16_t *) &frame.data[6]);
		    printf("Ping Antwort von ");
		    switch (kennung) {
		    case 0x0010:
			printf("Gleisbox");
			break;
		    case 0x0030:
		    case 0x0031:
		    case 0x0032:
		    case 0x0033:
			printf("MS2");
			break;
		    case 0x0053:
			printf("Cg Servo");
			break;
		    case 0x0054:
			printf("Cg Rückmelder");
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
		    printf(" UID 0x%08X, Software Version %d.%d\n", uid, frame.data[4], frame.data[5]);
		    break;
		/* CAN Bootloader */
		case 0x36:
		    printf("CAN Bootloader Anfrage\n");
		    break;
		case 0x37:
		    uid = ntohl(*(uint32_t *) frame.data);
		    kennung = ntohs(*(uint16_t *) &frame.data[6]);
		    printf("Bootloader Antwort von ");
		    switch (kennung) {
		    case 0x0010:
			printf("Gleisbox");
			break;
		    case 0x0030:
		    case 0x0031:
		    case 0x0032:
		    case 0x0033:
			printf("MS2");
			break;
		    case 0x0053:
			printf("Cg Servo");
			break;
		    case 0x0054:
			printf("Cg Rückmelder");
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
		    printf(" UID 0x%08X, Software Version %d.%d\n", uid, frame.data[4], frame.data[5]);
		    break;
		/* Statusdaten Konfiguration */
		case 0x3A:
		case 0x3B:
		    uid = ntohl(*(uint32_t *) & frame.data);
		    if (frame.can_dlc == 5)
			printf("Status Daten: UID 0x%08X Index 0x%02X\n", uid, frame.data[4]);
		    if (frame.can_dlc == 6)
			printf("Status Daten: UID 0x%08X Index 0x%02X Paketanzahl %d\n", uid, frame.data[4],
			       frame.data[5]);
		    if (frame.can_dlc == 8)
			/* TODO Daten analysiert ausgeben */
			printf("Status Daten: Paket %d\n", frame.can_id & 0xFCFF);
		    break;
		/* Anfordern Config Daten */
		case 0x40:
		case 0x41:
		    memset(s, 0, sizeof(s));
		    memcpy(s, frame.data, 8);
		    printf("Anfordern Config Data: %s\n", s);
		    break;
		/* Config Data Stream */
		case 0x42:
		case 0x43:
		    stream_size = ntohs(*(uint32_t *) frame.data);
		    crc = ntohs(*(uint16_t *) &frame.data[4]);
		    if (frame.can_dlc == 6)
			printf("Config Data Stream: Länge 0x%08X CRC 0x%04X\n", stream_size, crc);
		    if (frame.can_dlc == 7)
			printf("Config Data Stream: Länge 0x%08X CRC 0x%04X (unbekannt 0x%02X)\n", stream_size, crc,
			       frame.data[6]);
		    if (frame.can_dlc == 8)
			printf("Config Data Stream: Daten\n");
		    break;
		/* Automatik schalten */
		case 0x60:
		case 0x61:
		    kenner = ntohs(*(uint16_t *) frame.data);
		    function = ntohs(*(uint16_t *) &frame.data[2]);
		    if (frame.can_dlc == 6)
			printf("Automatik schalten: ID 0x%04X Funktion 0x%04X Stellung 0x%02X Parameter 0x%02X\n",
			       kenner, function, frame.data[4], frame.data[5]);
		    if (frame.can_dlc == 8)
			printf("Automatik schalten: ID 0x%04X Funktion 0x%04X Lok %s\n", kenner, function, getLoco(&frame.data[4], s));
		    break;
		/* Blocktext zuordnen */
		case 0x62:
		case 0x63:
		    kenner = ntohs(*(uint16_t *) frame.data);
		    function = ntohs(*(uint16_t *) &frame.data[2]);
		    if (frame.can_dlc == 4)
			printf("Blocktext zuordnen: ID 0x%04X Funktion 0x%04X\n", kenner, function);
		    if (frame.can_dlc == 8)
			printf("Blocktext zuordnen: ID 0x%04X Funktion 0x%04X Lok %s\n", kenner, function, getLoco(&frame.data[4], s));
		    break;
		default:
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
