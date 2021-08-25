// ddl.h - adapted for basrcpd project 2018 by Rainer Müller 

/* $Id: ddl.h 1419 2009-12-10 20:06:19Z gscholz $ */

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef DDL_H
#define DDL_H

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <libxml/tree.h>        /*xmlDocPtr, xmlNodePtr */

#include "netservice.h"

#define PKTSIZE     80
#define MAXDATA     20

//Timeout in Sekunden für "fast Refresh" nach neuem Lokkommando
#define FAST_REFRESH_TIMEOUT 60

#define MAX_MARKLIN_ADDRESS 256

/* idle-addr + 127 basic addr's + 10239 long's */
#define MAX_NMRA_ADDRESS 10367

/* broadcast-addr + 511, nur 9 Bit Adressierung wird unterstützt */
#define MAX_MFX_ADDRESS 512

/* data types for maerklin packet pool */
typedef struct _tMaerklinPacket {
    time_t timeLastUpdate;
    char packet[18];
    char f_packets[4][18];
} tMaerklinPacket;

typedef struct _tMaerklinPacketPool {
    tMaerklinPacket packets[MAX_MARKLIN_ADDRESS + 1];
    int knownAddresses[MAX_MARKLIN_ADDRESS + 1];
    int NrOfKnownAddresses;
} tMaerklinPacketPool;

/* data types for mfx packet pool */
typedef struct _tMFXPacket {
    time_t timeLastUpdate;
    char packet[PKTSIZE];
    int packet_size;
} tMFXPacket;

typedef struct _tMFXPacketPool {
    tMFXPacket* packets[MAX_MFX_ADDRESS + 1];
    int knownAddresses[MAX_MFX_ADDRESS + 1];
    int NrOfKnownAddresses;
} tMFXPacketPool;

/* data types for NMRA packet pool */
typedef struct _tNMRAPacket {
    time_t timeLastUpdate;
    char packet[PKTSIZE];
    int packet_size;
    char fx_packet[PKTSIZE];
    int fx_packet_size;
} tNMRAPacket;

typedef struct _tNMRAPacketPool {
    tNMRAPacket* packets[MAX_NMRA_ADDRESS + 1];
    int knownAddresses[MAX_NMRA_ADDRESS + 1];
    int NrOfKnownAddresses;
} tNMRAPacketPool;

typedef struct _tRefreshInfo {
    //Letzte Lok zu der ein Refresh Kommando gesandt wurde für jedes Protokoll
    int last_refreshed_maerklin_loco;
    int last_refreshed_maerklin_fx;
    int last_refreshed_nmra_loco;
    int last_refreshed_mfx_loco;
    //Welches aktivierte Protokoll (EP_xxx) bekommt den nächsten Refresh Cycle
    int protocol_refresh;
} tRefreshInfo;

typedef struct _DDL_DATA {
    int number_gl;
    int number_ga;

    int CHECKSHORT;             /* default no shortcut checking */
    time_t SHORTCUTDELAY;       /* usecs shortcut delay         */
    int CHECKCLIENTS;			/* check if clients can control */
    int ENABLED_PROTOCOLS;      /* enabled p's                  */
    int NMRA_GA_OFFSET;         /* offset for ga base address 0/1*/
    int PROGRAM_TRACK;          /* 0: suppress SM commands to PT address */
   	char MCS_DEVNAME[16];    	/* name of CAN interface for mCS */
   	bus_t FWD_M_ACCESSORIES;	/* bus used for MM accessories   */ 
   	bus_t FWD_N_ACCESSORIES;	/* bus used for NMRA accessories */ 

    int spiLastMM;              //War das letzte Paket ein Märklin Motorola Paket?
    unsigned int uid;           /* Für MFX die UID der Zentrale */

    time_t short_detected;
    char NMRA_idle_data[4 * 256]; //Worst Case SPI Mode

    //"Normaler" Refresh Zyklus
    tRefreshInfo refreshInfo;
    //Proiorisierter / schneller Refresh bekommen alle Loks, die in den letzten FAST_REFRESH_TIMEOUT Sekunden
    //ein neues Kommando erhielten.
    tRefreshInfo refreshInfoFast;
    //Alternierende Ausgabe "fast" und "normaler" Refresh.
    bool lastRefreshFast;
    
    bool pgtrkonly;             /* only programming track will be feed */

    pthread_mutex_t nmra_pktpool_mutex;
    tNMRAPacketPool NMRAPacketPool;

    pthread_mutex_t maerklin_pktpool_mutex;
    tMaerklinPacketPool MaerklinPacketPool;

    pthread_mutex_t mfx_pktpool_mutex;
    tMFXPacketPool MFXPacketPool;
    
    int rdsPipeNew[2]; //FD's für Zugriff auf MFX RDS Auftrags Pipe (Info an RDS Thread, dass es Arbeit gibt).
} DDL_DATA;

int readconfig_DDL(xmlDocPtr doc, xmlNodePtr node, bus_t busnumber);
int init_bus_DDL(bus_t busnumber);

#define  EP_MAERKLIN  1         /* maerklin protocol */
#define  EP_NMRADCC   2         /* nmra dcc protocol */
#define  EP_MFX       4         /* maerklin mfx protocol */

#define QEMPTY      -1
#define QNOVALIDPKT 0
#define QM1LOCOPKT  1
#define QM2LOCOPKT  2
#define QM2FXPKT    3
#define QM1FUNCPKT  4
#define QM1SOLEPKT  5
#define QNBLOCOPKT  6
#define QNBACCPKT   7
//MFX, keine Rückmeldung
#define QMFX0PKT    8
//MFX, 1 Bit Rückmeldung (Quittung)
#define QMFX1PKT    9
//MFX, 1 Bit Rückmeldung (Quittung), die aber nicht ausgewertet wird
#define QMFX1DPKT   10
//MFX, 8 bis 64 Bit (1 bis 8 Byte) Rückmeldung
#define QMFX8PKT    11
#define QMFX16PKT   12
#define QMFX32PKT   13
#define QMFX64PKT   14
//Paket ist nicht mehr gültig, da durch höher priorisiertes bereits überholt
#define QOVERRIDE   100

char *get_maerklin_packet(bus_t busnumber, int adr, int fx);
void update_MaerklinPacketPool(bus_t busnumber, int adr,
                               char const *const sd_packet,
                               char const *const f1, char const *const f2,
                               char const *const f3, char const *const f4);
void update_NMRAPacketPool(bus_t busnumber, int adr,
                           char const *const packet, int packet_size,
                           char const *const fx_packet, int fx_packet_size);
void update_MFXPacketPool(bus_t busnumber, int adr,
                          char const *const packet, int packet_size);

void send_packet(bus_t busnumber, char *packet,
                 int packet_size, int packet_type, int xmits);

/* serial line modes: */
#define ON  1
#define OFF 0

/* serial lines */
#define SL_RTS  4
#define SL_CTS  5
#define SL_DSR  6
#define SL_DTR 20
#define SL_RI  22

/* calculate difference between time value and now, returns it in µs */
time_t timeSince(struct timeval tv);

/* check if shortcut or emergengy break happened */
bool power_is_off(bus_t busnumber);

/* start and stop MCS gateway */
void init_mcs_gateway(bus_t busnumber);
void term_mcs_gateway(void);
int get_pingactive(void);

#endif
