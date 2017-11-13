/* $Id: ddl.h 1419 2009-12-10 20:06:19Z gscholz $ */

#ifndef DDL_H
#define DDL_H

#include <syslog.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <termios.h>
#include <signal.h>

#include <pthread.h>
#include <sched.h>

#if linux
#include <sys/io.h>
#include <linux/serial.h>
#endif

#include <libxml/tree.h>        /*xmlDocPtr, xmlNodePtr */

#include "config-srcpd.h"
#include "netservice.h"

#define QSIZE       2000
/* large packet size reserved for F0-F28 packets */
#define PKTSIZE     80
#define MAXDATA     20

//Timeout in Sekunden für "fast Refresh" nach neuem Lokkommando
#define FAST_REFRESH_TIMEOUT 60

typedef struct _tQData {
    int packet_type;
    int packet_size;
    char packet[PKTSIZE];
    int addr;
} tQData;

typedef struct _tQueue {
    int out, in;
    tQData QData[QSIZE];
} tQueue;

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

    int SERIAL_DEVICE_MODE;
    int RI_CHECK;               /* ring indicator checking      */
    int CHECKSHORT;             /* default no shortcut checking */
    int DSR_INVERSE;            /* controls how DSR is used to  */
    /*                             check shorts                 */
    time_t SHORTCUTDELAY;       /* usecs shortcut delay         */
    int NMRADCC_TR_V;           /* version of the nmra dcc      */
    /*                             translation routine(1,2 or 3) */
    int ENABLED_PROTOCOLS;      /* enabled p's                  */
    int IMPROVE_NMRADCC_TIMING; /* NMRA DCC: improve timing     */

    bool WAITUART_USLEEP_PATCH;  /* enable/disbable usleep patch */
    int WAITUART_USLEEP_USEC;   /* usecs for usleep patch       */
    int NMRA_GA_OFFSET;         /* offset for ga base address 0/1*/

    int PROGRAM_TRACK;          /* 0: suppress SM commands to PT address */
    int SIGG_MODE;              /*EDITS Booster Sigg Mode. CHECKSHORT wird automatisch gesetzt. Booster GO message on CTS Line, Booster GO / STOP Command impluse on RTS/DTR)*/
    int MOBILE_STATION_MODE;    /*Verwendung zusammen mit Mobilestation. Booster an Mobilestation für Loksteuerung. Bei einem GA Kommando
                                  wird der Booster über RTS übernommen und nach GA Kommandoausgabe wieder an die Mobile Station zurückgegeben.
                                  Meldung Booster GO erfolgt über DSR */
    int spiMode;                /*true wenn auf RaspPi SPI verwendet wird.*/
    int spiLastMM;              //War das letzte Paket ein Märklin Motorola Paket?

    unsigned int uid;           /* Für MFX die UID der Zentrale */
    struct termios maerklin_dev_termios;
    struct termios nmra_dev_termios;

#if linux
    struct serial_struct *serinfo_marklin;
    struct serial_struct *serinfo_nmradcc;
#endif
    pthread_mutex_t queue_mutex;        /* mutex to synchronize queue inserts */
    int queue_initialized;
    tQueue queueNormal, queuePriority;

    time_t short_detected;

    pthread_t refresh_ptid;
    struct _thr_param refresh_param;

    unsigned char idle_data[MAXDATA];
    char NMRA_idle_data[4 * 256]; //Worst Case SPI Mode
    int NMRA_idle_data_size;

    //"Normaler" Refresh Zyklus
    tRefreshInfo refreshInfo;
    //Proiorisierter / schneller Refresh bekommen alle Loks, die in den letzten FAST_REFRESH_TIMEOUT Sekunden
    //ein neues Kommando erhielten.
    tRefreshInfo refreshInfoFast;
    //Alternierende Ausgabe "fast" und "normaler" Refresh.
    bool lastRefreshFast;

    struct timespec rmtp;       /* Do we really nead rmtp? */

    pthread_mutex_t nmra_pktpool_mutex;
    tNMRAPacketPool NMRAPacketPool;

    pthread_mutex_t maerklin_pktpool_mutex;
    tMaerklinPacketPool MaerklinPacketPool;

    pthread_mutex_t mfx_pktpool_mutex;
    tMFXPacketPool MFXPacketPool;
    
    int rdsPipeNew[2]; //FD's für Zugriff auf MFX RDS Auftrags Pipe (Info an RDS Thread, dass es Arbeit gibt).

    int oslevel;                /* 0: ancient linux 2.4, 1 linux 2.6 */
    int program_track;          /* 0: suppress SM commands to PT address */

} DDL_DATA;

int readconfig_DDL(xmlDocPtr doc, xmlNodePtr node, bus_t busnumber);
int init_bus_DDL(bus_t busnumber);

#define  EP_MAERKLIN  1         /* maerklin protocol */
#define  EP_NMRADCC   2         /* nmra dcc protocol */
#define  EP_MFX       4         /* maerklin mfx protocol */

//NMRA Translation Routine Kennung (NMRADCC_TR_V) für SPI
#define NMRADCC_TR_SPI -1

/* constants for setSerialMode() */
#define SDM_NOTINITIALIZED -1
#define SDM_DEFAULT         1   /* must be one of the following values */
#define SDM_MAERKLIN        0
#define SDM_NMRA            1
int setSerialMode(bus_t busnumber, int mode);

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

/**
 * @param priority Wenn true hat das Paket Priorität, es wird in der Queue vor allen anderen eingefügt.
 *                 Wenn nun weiter hinten solche kleinerer Prio zur gleichen Adresse bestehen werden diese
 *                 als ungültig markiert, so dass sie nicht mehr ausgegeben werden.
 */
void queue_add(bus_t busnumber, int addr, char *const packet,
               int packet_type, int packet_size, bool priority);

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

void (*waitUARTempty) (bus_t busnumber);
void send_packet(bus_t busnumber, char *packet,
                 int packet_size, int packet_type, int refresh);

/* serial line modes: */
#define ON  1
#define OFF 0

/* serial lines */
#define SL_RTS  4
#define SL_CTS  5
#define SL_DSR  6
#define SL_DTR 20
#define SL_RI  22

/* calculate difference between two time values and return the
 * difference in microseconds */
long int compute_delta(struct timeval tv1, struct timeval tv2);

#endif
