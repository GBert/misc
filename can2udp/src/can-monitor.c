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

#include "lib.h"
#include "can-monitor.h"

#define RED	"\x1B[31m"
#define GRN	"\x1B[32m"
#define YEL	"\x1B[33m"
#define BLU	"\x1B[34m"
#define MAG	"\x1B[35m"
#define CYN	"\x1B[36m"
#define WHT	"\x1B[37m"
#define RESET	"\x1B[0m"

#define	MAX_PAKETE	256
#define	MAXSIZE		256
#define MAXDG   	4096	/* maximum datagram size */
#define MAXUDP  	16	/* maximum datagram size */
#define MAX(a,b)	((a) > (b) ? (a) : (b))

#define IPHDR_LEN       (20)
/* defines for the packet type code in an ETHERNET header */
#define ETHER_TYPE_IP	 (0x0800)
#define ETHER_TYPE_8021Q (0x8100)

/* diff between CS1/CS2 */
#define M_CS2_HASH_MASK   0x00000380U
#define M_CS2_HASH_FLAG   0x00000300U

unsigned char netframe[MAXDG];

struct knoten *statusdaten = NULL;
struct knoten *messwert = NULL;
struct cs2_config_data_t config_data;

unsigned char buffer[MAX_PAKETE * 8];
int verbose = 0, kanal = 0, expconf = 0;

static char *F_N_CAN_FORMAT_STRG = "  CAN  0x%08X  [%d]";
static char *F_N_UDP_FORMAT_STRG = "  UDP  0x%08X  [%d]";
static char *F_N_TCP_FORMAT_STRG = "  TCP  0x%08X  [%d]";
static char *F_N_SFF_FORMAT_STRG = "  CAN  <S>  0x%03X  [%d]";

uint16_t be16(uint8_t *u) {
    return (u[0] << 8) | u[1];
}

uint32_t be32(uint8_t *u) {
    return (u[0] << 24) | (u[1] << 16) | (u[2] << 8) | u[3];
}

uint32_t le32(uint8_t *u) {
    return (u[3] << 24) | (u[2] << 16) | (u[1] << 8) | u[0];
}

#if 0
int insert_right(struct knoten *liste, void *element) {
    struct knoten *tmp = liste;
    struct knoten *node = calloc(1, sizeof(struct knoten));
    if (node == NULL) {
	fprintf(stderr, "calloc failed in %s: %s\n", __func__, strerror(errno));
	return -1;
    }
    while (tmp->next != NULL)
	tmp = tmp->next;
    tmp->next = node;
    tmp->daten = element;
    return 0;
}

struct messwert_t *suche_messwert(struct knoten *liste, uint64_t messwert) {
    struct knoten *tmp = liste;
    int i = 0;

    while (tmp) {
	struct messwert_t *messwert_tmp = (void *)tmp->daten;
	if (messwert_tmp->geraete_id_messwert == messwert) {
	    return messwert_tmp;
	} else {
	    i++;
	    if (i >= MAX_MESSWERTE)
		return NULL;
	    tmp = tmp->next;
	}
    }
    return NULL;
}
#endif

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

#if 0
void writeCyan(const char *s) {
    printf(CYN "%s\n", s);
}

void writeYellow(const char *s) {
    printf(YEL "%s\n", s);
}
#endif

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -i <can interface>\n", prg);
    fprintf(stderr, "   Version 3.11\n\n");
    fprintf(stderr, "         -i <can int>      CAN interface - default can0\n");
    fprintf(stderr, "         -r <pcap file>    read PCAP file instead from CAN socket\n");
    fprintf(stderr, "         -s                select only network internal frames\n");
    fprintf(stderr, "         -l <candump file> read candump file instead from CAN socket\n");
    fprintf(stderr, "         -t <rocrail file> read Rocrail file instead from CAN socket\n");
    fprintf(stderr, "         -v                verbose output for TCP/UDP and errorframes\n\n");
    fprintf(stderr, "         -x                expose config data\n\n");
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

void ascii_to_can(char *s, struct can_frame *frame) {
    int i;
    unsigned char d[13];

    for (i = 0; i < 13; i++) {
	sscanf(&s[i * 3], "%hhx", &d[i]);
    }
    frame_to_can(d, frame);
}

void slcan_to_can(char *s, struct can_frame *frame) {
    int i;
    unsigned int dat;

    sscanf(s, "T%8X%1X", &frame->can_id, (unsigned int *)&frame->can_dlc);
    memset(&frame->data, 0, 8);
    for (i = 1; i <= frame->can_dlc; i++) {
	sscanf(&s[8 + i * 2], "%2X", &dat);
	frame->data[i - 1] = dat;
    }
}

void candump_to_can(char *s, struct can_frame *frame) {
    unsigned int i, dat;
    char *candata;

    sscanf(s, "%08x", &frame->can_id);
    candata = strstr(s, "#");
    i = (unsigned int)(candata - s);
    if ((i > 5) && ((frame->can_id & CAN_ERR_FLAG) == 0))
	frame->can_id |= CAN_EFF_FLAG;
    if (candata++ == NULL)
	return;
    if (candata[0] == 'R') {
	frame->can_id |= CAN_RTR_FLAG;
	frame->can_dlc = candata[1] & 0xF;
    } else {
	for (i = 0; i < 8; i++) {
	    if (sscanf(&candata[i * 2], "%2X", &dat) < 1)
		break;
	    frame->data[i] = dat;
	}
	frame->can_dlc = i;
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
    return 0;
}

void print_ascii_data(struct can_frame *frame) {
    int i;

    printf("  '");
    for (i = 0; i < 8; i++) {
	if (isprint(frame->data[i]))
	    printf("%c", frame->data[i]);
	else
	    putchar(46);
    }
    printf("'\n");
}

#if 0
int CS1(int hash) {
    if ((hash & (1 << 7)) == 0 && (hash & (1 << 8)) != 0 && (hash & (1 << 9)) != 0)
	return 0;
    else
	return 1;
}
#endif

char *getLoco(uint8_t * data, char *s) {
    uint16_t locID = be16(&data[2]);
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
    uint8_t modul;
    char s[32];

    memset(s, 0, sizeof(s));
    response = frame->can_id & 0x00010000;

    /* CdB extension */
    if (frame->can_dlc == 0) {
	modul = frame->can_id & 0x7F;
	if (response)
	    printf("System: CdB Such-Antwort Modul %d\n", modul);
	else
	    printf("System: CdB Suche\n");
	return;
    }

    uid = be32(frame->data);
    if (frame->can_dlc == 4) {
	if (uid)
	    printf("System: UID 0x%08X ", uid);
	else
	    printf("System: alle ");
	printf("Stopp/Go-Abfrage\n");
	return;
    }
    switch (frame->data[4]) {
    case 0x00:
	if (uid)
	    printf("System: UID 0x%08X ", uid);
	else
	    printf("System: alle ");
	writeRed("Stopp");
	break;
    case 0x01:
	if (uid)
	    printf("System: UID 0x%08X ", uid);
	else
	    printf("System: alle ");
	writeGreen("Go");
	break;
    case 0x02:
	if (uid)
	    printf("System: UID 0x%08X ", uid);
	else
	    printf("System: alle ");
	writeRed("Halt");
	break;
    case 0x03:
	printf("System: ");
	if (uid)
	    printf("Lok %s Nothalt", getLoco(frame->data, s));
	else
	    writeRed("Nothalt alle Loks");
	break;
    case 0x04:
	printf("System: Lok %s Zyklus Ende", getLoco(frame->data, s));
	break;
    case 0x05:
	printf("System: Lok %s Gleisprotokoll: %d", getLoco(frame->data, s), frame->data[5]);
	break;
    case 0x06:
	wert = be16(&frame->data[5]);
	printf("System: System Schaltzeit Zubehör UID 0x%08X Zeit 0x%04X", uid, wert);
	break;
    case 0x07:
	sid = be16(&frame->data[5]);
	printf("System: Fast Read mfx UID 0x%08X SID %d", uid, sid);
	break;
    case 0x08:
	printf("System: Gleisprotokoll freischalten -");
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
	wert = be16(&frame->data[5]);
	printf("System: Neuanmeldezähler setzen UID 0x%08X Zähler 0x%04X", uid, wert);
	break;
    case 0x0a:
	printf("System: Überlast UID 0x%08X Kanal 0x%04X", uid, frame->data[5]);
	break;
    case 0x0b:
	if (frame->can_dlc == 6)
	    printf("System: Statusabfrage UID 0x%08X Kanal 0x%02X", uid, frame->data[5]);
	if (frame->can_dlc == 7) {
	    printf("System: Konfiguration UID 0x%08X Kanal 0x%02X ", uid, frame->data[5]);
	    if (frame->data[6])
		printf(" gültig(%d)", frame->data[6]);
	    else
		printf(" ungültig(%d)", frame->data[6]);
	}
	if (frame->can_dlc == 8) {
	    wert = be16(&frame->data[6]);
	    if (response)
		printf("System: Statusabfrage UID 0x%08X Kanal 0x%02X Messwert 0x%04X", uid, frame->data[5], wert);
	    else
		printf("System: Konfiguration UID 0x%08X Kanal 0x%02X Konfigurationswert 0x%04X", uid, frame->data[5], wert);
	}
	break;
    case 0x0c:
	if (frame->can_dlc == 5) {
	    printf("System: Geraetekennung UID 0x%08X", uid);
	} else {
	    wert = be16(&frame->data[5]);
	    printf("System: Geraetekennung UID 0x%08X ist 0x%04X", uid, wert);
	}
	break;
    case 0x20:
	printf("System: Systemzeit UID 0x%08X Stunde %d Minute %d Faktor %d",
	       uid, frame->data[5], frame->data[6], frame->data[7]);
	break;
    case 0x30:
	printf("System: mfx Seek 0x%08X", uid);
	break;
    case 0x80:
	printf("System: System Reset UID 0x%08X Ziel 0x%02X", uid, frame->data[5]);
	break;
    default:
	printf("System: unbekannt 0x%02X", frame->data[4]);
	break;
    }
    printf("\n");
}

void cdb_extension_grd(struct can_frame *frame) {
    uint8_t kontakt, index, modul;
    uint16_t wert;

    kontakt = *frame->data;
    index = frame->data[1];
    modul = frame->can_id & 0x7F;

    if (kontakt) {
	if (frame->can_dlc == 2) {
	    printf("CdB: Abfrage Modul %d Kontakt %d ", modul, kontakt);
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
	    wert = be16(&frame->data[2]);
	    printf("CdB: Antwort Modul %d Kontakt %d ", modul, kontakt);
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
	    printf("CdB: Abfrage ");
	    switch (index) {
	    case 0x01:
		printf("Version\n");
		break;
	    case 0x10:
		printf("Gerätekennung\n");
		break;
	    case 0x11:
		printf("Refresh CS2-Layout\n");
		break;
	    case 0x12:
		printf("Sende Master CS2\n");
		break;
	    default:
		printf("CdB: Abfrage unbekannter Index %d\n", index);
		break;
	    }
	}
	if (frame->can_dlc == 4) {
	    wert = be16(&frame->data[2]);
	    switch (index) {
	    case 0x01:
		printf("CdB: Antwort Version %d.%d\n", frame->data[2], frame->data[3]);
		break;
	    case 0x10:
		printf("CdB: Gerätekennung %d\n", wert);
		break;
	    case 0x11:
		if (wert == 1)
		    printf("CdB: Refresh CS2-Layout\n");
		else
		    printf("CdB: kein Refresh CS2-Layout\n");
		break;
	    case 0x12:
		if (wert == 1)
		    printf("CdB: Sende Master CS2\n");
		else
		    printf("CdB: kein Sende Master CS2\n");
		break;
	    default:
		printf("CdB: Antwort unbekannter Index %d Wert 0x%04X\n", index, wert);
		break;
	    }
	}
    }
}

void cdb_extension_wc(struct can_frame *frame) {
    uint8_t modul, servo;
    uint16_t wert;

    modul = frame->can_id & 0x7F;
    servo = frame->data[0];

    if (frame->can_dlc == 2) {
	if (servo) {
	    if (frame->data[1] == 0x55)
		printf("CdB: WeichenChef Abfrage Modul %d Kontakt %d\n", modul, servo);
	    else
		printf("CdB: WeichenChef Abfrage Modul %d Servo %d\n", modul, servo);
	} else {
	    if (frame->data[1] == 1)
		printf("CdB: WeichenChef Abfrage Modul %d Version\n", modul);
	    else
		printf("CdB: WeichenChef Abfrage Modul %d System %d\n", modul, frame->data[1]);
	}
    } else if (frame->can_dlc == 4) {
	wert = be16(&frame->data[2]);
	if (servo) {
	    printf("CdB: WeichenChef Modul %d ", modul);

	    switch (frame->data[1]) {
	    case 0x04:
		printf("Servo %d Position in us Schritten: %d\n", servo, wert);
		break;
	    case 0x06:
		if (wert)
		    printf("Servo %d Spannung aus\n", servo);
		else
		    printf("Servo %d Spannung ein\n", servo);
		break;
	    case 0x08:
		if (wert)
		    printf("Servo %d CdB-Meldungen\n", servo);
		else
		    printf("Servo %d keine CdB-Meldungen\n", servo);
		break;
	    case 0x09:
		printf("Ausgang %d Adresse %d ", frame->data[0], frame->data[2] + 1);
		if (frame->data[3] == 0x38)
		    printf("DCC\n");
		else if (frame->data[3] == 0x30)
		    printf("MM\n");
		else
		    printf("Protokoll unbekannt\n");
		break;
	    case 0x0A:
		printf("Ausgang %d Adresse %d ", frame->data[0], frame->data[3] + 1);
		if (frame->data[2] == 0x38)
		    printf("DCC\n");
		else if (frame->data[2] == 0x30)
		    printf("MM\n");
		else
		    printf("Protokoll unbekannt\n");
		break;
	    case 0x0B:
		printf("Ausgang %da Adresse %d ", frame->data[0], frame->data[2] + 1);
		if (frame->data[3] == 0x38)
		    printf("DCC\n");
		else if (frame->data[3] == 0x30)
		    printf("MM\n");
		else
		    printf("Protokoll unbekannt\n");
		break;
	    case 0x0C:
		printf("Ausgang %da Adresse %d ", frame->data[0], frame->data[3] + 1);
		if (frame->data[2] == 0x38)
		    printf("DCC\n");
		else if (frame->data[2] == 0x30)
		    printf("MM\n");
		else
		    printf("Protokoll unbekannt\n");
		break;
	    case 0x12:
		printf("Servo %d Funktion %d\n", servo, wert);
		break;
	    case 0x14:
		printf("Servo %d Dauer/Zeitbetrieb %d * 0,7s\n", servo, wert);
		break;
	    case 0x22:
		printf("Servo %d Speichern als Position ", servo);
		if (frame->data[3] == 0x02)
		    printf("rot\n");
		else if (frame->data[3] == 0x03)
		    printf("grün\n");
		else
		    printf("- unbekannt\n");
		break;
	    case 0x24:
		printf("Servo %d ", servo);
		switch (frame->data[3]) {
		case 0x02:
		    printf("Speichern für Weg rot - Verzögerung %d ms\n", frame->data[2]);
		    break;
		case 0x03:
		    printf("Speichern für Weg grün - Verzögerung %d ms\n", frame->data[2]);
		    break;
		case 0x0A:
		    printf("Verzögerung %d ms je Schritt\n", frame->data[2]);
		    break;
		default:
		    printf("- unbekannt\n");
		    break;
		}
		break;
	    default:
		printf("Servo %d unbekannter Wert %d\n", servo, wert);
		break;
	    }
	/* Servo 0 -> System */
	} else {
	    switch (frame->data[1]) {
	    case 0x01:
		printf("CdB: WeichenChef Modul %d Version %d.%d\n", modul, frame->data[2], frame->data[3]);
		break;
	    case 0x18:
		printf("CdB: WeichenChef Modul %d -> Modul %d \n", modul, wert);
		break;
	    case 0xFF:
		printf("CdB: WeichenChef Modul %d Reset\n", modul);
		break;
	    default:
		printf("CdB: WeichenChef Modul %d System %d\n", modul, frame->data[1]);
		break;
	    }
	}
    } else {
	printf("CdB: WeichenChef unbekannt\n");
    }
}

void cdb_extension_set_grd(struct can_frame *frame) {
    uint16_t wert;
    uint8_t index, kontakt, modul;

    modul = frame->can_id & 0x1F;
    kontakt = frame->data[0];
    index = frame->data[1];

    if (frame->can_dlc == 4) {
	wert = be16(&frame->data[2]);
	printf("CdB: Setze Modul %d ", modul);
	if (kontakt)
	    printf("Kontakt %d ", kontakt);
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
	case 0x10:
	    printf("Gerätekennung %d\n", wert);
	    break;
	case 0x11:
	    if (wert == 1)
		printf("Refresh CS2-Layout\n");
	    else
		printf("kein Refresh CS2-Layout\n");
	    break;
	case 0x12:
	    if (wert == 1)
		printf("Sende Master CS2\n");
	    else
		printf("Sende nicht Master CS2\n");
	    break;
	default:
	    printf("unbekannt: %d\n", wert);
	    break;
	}
    }
}

void decode_frame(struct can_frame *frame) {
    uint32_t id, kennung, function, uid, cv_number, cv_index;
    uint16_t paket, crc, kenner, kontakt;
    uint8_t n_kanaele, n_messwerte;
    char s[32];
    float v;

    if (frame->can_id & 0x00010000UL)
	printf(CYN);
    else
	printf(YEL);

    switch ((frame->can_id & 0x00FF0000UL) >> 16) {
    /* System Befehle */
    case 0x00:
    case 0x01:
	command_system(frame);
	break;
    /* Lok Discovery */
    case 0x02:
    case 0x03:
	if (frame->can_dlc == 0)
	    printf("Lok Discovery - Erkennen alle Protokolle");
	if (frame->can_dlc == 1)
	    printf("Lok Discovery - Protokoll Kennung 0x%02X", frame->data[0]);
	if (frame->can_dlc == 5) {
	    uid = be32(frame->data);
	    printf("Lok Discovery - 0x%04X Protokoll Kennung 0x%02X", uid, frame->data[4]);
	}
	if (frame->can_dlc == 6) {
	    uid = be32(frame->data);
	    printf("Lok Discovery - 0x%04X Range %d ASK %d", uid, frame->data[4], frame->data[5]);
	}
	printf("\n");
	break;
    /* MFX Bind */
    case 0x04:
    case 0x05:
	if ((frame->can_dlc == 2) || (frame->can_dlc == 4))
	    cdb_extension_grd(frame);
	if (frame->can_dlc == 6) {
	    uid = be32(frame->data);
	    printf("MFX Bind: MFX UID 0x%08X MFX SID %d\n", uid, be16(&frame->data[4]));
	}
	break;
    /* MFX Verify */
    case 0x06:
    case 0x07:
	uid = be32(frame->data);
	if (frame->can_dlc == 2) {
	    kenner = be16(frame->data);
	    if (kenner == 0x00ff)
		printf("CdB: Reset");
	    else
		printf("CdB: unbekannt 0x%04x", kenner);
	}
	if (frame->can_dlc == 4) {
	    cdb_extension_set_grd(frame);
	}
	if (frame->can_dlc == 6)
	    printf("MFX Verify: MFX UID 0x%08X MFX SID %d", uid, be16(&frame->data[4]));
	if (frame->can_dlc == 7)
	    printf("MFX Verify: MFX UID 0x%08X MFX SID %d ASK-Verhältnis %d",
		   uid, be16(&frame->data[4]), frame->data[6]);
	printf("\n");
	break;
    /* Lok Geschwindigkeit */
    case 0x08:
    case 0x09:
	v = be16(&frame->data[4]);
	v = v / 10;
	if (frame->can_dlc == 4)
	    printf("Lok %s Abfrage Fahrstufe", getLoco(frame->data, s));
	else if (frame->can_dlc == 6)
	    printf("Lok %s Geschwindigkeit: %3.1f", getLoco(frame->data, s), v);
	printf("\n");
	break;
    /* Lok Richtung */
    case 0x0A:
    case 0x0B:
	memset(s, 0, sizeof(s));

	printf("Lok %s ", getLoco(frame->data, s));
	if (frame->can_dlc == 4) {
	    printf(" wird abgefragt\n");
	} else if (frame->can_dlc == 5) {
	    switch (frame->data[4]) {
	    case 0:
		printf("Richtung bleibt gleich\n");
		break;
	    case 1:
		printf("Richtung: vorwärts\n");
		break;
	    case 2:
		printf("Richtung: rückwärts\n");
		break;
	    case 3:
		printf("Richtung wechseln\n");
		break;
	    default:
		printf("Richtung unbekannt\n");
		break;
	    }
	}
	break;
    /* Lok Funktion */
    case 0x0C:
    case 0x0D:
	if (frame->can_dlc == 5)
	    printf("Lok %s Funktion %d", getLoco(frame->data, s), frame->data[4]);
	else if (frame->can_dlc == 6)
	    printf("Lok %s Funktion %d Wert %d", getLoco(frame->data, s), frame->data[4], frame->data[5]);
	else if (frame->can_dlc == 7)
	    printf("Lok %s Funktion %d Wert %d Funktionswert %d",
		   getLoco(frame->data, s), frame->data[4], frame->data[5], be16(&frame->data[6]));
	printf("\n");
	break;
    /* Read Config */
    case 0x0E:
	if (frame->can_dlc == 7) {
	    cv_number = ((frame->data[4] & 0x3) << 8) + frame->data[5];
	    cv_index = frame->data[4] >> 2;
	    printf("Read Config Lok %s CV Nummer %u Index %u Anzahl %u",
		   getLoco(frame->data, s), cv_number, cv_index, frame->data[6]);
	}
	printf("\n");
	break;
    case 0x0F:
	cv_number = ((frame->data[4] & 0x3) << 8) + frame->data[5];
	cv_index = frame->data[4] >> 2;
	if (frame->can_dlc == 6)
	    printf("Read Config Lok %s CV Nummer %u Index %u", getLoco(frame->data, s), cv_number, cv_index);
	if (frame->can_dlc == 7)
	    printf("Read Config Lok %s CV Nummer %u Index %u Wert %u",
		   getLoco(frame->data, s), cv_number, cv_index, frame->data[6]);
	printf("\n");
	break;
    /* Write Config */
    case 0x10:
    case 0x11:
	/* TODO */
	cv_number = ((frame->data[4] & 0x3) << 8) + frame->data[5];
	cv_index = frame->data[4] >> 2;
	if (frame->can_dlc == 8)
	    printf("Write Config Lok %s CV Nummer %u Index %u Wert %u Ctrl 0x%02X\n", getLoco(frame->data, s),
		   cv_number, cv_index, frame->data[6], frame->data[7]);
	else
	    printf("Write Config Lok %s Befehl unbekannt\n", getLoco(frame->data, s));
	break;
    /* Zubehör schalten */
    case 0x16:
    case 0x17:
	uid = be32(frame->data);
	if (frame->can_dlc >= 6) {
	    if ((uid > 0x2FFF) && (uid < 0x3400))
		printf("Magnetartikel MM1 ID %u Ausgang %u Strom %u", uid - 0x2FFF, frame->data[4], frame->data[5]);
	    else if ((uid > 0x37FF) && (uid < 0x4000))
		printf("Magnetartikel DCC ID %u Ausgang %u Strom %u", uid - 0x37FF, frame->data[4], frame->data[5]);
	    else
		printf("Magnetartikel ID 0x%08X Ausgang %u Strom %u", uid, frame->data[4], frame->data[5]);
	}
	if (frame->can_dlc == 8)
	    printf(" Schaltzeit/Sonderfunktionswert %u", be16(&frame->data[6]));
	printf("\n");
	break;
    /* S88 Polling */
    case 0x20:
	uid = be32(frame->data);
	printf("S88 Polling 0x%04X Modul Anzahl %d\n", uid, frame->data[4]);
	break;
    case 0x21:
	uid = be32(frame->data);
	printf("S88 Polling 0x%04X Modul %d Zustand %d\n", uid, frame->data[4], be16(&frame->data[5]));
	break;
    /* S88 Event */
    case 0x22:
	/* TODO: Parameter */
	kenner = be16(frame->data);
	kontakt = be16(&frame->data[2]);
	if (frame->can_dlc == 4)
	    printf("S88 Event Kennung %d Kontakt %d", kenner, kontakt);
	else if (frame->can_dlc == 5)
	    printf("S88 Event Kennung %d Kontakt %d Parameter %d", kenner, kontakt, frame->data[4]);
	else if (frame->can_dlc == 7)
	    printf("S88 Event Blockmodus Kennung %d Kontakt Start %d Parameter %d", kenner, kontakt, frame->data[6]);
	printf("\n");
	break;
    case 0x23:
	kenner = be16(frame->data);
	kontakt = be16(&frame->data[2]);
	if (frame->can_dlc == 8)
	    printf("S88 Event Kennung %d Kontakt %d Zustand alt %d Zustand neu %d Zeit %d",
		   kenner, kontakt, frame->data[4], frame->data[5], be16(&frame->data[6]));
	printf("\n");
	break;
    /* SX1 Event */
    case 0x24:
    case 0x25:
	uid = be32(frame->data);
	if (frame->can_dlc == 5)
	    printf("SX1 Event UID 0x%08X SX1-Adresse %d", uid, frame->data[4]);
	if (frame->can_dlc == 5)
	    printf("SX1 Event UID 0x%08X SX1-Adresse %d Zustand %d", uid, frame->data[4], frame->data[5]);
	printf("\n");
	break;
    /* Ping */
    case 0x30:
	printf("Ping Anfrage\n");
	break;
    case 0x31:
	uid = be32(frame->data);
	kennung = be16(&frame->data[6]);
	printf("Ping Antwort von ");
	switch (kennung) {
	case 0x0000:
	    if ((uid & 0xff000000) == 0x42000000)
		printf("Booster (6017x)");
	    else
		printf("GFP");
	    break;
	case 0x0010:
	case 0x0011:
	    printf("Gleisbox");
	    break;
	case 0x0020:
	    printf("Connect6021");
	    break;
	case 0x0030:
	case 0x0031:
	case 0x0032:
	case 0x0033:
	    printf("MS2");
	    break;
	case 0x0040:
	    if ((uid & 0xFFF00000) == 0x53300000)
		printf("LinkS88");
	    else
		printf("S88 Gateway");
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
	printf(" UID 0x%08X, Software Version %d.%d\n", uid, frame->data[4], frame->data[5]);
	break;
    /* CAN Bootloader */
    case 0x36:
	printf("CAN Bootloader");
	uid = be32(frame->data);
	if (frame->can_dlc == 5) {
	    if (uid)
		printf(" UID 0x%08X", uid);
	    else
		printf(" alle");
	    switch (frame->data[4]) {
	    case 0x11:
		printf(" Go");
		break;
	    case 0x44:
		printf(" Block %d", frame->data[5]);
		break;
	    case 0xE4:
		printf(" ?");
		break;
	    default:
		printf(" ???");
		break;
	    }
	} else if (frame->can_dlc == 6) {
	    printf(" Anfrage Block %d", frame->data[5]);
	} else if (frame->can_dlc == 7) {
	    printf(" Block CRC 0x%04X", be16(&frame->data[5]));
	} else if (frame->can_dlc == 8) {
	    printf(" Data");
	} else {
	    printf(" Anfrage");
	}
	printf("\n");
	break;
    case 0x37:
	uid = be32(frame->data);
	if (frame->can_dlc == 5) {
	    if (frame->data[4] == 0x88)
		printf("CAN Bootloader Antwort ACK\n");
	    if (((frame->data[4] & 0xf0) == 0xf0) && ((frame->data[4] & 0x0f) <= 4))
		printf("CAN Bootloader Error %d\n", frame->data[4] & 0xf0);
	    break;
	}
	if (frame->can_dlc == 6) {
	    printf("CAN Bootloader Antwort Block %d\n", frame->data[5]);
	    break;
	}
	kennung = be16(&frame->data[6]);
	printf("Bootloader Antwort von ");
	switch (kennung) {
	case 0x0000:
	    if ((uid & 0xff000000) == 0x42000000)
		printf("Booster (6017x)");
	    else
		printf("GFP");
	    break;
	case 0x0010:
	    printf("Gleisbox");
	    break;
	case 0x0020:
	    printf("Connect6021");
	    break;
	case 0x0030:
	case 0x0031:
	case 0x0032:
	case 0x0033:
	    printf("MS2");
	    break;
	case 0x0040:
	    if ((uid & 0xFFF00000) == 0x53300000)
		printf("LinkS88");
	    else
		printf("S88 Gateway");
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
	printf(" UID 0x%08X, Software Version %d.%d\n", uid, frame->data[4], frame->data[5]);
	break;
    /* Statusdaten Konfiguration */
    case 0x3A:
    case 0x3B:
	/* TODO Daten analysiert ausgeben */
	uid = be32(frame->data);
	if (frame->can_dlc == 5) {
	    kanal = frame->data[4];
	    printf("Statusdaten: UID 0x%08X Index 0x%02X\n", uid, kanal);
	    /* Datensatz ist komplett übertragen */
	    if (frame->can_id & 0x00010000UL) {
	    }
	}
	if (frame->can_dlc == 6)
	    printf("Statusdaten: UID 0x%08X Index 0x%02X Paketanzahl %d\n", uid, frame->data[4], frame->data[5]);
	if (frame->can_dlc == 8) {
	    paket = (frame->can_id & 0xFCFF) - 1;
	    printf("Statusdaten: Paket %d ", paket);
	    if (paket == 0)
		memset(buffer, 0, sizeof(buffer));
	    if (paket < MAX_PAKETE)
		memcpy(&buffer[paket * 8], frame->data, 8);
	    if ((kanal == 0) && (paket == 0)) {
		n_messwerte = frame->data[0];
		n_kanaele = frame->data[1];
		id = be32(&frame->data[4]);
		printf(" Anzahl Messwerte: %d Anzahl Kanäle: %d Gerätenummer: 0x%08x", n_messwerte, n_kanaele, id);
	    } else
		for (int i = 0; i < 8; i++) {
		    if (isprint(frame->data[i]))
			putchar(frame->data[i]);
		    else
			putchar(' ');
		}
	    printf("\n");
	}
	break;
    /* Anfordern Config Daten */
    case 0x41:
	if (config_data.deflated_data && (config_data.deflated_size_counter < config_data.deflated_size))
	    printf("[Config Data %s unvollständig] ", config_data.name);
	memset(config_data.name, 0, sizeof(config_data.name));
	memcpy(config_data.name, frame->data, 8);
	/* falls through */
    case 0x40:
	memset(s, 0, sizeof(s));
	memcpy(s, frame->data, frame->can_dlc);
	/* WeichenChef Erweiterung */
	if ((frame->can_id & 0x00FEFFFE) == 0x00404A80) {
	    if (frame->can_dlc == 8) {
		int i;
		printf("CdB: Weichenchef");
		for (i = 0; i < 4; i++) {
		    printf(" Adresse %d", frame->data[i * 2 + 1] + 1);
		    if (frame->data[i * 2] == 0x30)
			printf("MM");
		    else if (frame->data[i * 2] == 0x38)
			printf("DCC");
		}
		printf("\n");
	    } else {
		printf("CdB: Abfrage Weichenchef\n");
	    }
	} else {
	    printf("Anfordern Config Data: %s\n", s);
	}
	break;
    /* Config Data Stream */
    case 0x42:
    case 0x43:
	switch (frame->can_dlc) {
	case 6:
	    config_data.deflated_size = be32(frame->data);
	    config_data.crc = be16(&frame->data[4]);
	    if (config_data.deflated_data)
		free(config_data.deflated_data);
	    config_data.deflated_data = malloc((size_t)config_data.deflated_size + 8);
	    config_data.deflated_size_counter = 0;
	    printf("Config Data Stream: Länge 0x%08X CRC 0x%04X\n", config_data.deflated_size, config_data.crc);
	    break;
	case 7:
	    config_data.deflated_size = be32(frame->data);
	    config_data.crc = be16(&frame->data[4]);
	    if (config_data.deflated_data)
		free(config_data.deflated_data);
	    config_data.deflated_data = malloc((size_t)config_data.deflated_size + 8);
	    config_data.deflated_size_counter = 0;
	    printf("Config Data Stream: Länge 0x%08X CRC 0x%04X (unbekannt 0x%02X)\n",
		   config_data.deflated_size, config_data.crc, frame->data[6]);
	    break;
	case 8:
	    if (config_data.deflated_size_counter < config_data.deflated_size) {
		memcpy(config_data.deflated_data + config_data.deflated_size_counter, frame->data, 8);
		config_data.deflated_size_counter += 8;
	    }
	    printf("Config Data Stream: Daten (%d/%d)\n", config_data.deflated_size_counter, config_data.deflated_size);
	    if (config_data.deflated_size_counter >= config_data.deflated_size) {
		crc = CRCCCITT(config_data.deflated_data, config_data.deflated_size_counter, 0xFFFF);
		if (crc == config_data.crc) {
		    printf(GRN "Config Data %s mit CRC 0x%04X, Länge %d, ",
			   config_data.name, config_data.crc, config_data.deflated_size);
		    if (config_data.deflated_data[0] == 0) {
			config_data.inflated_size = ntohl(*(uint32_t *) config_data.deflated_data);
			printf("inflated %d Bytes\n", config_data.inflated_size);
			/* TODO: now you can inflate collected data */
		    } else {
			printf("unkomprimiert\n");
			if (expconf)
			    printf(RESET "%s", config_data.deflated_data);
		    }
		} else {
		    printf(RED "Config Data %s mit ungültigem CRC 0x%04X, erwartet 0x%04X\n",
			   config_data.name, crc, config_data.crc);
		}
		free(config_data.deflated_data);
		config_data.deflated_data = NULL;
	    }
	    break;
	default:
	    printf("Data Stream mit unerwartetem DLC %d\n", frame->can_dlc);
	}
	break;
    /* 6021 or CdB WeichenChef */
    case 0x44:
    case 0x45:
	if (frame->can_dlc == 6) {
	    uid = be32(frame->data);
	    kenner = be16(&frame->data[4]);
	    printf("Connect6021 UID 0x%08X mit Kenner 0x%04X\n", uid, kenner);
	} else {
	    cdb_extension_wc(frame);
	}
	break;
    /* Automatik schalten */
    case 0x60:
    case 0x61:
	kenner = be16(frame->data);
	function = be16(&frame->data[2]);
	if (frame->can_dlc == 6)
	    printf("Automatik schalten: ID 0x%04X Funktion 0x%04X Stellung 0x%02X Parameter 0x%02X\n",
		   kenner, function, frame->data[4], frame->data[5]);
	if (frame->can_dlc == 8)
	    printf("Automatik schalten: ID 0x%04X Funktion 0x%04X Lok %s\n", kenner, function,
		   getLoco(&frame->data[4], s));
	break;
    /* Blocktext zuordnen */
    case 0x62:
    case 0x63:
	kenner = be16(frame->data);
	function = be16(&frame->data[2]);
	if (frame->can_dlc == 4)
	    printf("Blocktext zuordnen: ID 0x%04X Funktion 0x%04X\n", kenner, function);
	if (frame->can_dlc == 8)
	    printf("Blocktext zuordnen: ID 0x%04X Funktion 0x%04X Lok %s\n", kenner, function, getLoco(&frame->data[4], s));
	break;
    case 0x64:
    case 0x65:
	printf("Debug\n");
	break;
    case 0x84:
	printf("Debug Text");
	print_ascii_data(frame);
	break;
    default:
	printf("unknown\n");
	break;
    }
}

int check_cs1_frame(uint32_t id) {
    if ((id & M_CS2_HASH_MASK) == M_CS2_HASH_FLAG)
	return 0;
    if (id & 0x1C000080)
	return 1;
    return 0;
}

/*  ID field for MS1, coding for normal operation and for detection:
    28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
    <-PRIO-> <------          OBJECTHANDLE           ------> <-CMND-> <---    NODE    --->
    <-PRIO-> <---      UID      ---> <-STEP-> <--  MID   --> <-CMND-> <---    NODE    --->
*/
void decode_frame_cs1(struct can_frame *frame) {
    uint8_t mid;
    uint8_t stage;
    uint8_t id;
    uint16_t objhandle = (frame->can_id >> 10) & 0xffff;
    uint8_t node = frame->can_id & 0x7f;

    if (((node == 126) && !(frame->can_id & 0x80)) || (node & 1))
	printf("[MS1] Slave  Node %u ", node);
    else
	printf("[MS1] Master Node %u ", node);
    if ((frame->can_id & 0x1FFFFFFF) < 0x1C000000)
	printf("OH %u ", objhandle);
    switch (frame->can_id & 0x1C000380) {
    case 0x00000380:		// Prio 000, Cmd 111
	printf("Abfrage Bus belegt");
	break;
    case 0x08000100:		// Prio 010, Cmd 010
	printf("Änderung FKT %u auf %u", frame->data[0], frame->data[2]);
	break;
    case 0x0C000380:		// Prio 011, Cmd 111
	printf("Ping an Node %u", frame->data[3]);
	break;
    case 0x10000100:		// Prio 100, Cmd 010
	printf("Statusänderung");	// TODO: add parameter
	break;
    case 0x14000000:		// Prio 101, Cmd 000
    case 0x18000000:		// Prio 110, Cmd 000
	printf("Lok-Zuordnung");	// TODO: add parameter
	break;
    case 0x18000080:		// Prio 110, Cmd 001
	switch (frame->data[0]) {
	case 0x02:
	    if (frame->can_dlc < 5)
		printf("Abfrage Namesteil[%02u]", frame->data[3]);
	    else {
		printf("Namesteil[%02u]: " GRN, frame->data[3]);
		for (int i = 4; i < frame->can_dlc; i++) {
		    if (isprint(frame->data[i]))
			printf("%c", frame->data[i]);
		    else
			putchar(46);
		}
		printf(RESET);
	    }
	    break;
	case 0x40:
	    if (frame->data[1] == 3) {
		printf("Loktyp");
		if (frame->can_dlc < 6)
		    printf("-Abfrage");
		else
		    printf(" ist %u", frame->data[5]);
	    } else {
		printf("Schienenformat %u", frame->data[2]);
		if (frame->can_dlc < 6)
		    printf(" Abfrage");
		else
		    printf(" ist %u", frame->data[5]);
	    }
	    break;
	case 0x41:
	    printf("Lokstackgröße");	// TODO: add parameter
	    break;
	}
	break;
    case 0x18000100:		// Prio 110, Cmd 010
	switch (frame->can_dlc) {
	case 2:
	    if (frame->data[1])
		printf("Typabfrage");
	    else
		printf("Zustandsabfrage");
	    printf(" von FKT %u", frame->data[0]);
	    break;
	case 3:
	    if (frame->data[1])
		printf("Typ");
	    else
		printf("Zustand");
	    printf(" von FKT %u ist %u", frame->data[0], frame->data[2]);
	    break;
	case 4:
	    if (frame->data[1] == 2)
		printf("Erw. Abfrage");
	    else
		printf("Typbeschreibung von FKT %u", frame->data[0]);
	    break;
	default:
	    printf("Erw. Funktion");	// TODO: clarify whats behind
	}
	break;
    case 0x18000180:		// Prio 110, Cmd 011
	switch (frame->data[0]) {
	case 0x03:
	    if (frame->can_dlc < 8)
		printf("System-Handle-Anforderung für Node %u", frame->data[1]);
	    else
		printf("System-Handle für Node %u ist %u", frame->data[1], be16(frame->data + 4));
	    break;
	case 0x40:
	    printf("SH-handle");	// TODO: add parameter
	    break;
	case 0x80:
	    if (frame->can_dlc < 8)
		printf("Lokstack-Anfrage über %u", be16(frame->data + 2));
	    else
		printf("neuer Lokstack-Eintrag ist %u", be16(frame->data + 4));
	    break;
	default:
	    printf("Data0 %02X unbekannt", frame->data[0]);
	}
	break;
    case 0x18000200:		// Prio 110, Cmd 100
	switch (frame->data[0]) {
	case 0x40:
	    if (frame->can_dlc < 8)
		printf("SD-Handle-Anforderung für Node %u", frame->data[1]);
	    else
		printf("SD-Handle für Node %u ist %u", frame->data[1], be16(frame->data + 4));
	    break;
	case 0x80:
	    printf("Lokstack erweitern für Node %u mit Index %u", frame->data[1], be16(frame->data + 4));
	    break;
	}
	break;
    case 0x18000280:		// Prio 110, Cmd 101
	printf("Lokstackeintrag löschen mit Index %u", be16(frame->data + 4));
	break;
    case 0x1C000000:		// Prio 111, Cmd 00X
    case 0x1C000080:
	id    = (objhandle >> 8) & 0xff;
	stage = (objhandle >> 5) & 0x07;
	mid   = objhandle & 0x1f;
	printf("Anmeldung MID %u Stage %u ID %02X", mid, stage, id);
	if (frame->can_dlc == 8)
	    switch (stage) {
	    case 4:
		printf(" -> UID %08X", be32(frame->data));
		break;
	    case 7:
		if (frame->can_id & 0x80)
		    printf(" -> UID %08X OH %u Node %u", be32(frame->data), be16(frame->data + 4), (frame->data[6] & 0x7F));
		else
		    printf(" -> AP Version %u.%u", frame->data[4], frame->data[5]);
		break;
	    }
	break;
    default:
	printf("<message is still not decoded>");
    }
    printf("\n");
}

void analyze_frame(struct can_frame *frame) {
    if (frame->can_id & CAN_EFF_FLAG) {	/* decode only EFF frames */
	print_can_frame(F_N_CAN_FORMAT_STRG, frame);
	if (check_cs1_frame(frame->can_id))
	    decode_frame_cs1(frame);
	else
	    decode_frame(frame);
    } else {
	if (frame->can_id & CAN_ERR_FLAG) {
	    print_can_frame(F_N_CAN_FORMAT_STRG, frame);
	    printf(RED "*** ERRORFRAME ***" RESET);
	    if (verbose) {
		char buf[CL_LONGCFSZ];
		snprintf_can_error_frame(buf, sizeof(buf), (struct canfd_frame *)frame, "\n\t");
		printf("\n\t%s", buf);
	    }
	} else {
	    print_can_frame(F_N_SFF_FORMAT_STRG, frame);
	}
	printf("\n");
    }
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
    int selint = 0;

    strcpy(ifr.ifr_name, "can0");
    memset(pcap_file, 0, sizeof(pcap_file));
    memset(candump_file, 0, sizeof(candump_file));
    memset(roctrc_file, 0, sizeof(roctrc_file));

    signal(SIGINT, INThandler);

    while ((opt = getopt(argc, argv, "i:r:l:t:svxh?")) != -1) {
	switch (opt) {
	case 'i':
	    strncpy(ifr.ifr_name, optarg, sizeof(ifr.ifr_name) - 1);
	    break;
	case 'r':
	    strncpy(pcap_file, optarg, sizeof(pcap_file) - 1);
	    break;
	case 'l':
	    strncpy(candump_file, optarg, sizeof(candump_file) - 1);
	    break;
	case 't':
	    strncpy(roctrc_file, optarg, sizeof(roctrc_file) - 1);
	    break;
	case 's':
	    selint = 1;
	    break;
	case 'v':
	    verbose = 1;
	    break;
	case 'x':
	    expconf = 1;
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
    /* do we have a candump file ? */
    if (candump_file[0] != 0) {
	FILE *fp;
	char *line, *pos_r;
	char datum[MAXSIZE];
	size_t size = MAXSIZE;
	struct can_frame aframe;
	int time, milli;
	time_t rawtime;
	struct tm ts;

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
	    if (sscanf(line, "(%d.%3d)", &time, &milli) == 2) {
		rawtime = time;
		ts = *localtime(&rawtime);
		strftime(datum, sizeof(datum), "%Y%m%d.%H%M%S", &ts);
		pos_r = strstr(line, "can");
		memset(&aframe, 0, sizeof(aframe));
		candump_to_can(pos_r + 5, &aframe);
		printf(RESET "%s.%03d  %.5s", datum, milli, pos_r - 1);
		analyze_frame(&aframe);
	    }
	}
	printf(RESET);
	return (EXIT_SUCCESS);
    }

    /* do we have a Rocrail trace file ? */
    if (roctrc_file[0] != 0) {
	FILE *fp;
	char *line;
	char can_string[MAXSIZE];
	char datum[MAXSIZE];
	size_t size = MAXSIZE;
	char *pos_r, *pos_w, *pos_0;
	struct can_frame aframe;
	int date, time, milli, slcan_format = 0;

	fp = fopen(roctrc_file, "r");
	if (!fp) {
	    fprintf(stderr, "\ncan't open file %s for reading - error: %s\n", roctrc_file, strerror(errno));
	    exit(EXIT_FAILURE);
	}

	line = (char *)malloc(MAXSIZE);
	if (line == NULL) {
	    fprintf(stderr, "Unable to allocate buffer\n");
	    exit(EXIT_FAILURE);
	}

	memset(datum, 0, sizeof(datum));
	while (getline(&line, &size, fp) > 0) {
	    /* line[strcspn(line, "\r\n")] = 0; */
	    memset(can_string, 0, sizeof(can_string));
	    pos_0 = strstr(line, "00000000: ");
	    if ((sscanf(line, "%8d.%6d.%3d", &date, &time, &milli)) == 3) {
		sscanf(line, "%19s ", datum);
	    }
	    pos_r = strstr(line, "ASCII read: ");
	    if (pos_r) {
		slcan_format = 1;
		sscanf(pos_r, "ASCII read: %27s", can_string);
	    }
	    pos_w = strstr(line, "ASCII write: ");
	    if (pos_w) {
		slcan_format = 1;
		sscanf(pos_w, "ASCII write: %27s", can_string);
	    }
	    if (pos_r || pos_w) {
		memset(&aframe, 0, sizeof(aframe));
		slcan_to_can(can_string, &aframe);
		printf(RESET "%s %30s", datum, can_string);
		print_can_frame(F_N_CAN_FORMAT_STRG, &aframe);
		decode_frame(&aframe);
	    }
	    if ((slcan_format == 0) && pos_0) {
		sscanf(pos_0, "00000000: %39c", can_string);
		memset(&aframe, 0, sizeof(aframe));
		ascii_to_can(can_string, &aframe);
		printf(RESET "%s  %30s", datum, can_string);
		print_can_frame(F_N_CAN_FORMAT_STRG, &aframe);
		decode_frame(&aframe);
	    }
	}
	return (EXIT_SUCCESS);
    }

    /* do we have a PCAP file ? */
    if (pcap_file[0] != 0) {
	unsigned int pkt_counter = 1;
	struct tcphdr *mytcp;
	struct udphdr *myudp;
	pcap_t *handle;
	char errbuf[PCAP_ERRBUF_SIZE];
	const unsigned char *packet;
	struct pcap_pkthdr header;
	struct ip *ip_hdr;
	struct tm *tm;
	uint16_t sport, dport;
	memset(timestamp, 0, sizeof(timestamp));

	handle = pcap_open_offline(pcap_file, errbuf);
	if (handle == NULL) {
	    fprintf(stderr, "Couldn't open pcap file %s: %s\n", pcap_file, errbuf);
	    return (EXIT_FAILURE);
	}
	int caplinktype = pcap_datalink(handle);
	if (verbose)
	    printf("Analyzing file %s with capture link type %d\n", pcap_file, caplinktype);

	while ((packet = pcap_next(handle, &header)) != NULL) {
	    pkt_counter++;
	    /* header contains information about the packet (e.g. timestamp) */
	    /* cast a pointer to the packet data */
	    unsigned char *pkt_ptr = (u_char *) packet;
	    tm = localtime(&header.ts.tv_sec);
	    sprintf(timestamp, "%02d:%02d:%02d.%03d", tm->tm_hour, tm->tm_min, tm->tm_sec,
		    (int)header.ts.tv_usec / 1000);

	    int ether_offset = 4;
	    if (caplinktype != DLT_NULL) {	/* skip for loopback encapsulation */
		/* parse the first (ethernet) header, grabbing the type field */
		ether_offset = (caplinktype == DLT_LINUX_SLL) ? 14 : 12;
		int ether_type = be16(&pkt_ptr[ether_offset]);

		if (ether_type == ETHER_TYPE_IP) {	/* most common */
		    ether_offset += 2;
		} else if (ether_type == ETHER_TYPE_8021Q) {	/* dot1q tag ? */
		    ether_offset += 6;
		} else if (ether_type == 0x000C) {	/* CAN ? */
		    ether_offset = 0;
		} else {
		    if (verbose)
			fprintf(stderr, "Unknown ethernet type, %04X, skipping...\n", ether_type);
		    continue;
		}
	    }

	    if (ether_offset == 0) {
		if (be16(pkt_ptr) == 0x0001) {
		    canframe_to_can(&pkt_ptr[16], &frame);
		    printf("%s ", timestamp);
		    print_can_frame(F_N_CAN_FORMAT_STRG, &frame);
		    decode_frame(&frame);
		    printf(RESET);
		}
		continue;
	    }

	    /* skip past the Ethernet II header */
	    pkt_ptr += ether_offset;
	    /* point to an IP header structure  */
	    ip_hdr = (struct ip *)pkt_ptr;

	    /* take only frames with source and destination in the same network */
	    if (selint && ((ip_hdr->ip_src.s_addr ^ ip_hdr->ip_dst.s_addr) & 0xFF) &&
		(ip_hdr->ip_dst.s_addr != 0xFFFFFFFF))
		continue;
	    int packet_length = ntohs(ip_hdr->ip_len);

	    if (ip_hdr->ip_p == IPPROTO_UDP) {
		myudp = (struct udphdr *)(pkt_ptr + sizeof(struct ip));
		dport = ntohs(myudp->uh_dport);
		sport = ntohs(myudp->uh_sport);
		if ((dport != 15730) && (sport != 15730) && (dport != 15731) && (sport != 15731))
		    continue;
		int size_payload = packet_length - (IPHDR_LEN + sizeof(struct udphdr));
		if (verbose) {
		    printf("%s ", timestamp);
		    printf("%04u UDP %s -> ", pkt_counter, inet_ntoa(ip_hdr->ip_src));
		    printf("%s port %d -> %d", inet_ntoa(ip_hdr->ip_dst), ntohs(myudp->uh_sport), ntohs(myudp->uh_dport));
		    printf("  packet_length %d\n", size_payload);
		}
		unsigned char *dump = (unsigned char *)pkt_ptr + IPHDR_LEN + sizeof(struct udphdr);
		for (int i = 0; i < size_payload; i += 13) {
		    printf("%s %.3d>", timestamp, (ip_hdr->ip_src.s_addr) >> 24);
		    frame_to_can(dump + i, &frame);
		    print_can_frame(F_N_UDP_FORMAT_STRG, &frame);
		    if (check_cs1_frame(frame.can_id))
			decode_frame_cs1(&frame);
		    else
			decode_frame(&frame);
		    printf(RESET);
		}
	    }
	    if (ip_hdr->ip_p == IPPROTO_TCP) {
		mytcp = (struct tcphdr *)(pkt_ptr + sizeof(struct ip));
		int tcp_offset = mytcp->th_off * 4;
		int size_payload = packet_length - (IPHDR_LEN + tcp_offset);
		unsigned char *dump = (unsigned char *)pkt_ptr + IPHDR_LEN + tcp_offset;

		/* look for HTTP */
		if ((ntohs(mytcp->th_dport) == 80) || (ntohs(mytcp->th_sport) == 80)) {
		    if (size_payload) {
			if (verbose) {
			    printf("%s ", timestamp);
			    printf("%04u HTTP %s -> ", pkt_counter, inet_ntoa(ip_hdr->ip_src));
			    printf("%s port %d -> %d", inet_ntoa(ip_hdr->ip_dst), ntohs(mytcp->th_sport), ntohs(mytcp->th_dport));
			    printf("  packet_length %d\n", size_payload);
			}
			printf("%s %.3d>  HTTP    -> ", timestamp, (ip_hdr->ip_src.s_addr) >> 24);
			for (int i = 0; i < size_payload; i++)
			    putchar(dump[i]);
			if (dump[size_payload - 1] != '\n')
			    putchar('\n');
		    }
		    continue;
		}
		dport = ntohs(mytcp->th_dport);
		sport = ntohs(mytcp->th_sport);
		if ((dport != 15730) && (sport != 15730) &&
		    (dport != 15731) && (sport != 15731) && (dport != 15732) && (sport != 15732))
		    continue;
		if (size_payload > 0) {
		    if (verbose) {
			printf("%s ", timestamp);
			printf("%04u TCP %s -> ", pkt_counter, inet_ntoa(ip_hdr->ip_src));
			printf("%s port %d -> %d", inet_ntoa(ip_hdr->ip_dst), ntohs(mytcp->th_sport),
			       ntohs(mytcp->th_dport));
			printf("  packet_length %d\n", size_payload);
		    }
		    for (int i = 0; i < size_payload; i += 13) {
			printf("%s %.3d>", timestamp, (ip_hdr->ip_src.s_addr) >> 24);
			frame_to_can(dump + i, &frame);
			print_can_frame(F_N_TCP_FORMAT_STRG, &frame);
			if (check_cs1_frame(frame.can_id))
			    decode_frame_cs1(&frame);
			else
			    decode_frame(&frame);
			/* print_content(dump, size_payload); */
			printf(RESET);
		    }
		}
	    }
	    printf(RESET);
	}
	return (EXIT_SUCCESS);
    /* reading from CAN socket */
    } else {

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

	can_err_mask_t err_mask = CAN_ERR_MASK;
	if (setsockopt(sc, SOL_CAN_RAW, CAN_RAW_ERR_FILTER, &err_mask, sizeof(err_mask)) < 0)
	    fprintf(stderr, "error enabling CAN error reporting: %s\n", strerror(errno));

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
		} else {
		    printf("%s ", timestamp);
		    analyze_frame(&frame);
		}
	    }
	}
	close(sc);
    }
    return 0;
}
