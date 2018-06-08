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
#define MAXDG   	4096	/* maximum datagram size */
#define MAXUDP  	16	/* maximum datagram size */
#define MAX(a,b)	((a) > (b) ? (a) : (b))

#define IPHDR_LEN       (20)
/* defines for the packet type code in an ETHERNET header */
#define ETHER_TYPE_IP	 (0x0800)
#define ETHER_TYPE_8021Q (0x8100)

unsigned char netframe[MAXDG];

struct knoten *statusdaten = NULL;
struct knoten *messwert = NULL;

unsigned char buffer[MAX_PAKETE * 8];
int verbose = 0;

static char *F_N_CAN_FORMAT_STRG = "  CAN  0x%08X  [%d]";
static char *F_N_UDP_FORMAT_STRG = "  UDP  0x%08X  [%d]";
static char *F_N_TCP_FORMAT_STRG = "  TCP  0x%08X  [%d]";

uint16_t be16(uint8_t *u) {
    return (u[0] << 8) | u[1];
}

uint32_t be32(uint8_t *u) {
    return (u[0] << 24) | (u[1] << 16) | (u[2] << 8) | u[3];
}

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
    fprintf(stderr, "   Version 2.0\n\n");
    fprintf(stderr, "         -i <can int>    CAN interface - default can0\n");
    fprintf(stderr, "         -r <pcap file>  read PCAP file instead from CAN socket\n");
    fprintf(stderr, "         -v              verbose output for TCP/UDP\n\n");
    fprintf(stderr, "         -h              show this help\n\n");
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

void print_can_frame(char *format_string, struct can_frame *frame) {
    int i;
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
	if (frame->can_dlc == 7)
	    printf("System: Statusabfrage UID 0x%08X Kanal 0x%02X Wert %d", uid, frame->data[5], frame->data[6]);
	if (frame->can_dlc == 8) {
	    wert = be16(&frame->data[6]);
	    if (response)
		printf("System: Statusabfrage UID 0x%08X Kanal 0x%02X Messwert 0x%04X",
		       uid, frame->data[5], wert);
	    else
		printf("System: Statusabfrage UID 0x%08X Kanal 0x%02X Konfigurationswert 0x%04X",
		       uid, frame->data[5], wert);
	}
	break;
    case 0x0c:
	if (frame->can_dlc == 5) {
	    printf("System: Geraetekennung UID");
	} else {
	    wert = be16(&frame->data[5]);
	    printf("System: Geraetekennung UID 0x%04X", wert);
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
    uint32_t id, kennung, function, uid, cv_number, cv_index, stream_size;
    uint16_t paket, crc, kenner, kontakt;
    uint8_t n_kanaele, n_messwerte, kanal = 0;
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
	    printf("Lok Discovery - Erkennen alle Protokolle\n");
	if (frame->can_dlc == 1)
	    printf("Lok Discovery - Protokoll Kennung 0x%02X\n", frame->data[0]);
	if (frame->can_dlc == 5) {
	    uid = be32(frame->data);
	    printf("Lok Discovery - 0x%04X Protokoll Kennung 0x%02X\n", uid, frame->data[4]);
	}
	if (frame->can_dlc == 6) {
	    uid = be32(frame->data);
	    printf("Lok Discovery - 0x%04X Range %d ASK %d\n", uid, frame->data[4], frame->data[5]);
	}
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
		printf("CdB: Reset\n");
	    else
		printf("CdB: unbekannt 0x%04x\n", kenner);
	}
	if (frame->can_dlc == 4) {
	    cdb_extension_set_grd(frame);
	}
	if (frame->can_dlc == 6)
	    printf("MFX Verify: MFX UID 0x%08X MFX SID %d\n", uid, be16(&frame->data[4]));
	if (frame->can_dlc == 7)
	    printf("MFX Verify: MFX UID 0x%08X MFX SID %d ASK-Verhältnis %d\n",
		   uid, be16(&frame->data[4]), frame->data[6]);
	break;
    /* Lok Geschwindigkeit */
    case 0x08:
    case 0x09:
	v = be16(&frame->data[4]);
	v = v / 10;
	if (frame->can_dlc == 4)
	    printf("Lok: %s, Abfrage Fahrstufe\n", getLoco(frame->data, s));
	else if (frame->can_dlc == 6)
	    printf("Lok: %s, Geschwindigkeit: %3.1f\n", getLoco(frame->data, s), v);
	break;
    /* Lok Richtung */
    case 0x0A:
    case 0x0B:
	memset(s, 0, sizeof(s));

	printf("Lok: %s ", getLoco(frame->data, s));
	if (frame->can_dlc == 4) {
	    printf(" wird abgefragt\n");
	} else if (frame->can_dlc == 5) {
	    switch (frame->data[4]) {
	    case 0:
		strcat(s, " bleibt gleich");
		break;
	    case 1:
		strcat(s, ": vorwärts");
		break;
	    case 2:
		strcat(s, ": rückwärts");
		break;
	    case 3:
		strcat(s, " wechseln");
		break;
	    default:
		strcat(s, "unbekannt");
		break;
	    }
	    printf("Richtung %s\n", s);
	}
	break;
    /* Lok Funktion */
    case 0x0C:
    case 0x0D:
	if (frame->can_dlc == 5)
	    printf("Lok %s Funktion %d\n", getLoco(frame->data, s), frame->data[4]);
	else if (frame->can_dlc == 6)
	    printf("Lok %s Funktion %d Wert %d\n", getLoco(frame->data, s), frame->data[4], frame->data[5]);
	else if (frame->can_dlc == 7)
	    printf("Lok %s Funktion %d Wert %d Funktionswert %d\n",
		   getLoco(frame->data, s), frame->data[4], frame->data[5], be16(&frame->data[6]));
	break;
    /* Read Config */
    case 0x0E:
	if (frame->can_dlc == 7) {
	    cv_number = ((frame->data[4] & 0x3) << 8) + frame->data[5];
	    cv_index = frame->data[4] >> 2;
	    printf("Read Config Lok %s CV Nummer %d Index %d Anzahl %d\n",
		   getLoco(frame->data, s), cv_number, cv_index, frame->data[6]);
	}
	break;
    case 0x0F:
	cv_number = ((frame->data[4] & 0x3) << 8) + frame->data[5];
	cv_index = frame->data[4] >> 2;
	if (frame->can_dlc == 6)
	    printf("Read Config Lok %s CV Nummer %d Index %d\n", getLoco(frame->data, s), cv_number, cv_index);
	if (frame->can_dlc == 7)
	    printf("Read Config Lok %s CV Nummer %d Index %d Wert %d\n",
		   getLoco(frame->data, s), cv_number, cv_index, frame->data[6]);
	break;
    /* Write Config */
    case 0x10:
    case 0x11:
	/* TODO */
	cv_number = ((frame->data[4] & 0x3) << 8) + frame->data[5];
	cv_index = frame->data[4] >> 2;
	if (frame->can_dlc == 8)
	    printf("Write Config Lok %s CV Nummer %d Index %d Wert %d Ctrl 0x%02X\n", getLoco(frame->data, s),
		   cv_number, cv_index, frame->data[6], frame->data[7]);
	else
	    printf("Write Config Lok %s Befehl unbekannt\n", getLoco(frame->data, s));
	break;
    /* Zubehör schalten */
    case 0x16:
    case 0x17:
	if (frame->can_dlc == 6)
	    printf("Zubehör Schalten Lok %s Stellung %d Strom %d\n",
		   getLoco(frame->data, s), frame->data[4], frame->data[5]);
	if (frame->can_dlc == 8)
	    printf("Zubehör Schalten Lok %s Stellung %d Strom %d Schaltzeit/Sonderfunktionswert %d\n",
		   getLoco(frame->data, s), frame->data[4], frame->data[5], be16(&frame->data[6]));
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
	kenner = be16(frame->data);
	kontakt = be16(&frame->data[2]);
	if (frame->can_dlc == 4)
	    printf("S88 Event: Kennung %d Kontakt %d\n", kenner, kontakt);
	if (frame->can_dlc == 5)
	    printf("S88 Event: Kennung %d Kontakt %d Parameter %d\n", kenner, kontakt, frame->data[4]);
	break;
    case 0x23:
	kenner = be16(frame->data);
	kontakt = be16(&frame->data[2]);
	if (frame->can_dlc == 8)
	    printf("S88 Event: Kennung %d Kontakt %d Zustand alt %d Zusand neu %d Zeit %d\n",
		   kenner, kontakt, frame->data[4], frame->data[5], be16(&frame->data[6]));
	break;
    /* SX1 Event */
    case 0x24:
    case 0x25:
	uid = be32(frame->data);
	if (frame->can_dlc == 5)
	    printf("SX1 Event: UID 0x%08X SX1-Adresse %d\n", uid, frame->data[4]);
	if (frame->can_dlc == 5)
	    printf("SX1 Event: UID 0x%08X SX1-Adresse %d Zustand %d\n", uid, frame->data[4], frame->data[5]);
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
	case 0x0010:
	    printf("Gleisbox");
	    break;
	case 0x0030:
	case 0x0031:
	case 0x0032:
	case 0x0033:
	    printf("MS2");
	    break;
	case 0x0040:
	    if ((uid & 0x53380000) == 0x53380000)
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
	printf("CAN Bootloader Anfrage\n");
	break;
    case 0x37:
	uid = be32(frame->data);
	kennung = be16(&frame->data[6]);
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
	case 0x0040:
	    printf("LinkS88");
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
	    printf("Statusdaten: Paket %d", paket);
	    if (paket == 0)
		memset(buffer, 0, sizeof(buffer));
	    if (paket < MAX_PAKETE)
		memcpy(&buffer[paket * 8], frame->data, 8);
	    if ((kanal == 0) && (paket == 0)) {
		n_messwerte = frame->data[0];
		n_kanaele = frame->data[1];
		id = be32(&frame->data[4]);
		printf(" Anzahl Messwerte: %d Anzahl Kanäle: %d Gerätenummer: 0x%08x", n_messwerte, n_kanaele, id);
	    }
	    if ((kanal == 0) && (paket == 1))
		printf("    %s", &buffer[8]);
	    if ((kanal == 0) && (paket == 3))
		printf("    %s", &buffer[16]);
	    printf("\n");
	}
	break;
    /* Anfordern Config Daten */
    case 0x40:
    case 0x41:
	memset(s, 0, sizeof(s));
	memcpy(s, frame->data, 8);
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
	stream_size = be32(frame->data);
	crc = be16(&frame->data[4]);
	if (frame->can_dlc == 6)
	    printf("Config Data Stream: Länge 0x%08X CRC 0x%04X\n", stream_size, crc);
	if (frame->can_dlc == 7)
	    printf("Config Data Stream: Länge 0x%08X CRC 0x%04X (unbekannt 0x%02X)\n", stream_size, crc,
		   frame->data[6]);
	if (frame->can_dlc == 8)
	    printf("Config Data Stream: Daten\n");
	break;
    /* CdB: WeichenChef */
    case 0x44:
    case 0x45:
	cdb_extension_wc(frame);
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
	    printf("Blocktext zuordnen: ID 0x%04X Funktion 0x%04X Lok %s\n", kenner, function,
		   getLoco(&frame->data[4], s));
	break;
    case 0x64:
    case 0x65:
	printf("Debug\n");
	break;
    default:
	printf("unknown\n");
	break;
    }
}

int main(int argc, char **argv) {
    int max_fds, opt, sc;
    struct can_frame frame;
    char pcap_file[256];
    struct sockaddr_can caddr;
    struct ifreq ifr;
    socklen_t caddrlen = sizeof(caddr);
    fd_set read_fds;
    char timestamp[16];

    strcpy(ifr.ifr_name, "can0");
    memset(pcap_file, 0, sizeof(pcap_file));

    signal(SIGINT, INThandler);

    while ((opt = getopt(argc, argv, "i:r:vh?")) != -1) {
	switch (opt) {
	case 'i':
	    strncpy(ifr.ifr_name, optarg, sizeof(ifr.ifr_name) - 1);
	    break;
	case 'r':
	    strncpy(pcap_file, optarg, sizeof(pcap_file) - 1);
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
	while ((packet = pcap_next(handle, &header)) != NULL) {
	    pkt_counter++;
	    /* header contains information about the packet (e.g. timestamp) */
	    /* cast a pointer to the packet data */
	    unsigned char *pkt_ptr = (u_char *) packet;
	    tm = localtime(&header.ts.tv_sec);
	    sprintf(timestamp, "%02d:%02d:%02d.%03d", tm->tm_hour, tm->tm_min, tm->tm_sec, (int)header.ts.tv_usec / 1000);

	    /* parse the first (ethernet) header, grabbing the type field */
	    int ether_type = be16(&pkt_ptr[12]);
	    int ether_offset = 0;

	    if (ether_type == ETHER_TYPE_IP)	/* most common */
		ether_offset = 14;
	    else if (ether_type == ETHER_TYPE_8021Q)	/* dot1q tag ? */
		ether_offset = 18;
	    else
		if (verbose)
		    fprintf(stderr, "Unknown ethernet type, %04X, skipping...\n", ether_type);
	    /* skip past the Ethernet II header */
	    pkt_ptr += ether_offset;
	    /* point to an IP header structure  */
	    ip_hdr = (struct ip *)pkt_ptr;

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
		    printf("%s ", timestamp);
		    frame_to_can(dump + i, &frame);
		    print_can_frame(F_N_UDP_FORMAT_STRG, &frame);
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
			printf("%s   HTTP    -> %s", timestamp, dump);
		    }
		    continue;
		}
		dport = ntohs(mytcp->th_dport);
		sport = ntohs(mytcp->th_sport);
		if ((dport != 15730) && (sport != 15731) &&
		    (dport != 15731) && (sport != 15731) &&
		    (dport != 15732) && (sport != 15732))
		    continue;
		if (size_payload > 0) {
		    if (verbose) {
			printf("%s ", timestamp);
			printf("%04u TCP %s -> ", pkt_counter, inet_ntoa(ip_hdr->ip_src));
			printf("%s port %d -> %d", inet_ntoa(ip_hdr->ip_dst), ntohs(mytcp->th_sport), ntohs(mytcp->th_dport));
			printf("  packet_length %d\n", size_payload);
		    }
		    for (int i = 0; i < size_payload; i += 13) {
			printf("%s ", timestamp);
			frame_to_can(dump + i, &frame);
			print_can_frame(F_N_TCP_FORMAT_STRG, &frame);
			decode_frame(&frame);
			// print_content(dump, size_payload);
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
		} else if ((frame.can_id & CAN_EFF_FLAG)	/* only EFF frames are valid */
			   &&(((frame.can_id & 0x00000380UL) == 0x00000300UL)	/* MS2/CS2 hash ? */
			      ||(frame.can_id == (0x00310000UL | CAN_EFF_FLAG)))) {	/* or Ping reply from CS2 GUI */
		    printf("%s ", timestamp);
		    print_can_frame(F_N_CAN_FORMAT_STRG, &frame);
		    decode_frame(&frame);
		} else {
		    printf("%s ", timestamp);
		    print_can_frame(F_N_CAN_FORMAT_STRG, &frame);
		    printf("\n");
		}
	    }
	}
    }
    close(sc);
    return 0;
}
