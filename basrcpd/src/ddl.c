/* $Id: ddl.c 1456 2010-02-28 20:01:39Z gscholz $ */

/*
 * DDL: Bus driver connected with a booster only without any special hardware.
 * 
 * SIG
 * 24.08.12:
 * RaspberryPI Portierung. Nicht ganz sauber, aber einfach:
 * Wenn __arm__ definiert ist wird RaspberryPI angenommen.
 * Für den GPIO Zugriff wird die lib bcm2835 (http://www.open.com.au/mikem/bcm2835/index.html) verwendet.
 * Diese muss installiert sein. Zum linken muss im makefile für "libs" "-l bcm2835" eingetragen sein.
 * 
 * 30.09.13:
 * RaspberryPI Erkennung über "configure" (CPU ARM und cpuinfo BCM2708).
 * Unterscheidung RaspberryPI Board Version 1 und 2.
 *
 * 20.08.16
 * RaspberryPI Model 2 & 3
 *
 * 23.08.16
 * SPI Ausgabe für Raspberry PI, aber unabhängig von diesem implementiert.
 * Jedoch trotzdem nur auf diesem benutzbar, da auf dem PI die zusätzlichen Leitungen
 * zur Booster Steuerung über GPIO's realisiert werden. Wenn ich nicht auf dem PI
 * bin weiss ich dann nicht, was ich da anstelle der RS232 HW-Handshake Leitungen nehmen soll...
 * Da der RS232 Ruhepegel Logisch 1 ist, der von SPI jedoch logisch 0 brauchen wir für das 
 * Märklin / Motorola Protokoll noch einen Inverter. Wer nur DCC/NMRA macht kann darauf
 * verzichten.
 *
 * 27.09.16
 * MFX Protokoll (nur für SPI Ausgabe).
 * MFX Rückmelderkennung wird über SPI MISO eingelesen.
 * MFX Rückmeldedaten auf den RaspPi über folgende Ports:
 *   - RDS Qual: GPIO23 (Pin 16)
 *   - RDS Clk : GPIO24 (Pin 18)
 *   - RDS Data: GPIO25 (Pin 22)
 * Für MFX auf anderen Plattformen muss das entsprechend neu definiert werden ....
 * MFX Loks werden automatisch erkannt und angemeldet
 * und dann dem SRCP Client gemeldet. Dieser kann dann ggf. die Schienenadresse anpassen.
 * Der SRCP Client muss die Speicherung der angemeldeten Loks übernehmen und diese nach
 * einem srcpd Neustart mittels "init" wieder melden.
 * Der MFX Neuanmeldezähler wird mit jeder automatischen Neuanmeldung inkrementiert
 * und nicht flüchtig gespeichert, so dass er bei einem Neustart wieder gleich zur Verfügung steht.
 * Definition SRCP Init Kommando / Info:
 * INIT <bus> GL <gleisaddr> <protocol> <protocolversio> <decoderspeedsteps> <numberofdecoderfunctions> <lokUid> <"lokname"> <mfxfunctioncode1> ... <mfxfunctioncode16>
 *  - protocol X (=MFX)
 *  - protocolversio = 0
 *  - decoderspeedsteps Anzahl Fahrstufen (128)
 *  - numberofdecoderfunctions Anzahl Funktionen (16)
 *  - lokUid MFX UID der Lok
 *  - "lokName": der ausgelesene Name der Lok in "".
 *  - numberfunction1 bis 16 Defintion der Funktion gemäss MFX Codes (3 Bytes in einer Zahl)
 * Wenn eine neue Lok erkannt wird werden alle notwnedigen Daten ausgelesen. Wenn sie bereits bekannt ist (UID bereits vorhanden),
 * wird ihr die bisherige Adrese zugewiesen, sonst die erste freie Lokadresse.
 * Wenn ein INIT Kommando zu einer bereits mit anderer Adresse vorhandenen Lok empfangen wird, dann wir der Lok die neue Adresse
 * zugewiesen und die alte Lok Adresse gelöscht.
 * srcp.conf Eintrag um MFX zu aktivieren: "enable_mfx", Parameter ist die max. 32Bit grosse UID der Zentrale, die verwendet werden soll.
 */

/* +----------------------------------------------------------------------+ */
/* | DDL - Digital Direct for Linux                                       | */
/* +----------------------------------------------------------------------+ */
/* | Copyright (c) 2002 - 2003 Vogt IT                                    | */
/* +----------------------------------------------------------------------+ */
/* | This source file is subject of the GNU general public license 2,     | */
/* | that is bundled with this package in the file COPYING, and is        | */
/* | available at through the world-wide-web at                           | */
/* | http://www.gnu.org/licenses/gpl.txt                                  | */
/* | If you did not receive a copy of the PHP license and are unable to   | */
/* | obtain it through the world-wide-web, please send a note to          | */
/* | gpl-license@vogt-it.com so we can mail you a copy immediately.       | */
/* +----------------------------------------------------------------------+ */

/***************************************************************/
/* erddcd - Electric Railroad Direct Digital Command Daemon    */
/*    generates without any other hardware digital commands    */
/*    to control electric model railroads                      */
/*                                                             */
/* Authors of the old erddcd part:                             */
/*                                                             */
/* 1999 - 2002 Torsten Vogt <vogt@vogt-it.com>                 */
/*                                                             */
/* Thanks to:                                                  */
/*                                                             */
/* Kurt Harders: i8255 implementation.                         */
/*                                                             */
/* Dieter Schaefer: s88 implementation                         */
/*                  additional code for marklin acc. decoders  */
/*                                                             */
/* Olaf Schlachter: debugging                                  */
/*                                                             */
/* Michael Peschel: new nmra dcc translation routine           */
/*                                                             */
/* Markus Gietzen: debugging and corrections                   */
/*                                                             */
/* Martin Wolf: re-implementation and enhancements of the s88  */
/*              support and the implementation of ga_manager   */
/*                                                             */
/* Sim IJskes: patch for a better port handling                */
/*                                                             */
/* Martin Schönbeck: third version of nmra dcc translation rtn.*/
/*                                                             */
/* Harald Barth: debugging and corrections                     */
/*                                                             */
/* Berthold Benning: usleep patch for SuSE kernels             */
/***************************************************************/

#include <sys/utsname.h>
#include <stdbool.h>

#include "config.h"
#include "config-srcpd.h"
#include "ddl.h"
#include "ddl_maerklin.h"
#include "ddl_nmra.h"
#include "ddl_mfx.h"
#include "io.h"
#include "srcp-error.h"
#include "srcp-fb.h"
#include "srcp-ga.h"
#include "srcp-gl.h"
#include "srcp-sm.h"
#include "srcp-info.h"
#include "srcp-power.h"
#include "srcp-server.h"
#include "syslogmessage.h"
//Header für SPI Ausgabe
#include <stdint.h> 
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h> 


#ifdef BANANAPI
    #include <a20hw.h>
    #define BAPI_RTS    (PORTIPIN+16)
    #define BAPI_CTS    (PORTIPIN+17)
#elif defined RASPBERRYPI
    #include <bcm2835.h>
    #ifdef RASPBERRYPI_BOARD_V1
    //Auf dem RaspberryPI wird für DTR GPIO4 (=Pin7), RTS GPIO27 (=Pin13), CTS GPIO1 (=Pin5) und DSR GPIO0 (=Pin3) direkt verwendet, andere Leitungen werden da NICHT unterstützt.
        #define RPI_DTR RPI_GPIO_P1_07
        #define RPI_RTS RPI_GPIO_P1_13
        #define RPI_CTS RPI_GPIO_P1_05
        #define RPI_DSR RPI_GPIO_P1_03
    #elif defined RASPBERRYPI_BOARD_V2
    //Auf dem Board V2 wurden ein paar Leitungen anders gemacht, warum auch immer...
    //Auf dem RaspberryPI wird für DTR GPIO4 (=Pin7), RTS GPIO27 (=Pin13), CTS GPIO3 (=Pin5) und DSR GPIO2 (=Pin3) direkt verwendet, andere Leitungen werden da NICHT unterstützt.
        #define RPI_DTR RPI_V2_GPIO_P1_07
        #define RPI_RTS RPI_V2_GPIO_P1_13
        #define RPI_CTS RPI_V2_GPIO_P1_05
        #define RPI_DSR RPI_V2_GPIO_P1_03
    #elif defined RASPBERRYPI_MODEL_2_3
        #define RPI_DTR RPI_BPLUS_GPIO_J8_07
        #define RPI_RTS RPI_BPLUS_GPIO_J8_13
        #define RPI_CTS RPI_BPLUS_GPIO_J8_05
        #define RPI_DSR RPI_BPLUS_GPIO_J8_03
    #else
        #error "Unsupported new RaspberryPI Board Version"
    #endif
#endif


#define __DDL ((DDL_DATA*)buses[busnumber].driverdata)
#define __DDLt ((DDL_DATA*)buses[btd->bus].driverdata)

#define DAUER_STOP_IMPULS_SIGG_MODE 500000
#define DAUER_START_IMPULS_SIGG_MODE 500000
//Zeit, die Booster On sein muss in uSekunden, damit bei Off (wegen Schluss) automatisch wieder eingeschaltet wird
#define TIMEOUT_SHORTCUT_POWEROFF 10000000

//SPI Baudrate für Märklin / Motorola Protokoll.
//Diese wäre eigentlich genau 38461 Baud (1 Bit=26us, 1Byte=208us)
#define SPI_BAUDRATE_MAERKLIN_LOCO 38461
//Nun macht der RaspberryPI aber leider nach jedem Byte Transfer auf dem SPI Bus 1 Bit Pause :-(
//Er macht diese 1 Bit / Clk Pause nicht mehr im DMA Mode. DMA wird ab Transfers
//von 96 Bytes verwendet (hoffe, das bleibt so ...).
//Um Märklin / Motorola Pakete auf 96 rauf zu bringen, wird folgendes gemacht:
//- die Baudrate Doppelt so gewählt wie notwendig (2 * 38461 für Loks)
//- die Wiederholung ins selbe Paket gepackt.
//- Pause am Anfang und vor Wiederholung mit 0 Bytes gefüllt.
#define SPI_BAUDRATE_MAERKLIN_LOCO_2 (2*SPI_BAUDRATE_MAERKLIN_LOCO)
#define SPI_BAUDRATE_MAERKLIN_FUNC_2 (2*SPI_BAUDRATE_MAERKLIN_LOCO_2)

//SPI Baudrate für DCC/NMRA.
//Diese wird so gewählt, dass ein kurzer 58/58us Impuls (logisch 1) in einem Byte (0xF0) ausgegeben werden kann,
//ein langer 116/116us Impuls wird dann als 2 Bytes (0xFF, 0x00) ausgegeben.
//Damit hätten wird für 8 Bit also 116us -> 1 Bit 14.5us -> 68966 Baud.
//Damit werden NMRA/DCC Pakete inkl. der führenden Sync. Bytes leider nicht immer >= 96 Bytes -> DMA Mode und keine Pause nach 8 Bits
#define SPI_BAUDRATE_NMRA 68966
//Deshalb wird auch hier die doppelte Baudrate verwendet und dann wie folgt kodiert:
//1: 0xFF, 0x00
//0: 0xFF, 0xFF, 0x00, 0x00
#define SPI_BAUDRATE_NMRA_2 (SPI_BAUDRATE_NMRA * 2)
//NMRA Idle Paket als direkt für SPI
static struct spi_ioc_transfer spi_nmra_idle;

//SPI Baudrate für MFX.
//Auch hier müssen wir auf sicher 96 Bytes kkommen um im DMA Modus zu sein und keine Pause zwischen den Bytes zu haben.
//1 Bit in MFX sind immer 100us. 1 Bit wird auf ein SPI Byte gelegt, also für ein Bit 100 / 8 = 12.5us -> 80000 Baud
//Grösse Paket wird damit (mit Sync Muster) für das kleinst mögliche Paket, das wir senden:
#define SPI_BAUDRATE_MFX 80000
//Minimales Paket das verwendet wird, ist "Fahren kurz" plus "Funktionen kurz"
// - 0            :  4 Bit -> Sicherstellen, dass Startpegel immer 0 ist
// - Sync         :  5 Bit
// - 10AAAAAAA    :  9 Bit Adresse (Minimal bei 7 Bit Adresse)
// - Kommando     :  7 Bit "Fahren kurz" 3+1+3
// - Kommando     :  7 Bit "Funktionen kurz" 3+4
// - Checksumme   :  8 Bit
// - Sync         : 10 Bit
// - Total        : 50 Bit
//Auch hier kommt nun deshalb wieder die doppelte Baudrate ins Spiel, damit wären wir bei 2*50=100 Bytes.
//1 MFX Bit -> 2 SPI Bytes
#define SPI_BAUDRATE_MFX_2 (SPI_BAUDRATE_MFX * 2)
#define MFX_STARTSTOP_0_BYTES 8
//11 Sync für RDS Rückmeldung
#define MFX_SYNC_COUNT_RDS 11
//Pause für 1 Bit Rückmeldungen in usec
#define MFX_RDS_1BIT_DELAY 6400
//Anzahl RDS Sync Impulse
#define MFX_RDS_SYNC_IMPULS_COUNT 23
//Takt RDS Sync Impulse (in usec)
#define MFX_RDS_SYNC_IMPULS_CLK_USEC 912
//Breite RDS Sync Impulse (in usec)
#define MFX_RDS_SYNC_IMPULS_USEC 25


static int (*nanosleep_DDL) (const struct timespec * req,
                             struct timespec * rem);
static void *thr_sendrec_DDL(void *);

/********* Q U E U E *****************/
static void queue_init_native(tQueue *queue) {
    int i;
    for (i = 0; i < QSIZE; i++) {
        queue->QData[i].packet_type = QNOVALIDPKT;
        queue->QData[i].addr = 0;
        memset(queue->QData[i].packet, 0, PKTSIZE);
    }
    queue->in = 0;
    queue->out = 0;
}

static void queue_init(bus_t busnumber)
{
    int result;

    result = pthread_mutex_init(&__DDL->queue_mutex, NULL);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_init() failed: %s (errno = %d).",
                   strerror(result), result);
        exit(EXIT_FAILURE);
    }

    result = pthread_mutex_lock(&__DDL->queue_mutex);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_lock() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    queue_init_native(&__DDL->queueNormal);
    queue_init_native(&__DDL->queuePriority);
    __DDL->queue_initialized = true;

    result = pthread_mutex_unlock(&__DDL->queue_mutex);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_unlock() failed: %s (errno = %d).",
                   strerror(result), result);
    }

}

static int queue_empty_native(tQueue *queue) {
    return queue->in == queue->out;
}

static int queue_empty(bus_t busnumber)
{
    return queue_empty_native(&__DDL->queueNormal) && queue_empty_native(&__DDL->queuePriority);
}

static void queue_add_native(tQueue *queue, int addr, char *const packet, int packet_type, int packet_size) {
      memset(queue->QData[queue->in].packet, 0, PKTSIZE);
      memcpy(queue->QData[queue->in].packet, packet, packet_size);
      queue->QData[queue->in].packet_type = packet_type;
      queue->QData[queue->in].packet_size = packet_size;
      queue->QData[queue->in].addr = addr;
      queue->in++;
      if (queue->in == QSIZE) {
          queue->in = 0;
      }
}

void queue_add(bus_t busnumber, int addr, char *const packet,
               int packet_type, int packet_size, bool priority)
{
    int result;

    result = pthread_mutex_lock(&__DDL->queue_mutex);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_lock() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    if (priority) {
      //Wenn schon Pakete zur selben Adresse in der normalen Queue sind -> diese sind nicht mehr gültig
      int next = __DDL->queueNormal.out;
      while (next != __DDL->queueNormal.in) {
        if ((__DDL->queueNormal.QData[next].addr == addr) && (__DDL->queueNormal.QData[next].packet_type == packet_type)) {
          __DDL->queueNormal.QData[next].packet_type = QOVERRIDE;
        }
        next++;
        if (next == QSIZE) {
          next = 0;
        }
      }
      queue_add_native(&__DDL->queuePriority, addr, packet, packet_type, packet_size);
    }
    else {
      queue_add_native(&__DDL->queueNormal, addr, packet, packet_type, packet_size);
    }

    result = pthread_mutex_unlock(&__DDL->queue_mutex);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_unlock() failed: %s (errno = %d).",
                   strerror(result), result);
    }
}

/**
 * Liefert den nächsten Eintrag der Queue.
 */
static int queue_get_native(tQueue *queue, int *addr, char *packet, int *packet_size) {
  if (queue_empty_native(queue)) {
    return QEMPTY;
  }
  memcpy(packet, queue->QData[queue->out].packet, PKTSIZE);
  int rtc = queue->QData[queue->out].packet_type;
  *packet_size = queue->QData[queue->out].packet_size;
  *addr = queue->QData[queue->out].addr;
  queue->QData[queue->out].packet_type = QNOVALIDPKT;
  queue->out++;
  if (queue->out >= QSIZE) {
    queue->out = 0;
  }
  return rtc;
}

/**
  * Liefert, wenn vorhanden, das nächste Paket aus der Queue. Es wird immer zuerst die 
  * QDataPriority und erst wenn diese leer ist QData berücksichtigt.
  */
static int queue_get(bus_t busnumber, int *addr, char *packet,
                     int *packet_size)
{
    int rtc;
    int result;

    if (!__DDL->queue_initialized) {
        return QEMPTY;
    }

    result = pthread_mutex_lock(&__DDL->queue_mutex);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_lock() failed: %s (errno = %d).",
                   strerror(result), result);
    }
    if (queue_empty(busnumber)) {
      rtc = QEMPTY;
    }
    else {
      //Wenn es in der Prio Queue etwas hat, dann kommt diese zuerst an die Reihe
      if (queue_empty_native(&__DDL->queuePriority)) {
        //Keine Prio Aufträge -> normale Queue
        rtc = queue_get_native(&__DDL->queueNormal, addr, packet, packet_size);
      }
      else {
        rtc = queue_get_native(&__DDL->queuePriority, addr, packet, packet_size);
      }
    }
    result = pthread_mutex_unlock(&__DDL->queue_mutex);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_unlock() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    return rtc;
}

// OBSOLETE
int setSerialMode(bus_t busnumber, int mode)
{
    return 0;
}

int init_lineDDL(bus_t busnumber)
{
    /* opens and initializes the selected SPI port */
    /* returns a file handle                       */

    int dev;

    /* open SPI port */
    dev = open(buses[busnumber].device.file.path, O_WRONLY);
    if (dev < 0) {
        syslog_bus(busnumber, DBG_FATAL,
                   "Device '%s' open failed: %s (errno = %d). "
                   "Terminating...\n", buses[busnumber].device.file.path,
                   strerror(errno), errno);
        /* there is no chance to continue */
        exit(1);
    }
    if (strstr(buses[busnumber].device.file.path, "spidev")) {
      //Es wird ein SPI Interface zur Ausgabe verwendet
      __DDL->spiLastMM = 0;

#ifndef RASPBERRYPI
        syslog_bus(busnumber, DBG_FATAL, "SPI only on Raspberry PI supported!");
        /* there is no chance to continue */
        exit(1);
#endif      
    }

      // Set SPI parameters.
    int mode = 1; 
    if (ioctl (dev, SPI_IOC_WR_MODE, &mode) < 0) {
        syslog_bus(busnumber, DBG_FATAL, "SPI Mode Change failure: %s", strerror(errno));
        return 1;
    }
    uint8_t spiBPW = 8;
    if (ioctl (dev, SPI_IOC_WR_BITS_PER_WORD, &spiBPW) < 0) {
        syslog_bus(busnumber, DBG_FATAL, "SPI BPW Change failure: %s", strerror(errno));
        return 1;
    }
    int speed = SPI_BAUDRATE_MAERKLIN_LOCO_2;
    if (ioctl (dev, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
        syslog_bus(busnumber, DBG_FATAL, "SPI Speed Change failure: %s", strerror(errno));
        return 1;
    }
    buses[busnumber].device.file.fd = dev;      /* we need that value at the next step */

    return dev;
}


/****** routines for Maerklin packet pool *********************/

static void init_MaerklinPacketPool(bus_t busnumber)
{
    int i, j;
    int result;

    result = pthread_mutex_init(&__DDL->maerklin_pktpool_mutex, NULL);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_init() failed: %s (errno = %d). Abort!",
                   strerror(result), result);
        exit(1);
    }

    result = pthread_mutex_lock(&__DDL->maerklin_pktpool_mutex);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_lock() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    for (i = 0; i <= MAX_MARKLIN_ADDRESS; i++)
        __DDL->MaerklinPacketPool.knownAddresses[i] = 0;

    __DDL->MaerklinPacketPool.NrOfKnownAddresses = 1;
    __DDL->MaerklinPacketPool.knownAddresses[__DDL->MaerklinPacketPool.
                                             NrOfKnownAddresses - 1] = 81;
    /* generate idle packet */
    for (i = 0; i < 4; i++) {
        __DDL->MaerklinPacketPool.packets[81].packet[2 * i] = getMaerklinHI();
        __DDL->MaerklinPacketPool.packets[81].packet[2 * i + 1] = getMaerklinLO();
        for (j = 0; j < 4; j++) {
            __DDL->MaerklinPacketPool.packets[81].f_packets[j][2 * i] = getMaerklinHI();
            __DDL->MaerklinPacketPool.packets[81].f_packets[j][2 * i + 1] = getMaerklinLO();
        }
    }
    for (i = 4; i < 9; i++) {
        __DDL->MaerklinPacketPool.packets[81].packet[2 * i] = getMaerklinLO();
        __DDL->MaerklinPacketPool.packets[81].packet[2 * i + 1] = getMaerklinLO();
        for (j = 0; j < 4; j++) {
            __DDL->MaerklinPacketPool.packets[81].f_packets[j][2 * i] = getMaerklinLO();
            __DDL->MaerklinPacketPool.packets[81].f_packets[j][2 * i + 1] = getMaerklinLO();
        }
    }

    result = pthread_mutex_unlock(&__DDL->maerklin_pktpool_mutex);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_unlock() failed: %s (errno = %d).",
                   strerror(result), result);
    }
}

char *get_maerklin_packet(bus_t busnumber, int adr, int fx)
{
    return __DDL->MaerklinPacketPool.packets[adr].f_packets[fx];
}

void update_MaerklinPacketPool(bus_t busnumber, int adr,
                               char const *const sd_packet,
                               char const *const f1, char const *const f2,
                               char const *const f3, char const *const f4)
{
    int i, found;
    int result;

    syslog_bus(busnumber, DBG_INFO, "Update MM packet pool: %d", adr);
    found = 0;
    for (i = 0; i < __DDL->MaerklinPacketPool.NrOfKnownAddresses && !found;
         i++)
        if (__DDL->MaerklinPacketPool.knownAddresses[i] == adr)
            found = true;

    result = pthread_mutex_lock(&__DDL->maerklin_pktpool_mutex);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_lock() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    __DDL->MaerklinPacketPool.packets[adr].timeLastUpdate = time(NULL);
    memcpy(__DDL->MaerklinPacketPool.packets[adr].packet, sd_packet, 18);
    memcpy(__DDL->MaerklinPacketPool.packets[adr].f_packets[0], f1, 18);
    memcpy(__DDL->MaerklinPacketPool.packets[adr].f_packets[1], f2, 18);
    memcpy(__DDL->MaerklinPacketPool.packets[adr].f_packets[2], f3, 18);
    memcpy(__DDL->MaerklinPacketPool.packets[adr].f_packets[3], f4, 18);

    result = pthread_mutex_unlock(&__DDL->maerklin_pktpool_mutex);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_unlock() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    if (__DDL->MaerklinPacketPool.NrOfKnownAddresses == 1
        && __DDL->MaerklinPacketPool.knownAddresses[0] == 81)
        __DDL->MaerklinPacketPool.NrOfKnownAddresses = 0;

    if (!found) {
        __DDL->MaerklinPacketPool.knownAddresses[__DDL->MaerklinPacketPool.
                                                 NrOfKnownAddresses] = adr;
        __DDL->MaerklinPacketPool.NrOfKnownAddresses++;
    }
}


/****** routines for NMRA packet pool *********************/
static void reset_NMRAPacketPool(bus_t busnumber)
{
    int i;
    int result;
    result = pthread_mutex_lock(&__DDL->nmra_pktpool_mutex);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_lock() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    for (i = 0; i < __DDL->NMRAPacketPool.NrOfKnownAddresses; i++) {
        int nr = __DDL->NMRAPacketPool.knownAddresses[i];
        free(__DDL->NMRAPacketPool.packets[nr]);
        __DDL->NMRAPacketPool.packets[nr] = 0;
    }

    /* free idle package - this is needed because the idle packet is removed
       from the knownAdresses in the PacketPool after the first Loco is
       refreshed -> TODO: a better place for this free would be in 
       update_NMRAPacketPool */
    free(__DDL->NMRAPacketPool.packets[128]);

    result = pthread_mutex_unlock(&__DDL->nmra_pktpool_mutex);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_unlock() failed: %s (errno = %d).",
                   strerror(result), result);
    }

}

/**
 * Konvertiert ein für SPI Ausgabe bestimmtest NMRA Packet in den zur Ausgabe benötigten Bytestream.
 * @param packet Das zu konvertierende Packet. Im ersten Byte werden die Anzahl Bits angegegebn, die
 *               ab dem 2. Byte folgendes
 * @param spiBuffer Buffer zur Speicherung des SPI Bytestream. Muss mindestens 256*4 Bytes gross sein.
 * @return Länge des spiBuffer.
 */
static unsigned int convertNMRAPacketToSPIStream(bus_t busnumber, char *packet, char *spiBuffer) {
  // 1 : 0xFF, 0x00 -> Ein Impuls in 2 Bytes, Baudrate ist so, dass diese Ausgabe 116us dauert
  // 0 : 0xFF, 0xFF, 0x00, 0x00 -> Ein Impuls in 4 Bytes, damit 116us HI, 116us LOW
  unsigned int len = 0;
  unsigned int i;
  spiBuffer[len++] = 0x00;  // BananaPi
  for (i=0; i<(unsigned char)packet[0]; i++) {
    if (packet[(i / 8) + 1] & (1 << (i % 8))) {
      //1
      spiBuffer[len] = 0xFF;
      len++;
      spiBuffer[len] = 0x00;
      len++;
    }
    else {
      //0
      spiBuffer[len] = 0xFF;
      len++;
      spiBuffer[len] = 0xFF;
      len++;
      spiBuffer[len] = 0x00;
      len++;
      spiBuffer[len] = 0x00;
      len++;
    }
  } 
  spiBuffer[len++] = 0xFF;  // RM: noch mindestens eine EINS dranhängen
  spiBuffer[len++] = 0x00;
  if (len < 96) {
    syslog_bus(busnumber, DBG_WARN, "SPI Transfer < 96 Bytes (%d Bytes) -> no DMA Mode, wrong Timing", len);
  }
  return len;
}

static void init_NMRAPacketPool(bus_t busnumber)
{
    int i, j;
    char idle_packet[] = "1111111111111110111111110000000000111111111";
    char idle_pktstr[PKTSIZE];
    int result;

    result = pthread_mutex_init(&__DDL->nmra_pktpool_mutex, NULL);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_init() failed: %s (error = %d). Terminating!\n",
                   strerror(result), result);
        exit(EXIT_FAILURE);
    }

    result = pthread_mutex_lock(&__DDL->nmra_pktpool_mutex);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_lock() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    for (i = 0; i <= MAX_NMRA_ADDRESS; i++) {
        __DDL->NMRAPacketPool.knownAddresses[i] = 0;
        __DDL->NMRAPacketPool.packets[i] = 0;
    }

    __DDL->NMRAPacketPool.NrOfKnownAddresses = 0;

    result = pthread_mutex_unlock(&__DDL->nmra_pktpool_mutex);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_unlock() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    /* put idle packet in packet pool */
    j = translateBitstream2Packetstream(busnumber, idle_packet,
                                        idle_pktstr, false);
    update_NMRAPacketPool(busnumber, 128, idle_pktstr, j, idle_pktstr, j);
    /* generate and override idle_data */
    /* insert the NMRA idle packetstream (the standard idle stream was all
       '1' which is OK for NMRA, so keep the rest of the idle string) */
/*!!!!!!!! TODO, SIG 5.9.16, warum wird hier 0x55 idle Data, die mit Märklin Baudrate gesendet werden, überschrieben?????
    for (i = 0; i < (MAXDATA / j) * j; i++)
      __DDL->idle_data[i] = idle_pktstr[i % j]; */
    
    memset (&spi_nmra_idle, 0, sizeof (spi_nmra_idle)) ;
    spi_nmra_idle.len = convertNMRAPacketToSPIStream(busnumber, idle_pktstr, __DDL->NMRA_idle_data);
    spi_nmra_idle.tx_buf =  (unsigned long)__DDL->NMRA_idle_data;
    spi_nmra_idle.bits_per_word = 8;
    spi_nmra_idle.speed_hz = SPI_BAUDRATE_NMRA_2;
//      memcpy(__DDL->NMRA_idle_data, idle_pktstr, j);
//      __DDL->NMRA_idle_data_size = j;
}

void update_NMRAPacketPool(bus_t busnumber, int adr,
                           char const *const packet, int packet_size,
                           char const *const fx_packet, int fx_packet_size)
{
    int i, found;
    int result;

    found = 0;
    for (i = 0; i <= __DDL->NMRAPacketPool.NrOfKnownAddresses && !found;
         i++)
        if (__DDL->NMRAPacketPool.knownAddresses[i] == adr)
            found = true;

    result = pthread_mutex_lock(&__DDL->nmra_pktpool_mutex);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_lock() failed: %s (errno = %d).",
                   strerror(result), result);
    }
    if (!__DDL->NMRAPacketPool.packets[adr]) {
        __DDL->NMRAPacketPool.packets[adr] = malloc(sizeof(tNMRAPacket));
        if (__DDL->NMRAPacketPool.packets[adr] == NULL) {
            syslog_bus(busnumber, DBG_ERROR,
                       "Memory allocation error in update_NMRAPacketPool");
            return;
        }
    }
    __DDL->NMRAPacketPool.packets[adr]->timeLastUpdate = time(NULL);
    memcpy(__DDL->NMRAPacketPool.packets[adr]->packet, packet,
           packet_size);
    __DDL->NMRAPacketPool.packets[adr]->packet_size = packet_size;
    memcpy(__DDL->NMRAPacketPool.packets[adr]->fx_packet, fx_packet,
           fx_packet_size);
    __DDL->NMRAPacketPool.packets[adr]->fx_packet_size = fx_packet_size;


    if (__DDL->NMRAPacketPool.NrOfKnownAddresses == 1
        && __DDL->NMRAPacketPool.knownAddresses[0] == 128)
        __DDL->NMRAPacketPool.NrOfKnownAddresses = 0;

    if (!found) {
        __DDL->NMRAPacketPool.knownAddresses[__DDL->NMRAPacketPool.
                                             NrOfKnownAddresses] = adr;
        __DDL->NMRAPacketPool.NrOfKnownAddresses++;
    }
    result = pthread_mutex_unlock(&__DDL->nmra_pktpool_mutex);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_unlock() failed: %s (errno = %d).",
                   strerror(result), result);
    }
}

/****** routines for MFX packet pool *********************/
static void reset_MFXPacketPool(bus_t busnumber)
{
    int i;
    int result;

    if (stopMFXThreads() != 0) {
      syslog_bus(busnumber, DBG_ERROR, "stopMFXThreads failed.");
    }
    
    result = pthread_mutex_lock(&__DDL->mfx_pktpool_mutex);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_lock() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    for (i = 0; i < __DDL->MFXPacketPool.NrOfKnownAddresses; i++) {
        int nr = __DDL->MFXPacketPool.knownAddresses[i];
        free(__DDL->MFXPacketPool.packets[nr]);
        __DDL->MFXPacketPool.packets[nr] = 0;
    }

    result = pthread_mutex_unlock(&__DDL->mfx_pktpool_mutex);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_unlock() failed: %s (errno = %d).",
                   strerror(result), result);
    }
    
    close(__DDL->rdsPipeNew[0]);
    close(__DDL->rdsPipeNew[1]);
}

/**
 * Fügt eine MFX Sync Struktur ein.
 * @param spiBuffer Buffer, in den eingefügt werden soll
 * @param pos Position im Buffer, an die eingefügt werden soll
 *        Muss Grösser 0 sein damit im Byte vorher der letzte Pegel erkannt werden kann.
 *        Wird auf die neue, nächste freie Position erhöht.
 */
static void addMFXSync(char *spiBuffer, unsigned int *pos) {
  unsigned char p0, p1;
  if (spiBuffer[*pos - 1]) {
    //Letzter Pegel war 1 -> invertiert arbeiten
    p0 = 0xFF;
    p1 = 0x00;
  }
  else {
    //Letzter Pegel war 0 -> "normal"
    p0 = 0x00;
    p1 = 0xFF;
  }
  spiBuffer[*pos+0] = p1; spiBuffer[*pos+1] = p1; //0
  spiBuffer[*pos+2] = p0; spiBuffer[*pos+3] = p1; //1
  spiBuffer[*pos+4] = p1; spiBuffer[*pos+5] = p0; //1 mit Regelverlettzung, keine Flanke zu Beginn
  spiBuffer[*pos+6] = p0; spiBuffer[*pos+7] = p1; //1 mit Regelverlettzung, keine Flanke zu Beginn
  spiBuffer[*pos+8] = p0; spiBuffer[*pos+9] = p0; //0
  *pos += 10;
}

/**
 * Fügt eine 0 oder 1 in den MFX Datenstrom ein.
 * zu Beginn immer Flankenwechsel, bei 0 keiner in der Mitte, bei 1 einer in der Mitte
 * @param value true=1, false=0 einfügen
 * @param spiBuffer Buffern in den eingefügt werden soll
 * @param pos Position, an die eingefügt werden soll, wird um eingefügte Bytes inkrementiert
 *        Muss > 0 sein.
 */
static void addMFX(bool value, char *spiBuffer, unsigned int *pos) {
  if (spiBuffer[*pos - 1]) {
    //Letzter Pegel war 1 -> Start mit 0
    spiBuffer[*pos] = 0x00;
    spiBuffer[*pos + 1] = 0x00;
  }
  else {
    //Letzter Pegel war 0 -> Start mit 1
    spiBuffer[*pos] = 0xFF;
    spiBuffer[*pos + 1] = 0xFF;
  }
  //Wenn eine 1 gewünscht ist, dann brauchen wir nun einen Wechsel in der Mitte
  if (value) {
    spiBuffer[*pos + 1] = ~spiBuffer[*pos + 1];
  }
  *pos += 2;
}

/**
 * Fügt das Sync Muster für den Start einer RDS Rückmeldung an.
 * @param spiBuffer Buffern in den eingefügt werden soll
 * @param pos Position, an die eingefügt werden soll, wird um eingefügte Bytes inkrementiert
 *        Muss > 0 sein.
 */ 
static void addRDSSync(char *spiBuffer, unsigned int *pos) {
  //1 Bit Rückkmeldung -> 11 Sync, 0011, Pause, Sync, Pause, 2 Sync
  unsigned int i;
  for (i=0; i<MFX_SYNC_COUNT_RDS; i++) {
    addMFXSync(spiBuffer, pos);
  }
  //Nun kommt 0011
  addMFX(false, spiBuffer, pos);
  addMFX(false, spiBuffer, pos);
  addMFX(true, spiBuffer, pos);
  addMFX(true, spiBuffer, pos);
}

/**
 * Konvertiert ein für SPI Ausgabe bestimmtest MFX Packet in den zur Ausgabe benötigten Bytestream.
 * @param packet Das zu konvertierende Packet. Im ersten Byte werden die Anzahl Bits angegegebn, die
 *               ab dem 2. Byte folgenden
 * @param spiBuffer Buffer zur Speicherung des SPI Bytestream. Muss mindestens folgende Grösse haben:
 *                  QMFX0PKT : (256+15+256/8)*2=606 Bytes (max. Daten + Sync + max. Stuffing)
 *                  QMFX1PKT : QMFX0PKT + 2 * (11 + 1 + 2 Sync + 4 Bit) + 2 * 6.4ms (128 Bytes) = 606 + 148 + 256 = 1010
 *                  QMFX8PKT : QMFX0PKT + 2 * (11 + 1 Sync + 4 Bit) + 146 * (23 + 15 + DataBits) / 8 = 1574
 *                  QMFX16PKT : 1720
 *                  QMFX32PKT : 2012
 *                  QMFX64PKT : 2596
 * @param mfxTyp Eine MFX Typ aus QMFX?PKT (Keine bis 8 Byte Rückmeldung)
 * @param rdsPos Nur bei 1-Bit Rückmeldung (QMFX1PKT): Rückgabe der Position im SPI Buffer, ab der die Rückmeldung erwartet wird.
 *               Wenn nicht QMFX1PKT darf null übergeben werden. 
 * @param rdsLen Analog rdsPos, die Länge der Rückmeldefensters.
 * @return Länge des spiBuffer.
 */
static unsigned int convertMFXPacketToSPIStream(bus_t busnumber, char *packet, char *spiBuffer, unsigned int mfxTyp, 
                                                unsigned int *rdsPos, unsigned int *rdsLen) {
  // Ein Bit in 2 Bytes, Baudrate ist so, dass diese Ausgabe 100us dauert
  // 1 : 0xFF, 0x00 oder 0x00, 0xFF (je nach Pegel vorher)
  // 0 : 0xFF, 0xFF oder 0x00, 0x00 (je nach Pegel vorher)
  //Ruhepegel am Anfang ist immer 0 (4 Byte) -> Start sync. ist immer gleich
  unsigned int i, j;
  unsigned int len = MFX_STARTSTOP_0_BYTES;
  for (i=0; i<MFX_STARTSTOP_0_BYTES; i++) {
    spiBuffer[i] = 0;
  }
  //Start Sync
  addMFXSync(spiBuffer, &len);
  unsigned int count1 = 0;
  //Nutzdaten
  for (i=0; i<(unsigned char)packet[0]; i++) {
    if (packet[(i / 8) + 1] & (1 << (i % 8))) {
      //1
      addMFX(true, spiBuffer, &len);
      count1++;
      if (count1 >= 8) {
        //Bitstuffing 0 einfügen
        addMFX(false, spiBuffer, &len);
        count1 = 0;
      }
    }
    else {
      //0
      count1 = 0;
      addMFX(false, spiBuffer, &len);
    }
  }
  switch (mfxTyp) {
    case QMFX0PKT:
      //Ende Sync
      addMFXSync(spiBuffer, &len);
      addMFXSync(spiBuffer, &len);
      //Vereinzelte MFX Loks funktionieren nicht zuverlässig. Ausser:
      //- 3. Sync am Ende
      //- Pause nach Abschluss.
      addMFXSync(spiBuffer, &len);
      for (i=0; i<MFX_STARTSTOP_0_BYTES; i++) {
        spiBuffer[len+i] = 0;
      }
      len += MFX_STARTSTOP_0_BYTES;
      break;
    case QMFX1PKT: //1 Bit Quittungsrückmeldung
    case QMFX1DPKT:
      addRDSSync(spiBuffer, &len);
      //6.4ms Pause mit 0, Rechnung passt noch in 32 Bit ...
      j = 1000 * SPI_BAUDRATE_MFX_2 / 8000000; //erst 1ms nach Fenster auswerten, da von Schaltflanke noch RDS Ansprecher dedektiert werden können
      *rdsPos = len + j;
      i = MFX_RDS_1BIT_DELAY * SPI_BAUDRATE_MFX_2 / 8000000; //8 Bit = 1 Byte und in usec
      //Mit anderem Pegel als vorher aufgehört wurde.
      memset(spiBuffer + len, ~spiBuffer[len - 1], i);
      len += i;
      *rdsLen = i - 2*j; //Und eine ms früher
      //1 Sync
      addMFXSync(spiBuffer, &len);
      //6.4ms Pause mit 0 oder 1 (anders als vorher aufgehört)
      memset(spiBuffer + len, ~spiBuffer[len - 1], i);
      len += i;
      //2 Sync
      addMFXSync(spiBuffer, &len);
      addMFXSync(spiBuffer, &len);
      break;
    case QMFX8PKT: //1 Byte RDS Rückmeldung
    case QMFX16PKT: //2 Bytes
    case QMFX32PKT: //4 Bytes
    case QMFX64PKT: //8 Bytes
      addRDSSync(spiBuffer, &len);
      //23 * einen 25us Impuls alle 912us
      //Um diese ungerade Zeit möglichst genau einzuhalten wird hir mit einzelnen Bits in der SPI Ausgabe gearbeitet.
      //Ein Bit mit Baudrate SPI_BAUDRATE_MFX_2 dauert 6.25us. Mit 146 Bits sind wir bei 912.5us. Das dürfte genau genug sein!
      //Der letzte Takt wird dann wieder auf ganze Bytes aufgerundet, das sollte auch nicht stören.
      //Ruhepegel ist immer. Wenn vorher mit 0 aufgeöhrt wird das erste Bit noch auf 1 gesetzt.
      bool start1 = (spiBuffer[len - 1] & 0x01) ? true : false;
      //Start ab aktueller len, ab dieser Position wird nun in Bit gerechnet (RaspPi SPI sendet MSB zuerst).
      unsigned int bitPos = 0;
      //Anzahl Bits für eine Periode berechnen (+1 um Integer Abrundung der Division aufzurunden)
      unsigned int bitsPerRDSSyncPeriod = (MFX_RDS_SYNC_IMPULS_CLK_USEC * SPI_BAUDRATE_MFX_2 / 1000000) + 1;
      unsigned int bitsPerRDSSyncImp = MFX_RDS_SYNC_IMPULS_USEC * SPI_BAUDRATE_MFX_2 / 1000000;
      for (i = 0; i<MFX_RDS_SYNC_IMPULS_COUNT; i++) {
        //Ruhepegel einfügen 
        for (j = 0; j < (bitsPerRDSSyncPeriod - bitsPerRDSSyncImp); j++) {
          if ((bitPos % 8) == 0) {
            //Neues Byte
            spiBuffer[len + (bitPos / 8)] = 0;
          }
          bitPos++;
        }
        //25us Impuls einfügen
        for (j = 0; j < bitsPerRDSSyncImp; j++) {
          spiBuffer[len + (bitPos / 8)] |= 0x80 >> (bitPos % 8);
          bitPos++;
        }
      }
      //Jetzt die Takte für die Datenbits, dazu kommt in die Mitte MFX_RDS_SYNC_IMPULS_CLK_USEC noch ein Impuls.
      //Anzahl Bits ist (Startmuster 010 + Anzahl Datenbits + 8 Checksumme + 4) = Anzahl Datenbits + 15.
      unsigned int rdsBitCount = 15;
      switch (mfxTyp) {
        case QMFX8PKT: //1 Byte RDS Rückmeldung
          rdsBitCount += 8;
          break;
        case QMFX16PKT: //2 Bytes
          rdsBitCount += 16;
          break;
        case QMFX32PKT: //4 Bytes
          rdsBitCount += 32;
          break;
        case QMFX64PKT: //8 Bytes
          rdsBitCount += 64;
          break;
      }
      //Mal zwei da mit halber Periode gearbeitet wird
      for (i = 0; i<(rdsBitCount * 2); i++) {
        //Ruhepegel einfügen 
        for (j = 0; j < (bitsPerRDSSyncPeriod / 2 - bitsPerRDSSyncImp); j++) {
          if ((bitPos % 8) == 0) {
            //Neues Byte
            spiBuffer[len + (bitPos / 8)] = 0;
          }
          bitPos++;
        }
        //25us Impuls einfügen
        for (j = 0; j < bitsPerRDSSyncImp; j++) {
          spiBuffer[len + (bitPos / 8)] |= 0x80 >> (bitPos % 8);
          bitPos++;
        }
      }
      //Ggf. nun noch Das erste Bit auf 1 um die letzte Flanke vor dem Sync. Muster korrekt zu haben
      int rdsSize = (bitPos+7) / 8;
      if (start1) {
        spiBuffer[len] |= 0x80;
      }
      len += rdsSize;
      //Und zum Abschluss noch ein letztes Sync
      addMFXSync(spiBuffer, &len);
      break;
  }
  
  if (len < 96) {
    syslog_bus(busnumber, DBG_WARN, "SPI Transfer < 96 Bytes (%d Bytes) -> no DMA Mode, wrong Timing", len);
  }
  return len;
}

static void init_MFXPacketPool(bus_t busnumber)
{
    int i;
    int result;

    result = pthread_mutex_init(&__DDL->mfx_pktpool_mutex, NULL);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_init() failed: %s (error = %d). Terminating!\n",
                   strerror(result), result);
        exit(1);
    }

    result = pthread_mutex_lock(&__DDL->mfx_pktpool_mutex);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_lock() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    for (i = 0; i <= MAX_MFX_ADDRESS; i++) {
        __DDL->MFXPacketPool.knownAddresses[i] = 0;
        __DDL->MFXPacketPool.packets[i] = 0;
    }
    __DDL->MFXPacketPool.NrOfKnownAddresses = 0;

    result = pthread_mutex_unlock(&__DDL->mfx_pktpool_mutex);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_unlock() failed: %s (errno = %d).",
                   strerror(result), result);
    }
    
    //Pipe zur MFX RDS Auftragsvergabe an RDS Thread
    result = pipe(__DDL->rdsPipeNew);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pipe() failed: %s (errno = %d).",
                   strerror(result), result);
    }
    if (startMFXThreads(busnumber, __DDL->rdsPipeNew[0])!= 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "startMFXThreads failed.");
    }
}

void update_MFXPacketPool(bus_t busnumber, int adr,
                           char const *const packet, int packet_size)
{
//  printf("update_MFXPacketPool(busnumber=%d, adr=%d, packet, packet_size=%d\n", busnumber, adr, packet_size);
    int i, found;
    int result;

    found = 0;
    for (i = 0; i <= __DDL->MFXPacketPool.NrOfKnownAddresses && !found; i++) {
        if (__DDL->MFXPacketPool.knownAddresses[i] == adr) {
            found = true;
        }
    }

    result = pthread_mutex_lock(&__DDL->mfx_pktpool_mutex);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_lock() failed: %s (errno = %d).",
                   strerror(result), result);
    }
    if (!__DDL->MFXPacketPool.packets[adr]) {
        __DDL->MFXPacketPool.packets[adr] = malloc(sizeof(tMFXPacket));
        if (__DDL->MFXPacketPool.packets[adr] == NULL) {
            syslog_bus(busnumber, DBG_ERROR,
                       "Memory allocation error in update_MFXPacketPool");
            return;
        }
    }
    memcpy(__DDL->MFXPacketPool.packets[adr]->packet, packet, packet_size);
    __DDL->MFXPacketPool.packets[adr]->packet_size = packet_size;
    __DDL->MFXPacketPool.packets[adr]->timeLastUpdate = time(NULL);

    if (!found) {
        __DDL->MFXPacketPool.knownAddresses[__DDL->MFXPacketPool.NrOfKnownAddresses] = adr;
        __DDL->MFXPacketPool.NrOfKnownAddresses++;
    }
    result = pthread_mutex_unlock(&__DDL->mfx_pktpool_mutex);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_unlock() failed: %s (errno = %d).",
                   strerror(result), result);
    }
}
/**************************************************************************/

static int checkShortcut(bus_t busnumber) 
{
    int arg;
    time_t short_now = 0;
    struct timeval tv_shortcut = { 0, 0 };

    // nur CTS wird eingelesen 
#if defined BANANAPI
    arg = (a20_gpio_lev(BAPI_CTS) == LOW ? TIOCM_CTS : 0);    
#elif defined RASPBERRYPI
    arg = (bcm2835_gpio_lev(RPI_CTS) == LOW ? TIOCM_CTS : 0);
#endif

    if (__DDL->CHECKSHORT != 2) arg =  ~arg;	// CTS inverted or not
    if (arg & TIOCM_CTS) {
    	if (__DDL->short_detected == 0) {
        	gettimeofday(&tv_shortcut, NULL);
            __DDL->short_detected =
                    tv_shortcut.tv_sec * 1000000 + tv_shortcut.tv_usec;
        }
        gettimeofday(&tv_shortcut, NULL);
        short_now = tv_shortcut.tv_sec * 1000000 + tv_shortcut.tv_usec;
        if (__DDL->SHORTCUTDELAY <=
                (short_now - __DDL->short_detected)) {
//                char buffer[1000];
//                sprintf(buffer, "__DDL->SHORTCUTDELAY=%u short_now=%u __DDL->short_detected=%u", __DDL->SHORTCUTDELAY, short_now, __DDL->short_detected);
//                syslog_bus(busnumber, DBG_FATAL, buffer);
        	syslog_bus(busnumber, DBG_INFO,
                           "Shortcut detected. Power on tracks stopped!");
            return 1;
        }
	}
    else {
    	__DDL->short_detected = 0;
    }
    return 0;
}


/**
 * Setzen einer HW Handshake Leitung.
 */
static void set_SerialLine(bus_t busnumber, int line, int mode)
{
    //ON setzt Leitung auf 0 wegen Invertierung durch RS232 Treiber
  #ifdef BANANAPI
    if (line == SL_RTS) a20_gpio_write(BAPI_RTS, mode == ON ? LOW : HIGH);
  #elif defined RASPBERRYPI
    if (line == SL_RTS) bcm2835_gpio_write(RPI_RTS, mode == ON ? LOW : HIGH);
  #endif
}

#define PAUSE_START 2
void send_packet(bus_t busnumber, char *packet,
                        int packet_size, int packet_type, int refresh)
{
    int i, laps;
    /* arguments for nanosleep and Maerklin loco decoders (19KHz) */
    /* all using busy waiting */
//    const static struct timespec rqtp_btw19K = { 0, 1250000 };
//SID, 22.03.09: gemäss http://home.mnet-online.de/modelleisenbahn-digital/Dig-tutorial-start.html sind es 4.2ms .....
//    const static struct timespec rqtp_end19K = { 0, 4200000 };       /* ==> busy waiting */
//    static struct timespec rqtp_end19K = { 0, 1700000 };       /* ==> busy waiting */
    /* arguments for nanosleep and Maerklin solenoids/function decoders (38KHz) */
//    const static struct timespec rqtp_btw38K = { 0, 625000 };
//SID, 04.01.08 : Wartezeit wäre theoretisch schon 850us, dies geht aber mit den LDT Dekodern nicht....
//    const static struct timespec rqtp_end38K = { 0,  3000000 }; /* ==> busy waiting     */
//    struct timespec rqtp_end38K = { 0, 850000 };        /* ==> busy waiting */
    //Anzugsverzögerung Umschaltrelais bei Mobile Station Mode 10ms
//    const static struct timespec mobileStationRelaisDelay = { 0,  10000000 };
    
    //Nur für MFX RDS 1-Bit Rückmeldung: Position und Länge im SPI Bytestream an der sich die Rückmeldung befinden muss.
    unsigned int mfxRdsPos, mfxRdsLen;
    mfxRdsPos = 0;
    
    struct spi_ioc_transfer spi;
    char spiBuffer[2700]; //Worst Case wenn maximales MFX Paket
    
    memset (&spi, 0, sizeof (spi)) ;
    spi.bits_per_word = 8;
    spi.tx_buf = (unsigned long)spiBuffer;
    switch (packet_type) {
        case QNBLOCOPKT:
        case QNBACCPKT:
          __DDL->spiLastMM = 0;
          spi.len = convertNMRAPacketToSPIStream(busnumber, packet, spiBuffer);
          spi.speed_hz = SPI_BAUDRATE_NMRA_2;
          spi.delay_usecs = 0;
          break;
        case QM1LOCOPKT:
        case QM2LOCOPKT:
        case QM2FXPKT:
        case QM1FUNCPKT:
        case QM1SOLEPKT:
        {
          //Für Märklin Motorola wird wie folgt kodiert (doppelte Baudrate):
          //- Paket mit
          //  - 0 -> 0xC0, 0x00, ich habe aber Schaltdekoder, die damit nicht funktionieren sondern einen ein wenig längeren Impuls wollen, also 0xE0, 0x00 ....
          //  - 1 -> 0xFF, 0xFC
          //  -> 18 * 2 = 36 Bytes
          //- 0 Bytes für Pause zwischen Paketen: 3 * (t 2 Bit, 208us / 416us) -> wegen doppelter Baudrate also 2 * 3 * 2 = 12 Bytes 0x00
          //- Paket Wiederholung 
          //- 0 Bytes für Pause nach Paket: 4.2ms (Lok), resp. 2.1ms (Schaltdekoder) -> wegen bei doppleter Baudrate 1 Byte 104us (Lok). 62us (Schalt) = 42 Bytes
          //Total also 36 + 12 + 36 + 42 = 126 Bytes -> DMA Mode!
          //Berechnung in Anzahl Bytes mit Zeiten und Baudrate für Lokbefehle
          //-> mit doppelter Baudrate für Schaltdekoder und halben Zeiten ergibt dies die selbe Anzahl Bytes (+2: Aufrunden und etwas Reserve)
          //Die verteilten Divisionen um die ns auf Sekunden zu bringen sind so, um Integer Overflows zu vermeiden.
          unsigned int pause_btw;
          unsigned int pause_end;
          if ((packet_type == QM1FUNCPKT) || (packet_type == QM1SOLEPKT)) {
            pause_btw = 12;     // rqtp_btw38K.tv_nsec / 1000 * SPI_BAUDRATE_MAERKLIN_FUNC_2 / 1000000 / 8;
            pause_end = 59;     // rqtp_end38K.tv_nsec / 1000 * SPI_BAUDRATE_MAERKLIN_FUNC_2 / 1000000 / 8 + 2;
            spi.speed_hz = SPI_BAUDRATE_MAERKLIN_FUNC_2;
          }
          else {
            pause_btw = 12;     // rqtp_btw19K.tv_nsec / 1000 * SPI_BAUDRATE_MAERKLIN_LOCO_2 / 1000000 / 8;
            pause_end = 42;     // rqtp_end19K.tv_nsec / 1000 * SPI_BAUDRATE_MAERKLIN_LOCO_2 / 1000000 / 8 + 2;
            spi.speed_hz = SPI_BAUDRATE_MAERKLIN_LOCO_2;
          }
          memset (spiBuffer, 0, sizeof (spiBuffer));
          if (! __DDL->spiLastMM) {
            //Wenn das letzte Paket kein MM Paket war, dann wird am Anfang noch eine Pause eingefügt
            spi.len = pause_end;
            if (ioctl(buses[busnumber].device.file.fd, SPI_IOC_MESSAGE(1), &spi) < 0) {
              syslog_bus(busnumber, DBG_FATAL, "Error SPI Transfer ioctl.");
              return;
            }
          }
          for (i=0; i<packet_size; i++) {
            if (packet[i]) {
              //1
              spiBuffer[PAUSE_START + i * 2] = 0xFF;
              spiBuffer[PAUSE_START + i * 2 + 1] = 0xFC;
            }
            else {
              //0
              if ((packet_type == QM1FUNCPKT) || (packet_type == QM1SOLEPKT)) {
                spiBuffer[PAUSE_START + i * 2] = 0xE0;
              }
              else {
                spiBuffer[PAUSE_START + i * 2] = 0xC0;
              }
              spiBuffer[PAUSE_START + i * 2 + 1] = 0x00;
            }
          }
          //Wiederholung nach Pause "dazwischen"
          memcpy(&(spiBuffer[PAUSE_START + packet_size * 2 + pause_btw]), &(spiBuffer[PAUSE_START]), packet_size * 2);
          //Länge ist 2 Mal Paket, Pause dazwischen, Pause Ende
          spi.len = PAUSE_START + 2 * (packet_size * 2) + pause_btw + pause_end;
          __DDL->spiLastMM = 1;
          break;
        }
        case QMFX0PKT:
        case QMFX1PKT:
        case QMFX1DPKT:
        case QMFX8PKT:
        case QMFX16PKT:
        case QMFX32PKT:
        case QMFX64PKT:
          __DDL->spiLastMM = 0;
          spi.len = convertMFXPacketToSPIStream(busnumber, packet, spiBuffer, packet_type, &mfxRdsPos, &mfxRdsLen);
          spi.speed_hz = SPI_BAUDRATE_MFX_2;
          spi.delay_usecs = 0;
          if (spi.len > sizeof(spiBuffer)) {
            //Buffer war zu klein, es wurde Speicher überschrieben.
            syslog_bus(busnumber, DBG_FATAL,
                       "MFX SPI Buffer Override. Buffersize=%d, Bytes write=%d",
                       sizeof(spiBuffer), spi.len);
            /*What to do now ?*/
            exit(1);
          }
          break;
    }

    switch (packet_type) {
        case QM1LOCOPKT:
        case QM2LOCOPKT:
            if (refresh) {
              laps = 1;
            }
            else {
              laps = 4;       /* YYTV 9 */
            }
            for (i = 0; i < laps; i++) {
                if (ioctl(buses[busnumber].device.file.fd, SPI_IOC_MESSAGE(1), &spi) < 0) {
                  syslog_bus(busnumber, DBG_FATAL, "Error SPI Transfer ioctl.");
                  return;
                }
            }
            break;
        case QM2FXPKT:
            if (refresh)
                laps = 1;
            else
                laps = 3;       /* YYTV 6 */
            for (i = 0; i < laps; i++) {
                if (ioctl(buses[busnumber].device.file.fd, SPI_IOC_MESSAGE(1), &spi) < 0) {
                  syslog_bus(busnumber, DBG_FATAL, "Error SPI Transfer ioctl.");
                  return;
                }
            }
            break;
        case QM1FUNCPKT:
        case QM1SOLEPKT:
            for (i = 0; i < 2; i++) {
                if (ioctl(buses[busnumber].device.file.fd, SPI_IOC_MESSAGE(1), &spi) < 0) {
                  syslog_bus(busnumber, DBG_FATAL, "Error SPI Transfer ioctl.");
                  return;
                }
            }
            break;
        case QNBLOCOPKT:
        case QNBACCPKT:
            if (ioctl(buses[busnumber].device.file.fd, SPI_IOC_MESSAGE(1), &spi) < 0) {
                syslog_bus(busnumber, DBG_FATAL, "Error SPI Transfer ioctl.");
                return;
            }
            if (ioctl(buses[busnumber].device.file.fd, SPI_IOC_MESSAGE(1), &spi_nmra_idle) < 0) {
                syslog_bus(busnumber, DBG_FATAL, "Error SPI Transfer ioctl.");
                return;
            }
            if (ioctl(buses[busnumber].device.file.fd, SPI_IOC_MESSAGE(1), &spi) < 0) {
                syslog_bus(busnumber, DBG_FATAL, "Error SPI Transfer ioctl.");
                return;
            }
            break;
        case QMFX0PKT:
        case QMFX1PKT:
        case QMFX1DPKT:
        case QMFX8PKT:
        case QMFX16PKT:
        case QMFX32PKT:
        case QMFX64PKT:	{	}
            //Die "1 Bit" Rückmeldungen (RDS Signal vorhanden) erfolgen direkt über SPI MISO
            unsigned int spiRxBufferSize = 0;
            MFXRDSBits mfxRDSBits;
            switch (packet_type) {
              case QMFX1PKT:
                //Es wird ein Eingangsbuffer benötigt
                spiRxBufferSize = spi.len;
                break;
              case QMFX8PKT:
                mfxRDSBits = RDS_8;
                write(__DDL->rdsPipeNew[1], &mfxRDSBits, sizeof(mfxRDSBits));
                break;
              case QMFX16PKT:
                mfxRDSBits = RDS_16;
                write(__DDL->rdsPipeNew[1], &mfxRDSBits, sizeof(mfxRDSBits));
                break;
              case QMFX32PKT:
                mfxRDSBits = RDS_32;
                write(__DDL->rdsPipeNew[1], &mfxRDSBits, sizeof(mfxRDSBits));
                break;
              case QMFX64PKT:
                mfxRDSBits = RDS_64;
                write(__DDL->rdsPipeNew[1], &mfxRDSBits, sizeof(mfxRDSBits));
                break;
            }
            char spiRxBuffer[spiRxBufferSize];
            if (spiRxBufferSize > 0) {
              spi.rx_buf = (unsigned long)spiRxBuffer;
            }
            //Ausgabe, doppelt für Befehle ohne Rückmeldung
            for (i=0; i<(packet_type == QMFX0PKT ? 2 : 1); i++) {
              if (ioctl(buses[busnumber].device.file.fd, SPI_IOC_MESSAGE(1), &spi) < 0) {
                syslog_bus(busnumber, DBG_FATAL, "Error SPI Transfer ioctl.");
                return;
              }
            }
            //Auswertung "1 Bit" Rückmeldung
            if (spiRxBufferSize > 0) {
              //Wenn im Rückmeldefenster 50% auf 1 gesetzt ist, hat ein MFX Dekoder geantwortet
              unsigned int count1 = 0;
              for (i = 0; i < mfxRdsLen; i++) {
                count1 += __builtin_popcount(spiRxBuffer[mfxRdsPos + i]);
              }
              //50% der Bits gesetzt, es wird wohl tatsächlich ein Dekoder geantwortet haben und keine Störung sein
              bool rdsOK = (count1 >= (8*mfxRdsLen/2));
              //RDS Rückmeldung absenden
              tMFXRDSFeedback rdsFeedback;
              rdsFeedback.ok = true; //Bei 1 Bit Rückmeldungen immer ok.
              rdsFeedback.bits = 1;
              rdsFeedback.feedback[0] = rdsOK;
              newMFXRDSFeedback(rdsFeedback);
            }
            break;
    }
}

/**
 * Refresh Paket der nächsten Lok senden.
 * Es werden alle Loks jedes aktivierten Protokolles der Reihe nach berücksichtigt.
 * @param busnumber
 * @param fast false: alle Loks
 *             true: nur Loks die innerhalb der letzten FAST_REFRESH_TIMEOUT Sekunden
 *                   ein neues Kommando erhielten
 * @return false wenn keine Lok zum Refresh vorhanden sein sollte.
 */
static bool refresh_loco_one(bus_t busnumber, bool fast) {
  tRefreshInfo *refreshInfo = fast ? &__DDL->refreshInfoFast : &__DDL->refreshInfo;

  int adr;
//  int result;
  bool refreshGesendet = false;
  bool busComplete = false;

  if (refreshInfo -> protocol_refresh & EP_MAERKLIN) {
    adr = __DDL->MaerklinPacketPool.knownAddresses[refreshInfo -> last_refreshed_maerklin_loco];
    if (fast != ((time(NULL) - __DDL->MaerklinPacketPool.packets[adr].timeLastUpdate) > FAST_REFRESH_TIMEOUT)) {
      //Bei "Fast" nur die Loks, die nicht zu alt sind,
      //bei "nicht fast" / "Normal" nur die Loks, die alt sind
      refreshGesendet = true;
      send_packet(busnumber, __DDL->MaerklinPacketPool.packets[adr].packet, 18, QM2LOCOPKT, true);
      //bei jeden Durchgang (alle Loks) das nächste Fx Paket
      send_packet(busnumber, __DDL->MaerklinPacketPool.packets[adr].f_packets[refreshInfo -> last_refreshed_maerklin_fx], 18, QM2FXPKT, true);
    }
    do {
      refreshInfo -> last_refreshed_maerklin_loco++;
      if (refreshInfo -> last_refreshed_maerklin_loco >= __DDL->MaerklinPacketPool.NrOfKnownAddresses) {
        refreshInfo -> last_refreshed_maerklin_loco = 0;
        busComplete = true;
        //Beim nächsten Durchgang das nächste Fx Paket
        refreshInfo -> last_refreshed_maerklin_fx++;
        if (refreshInfo -> last_refreshed_maerklin_fx >= 4) {
          refreshInfo -> last_refreshed_maerklin_fx = 0;
        }
        break;
      }
      //Bei Fast Zyklus wird die nächste Lok mit neuem Kommando gesucht,
      //bei "normal" die nächste Lok ohne neues Kommando
      adr = __DDL->MaerklinPacketPool.knownAddresses[refreshInfo -> last_refreshed_maerklin_loco];
    } while (fast != ((time(NULL) - __DDL->MaerklinPacketPool.packets[adr].timeLastUpdate) <= FAST_REFRESH_TIMEOUT));
  }
  if (refreshInfo -> protocol_refresh & EP_NMRADCC) {
    adr = __DDL->NMRAPacketPool.knownAddresses[refreshInfo -> last_refreshed_nmra_loco];
    if (adr >= 0) {
      if (fast != ((time(NULL) - __DDL->NMRAPacketPool.packets[adr] -> timeLastUpdate) > FAST_REFRESH_TIMEOUT)) {
        refreshGesendet = true;
        send_packet(busnumber,
                    __DDL->NMRAPacketPool.packets[adr]->packet,
                    __DDL->NMRAPacketPool.packets[adr]->packet_size,
                    QNBLOCOPKT, true);
        send_packet(busnumber,
                    __DDL->NMRAPacketPool.packets[adr]->fx_packet,
                    __DDL->NMRAPacketPool.packets[adr]->
                    fx_packet_size, QNBLOCOPKT, true);
      }
      do {
        refreshInfo -> last_refreshed_nmra_loco++;
        if (refreshInfo -> last_refreshed_nmra_loco >= __DDL->NMRAPacketPool.NrOfKnownAddresses) {
          refreshInfo -> last_refreshed_nmra_loco = 0;
          busComplete = true;
          break;
        }
        //Bei Fast Zyklus wird die nächste Lok mit neuem Kommando gesucht,
        //bei "normal" die nächste Lok ohne neues Kommando
        adr = __DDL->NMRAPacketPool.knownAddresses[refreshInfo -> last_refreshed_nmra_loco];
      } while (fast != ((time(NULL) - __DDL->NMRAPacketPool.packets[adr] -> timeLastUpdate) <= FAST_REFRESH_TIMEOUT));
    }
    else {
      //Keine DCC Lok vorhanden -> mit nächstem Protokoll weitermachen
      busComplete = true;
  	}
  }
    
  if (refreshInfo -> protocol_refresh & EP_MFX) {
    //MFX Refresh Cycle
    adr = __DDL->MFXPacketPool.knownAddresses[refreshInfo -> last_refreshed_mfx_loco];
    if (adr > 0) {
      if (fast != ((time(NULL) - __DDL->MFXPacketPool.packets[adr] -> timeLastUpdate) > FAST_REFRESH_TIMEOUT)) {
        refreshGesendet = true;
        send_packet(busnumber,
                    __DDL->MFXPacketPool.packets[adr]->packet,
                    __DDL->MFXPacketPool.packets[adr]->packet_size,
                    QMFX0PKT, true);
      }
      do {
        refreshInfo -> last_refreshed_mfx_loco++;
        if (refreshInfo -> last_refreshed_mfx_loco >= __DDL->MFXPacketPool.NrOfKnownAddresses) {
          refreshInfo -> last_refreshed_mfx_loco = 0;
          busComplete = true;
          break;
        }
        //Bei Fast Zyklus wird die nächste Lok mit neuem Kommando gesucht,
        //bei "normal" die nächste Lok ohne neues Kommando
        adr = __DDL->NMRAPacketPool.knownAddresses[refreshInfo -> last_refreshed_mfx_loco];
      } while (fast != ((time(NULL) - __DDL->NMRAPacketPool.packets[adr] -> timeLastUpdate) <= FAST_REFRESH_TIMEOUT));
    }
    else {
      //Keine MFX Lok vorhanden -> mit nächstem Protokoll weitermachen
      busComplete = true;
    }
  }

  if (busComplete) {
    //Im nächsten Zyklus kommt das nächste aktivierte Protokoll an die Reihe
    do {
      refreshInfo -> protocol_refresh <<= 1;
      if (refreshInfo -> protocol_refresh == 0) {
        //Wieder von vorne beginnen
        refreshInfo -> protocol_refresh = 1;
      }
    } while ((__DDL->ENABLED_PROTOCOLS & refreshInfo -> protocol_refresh) == 0);
  }
  return refreshGesendet;
}

/**
 * Refresh Paket der nächsten Lok senden.
 * Es werden alle Loks jedes aktivierten Protokolles der Reihe nach berücksichtigt.
 * Wenn Loks vorhanden sind, die in den letzten FAST_REFRESH_TIMEOUT ein neues 
 * Kommando erhielten (Fast Refresh) werden diese jeden 2. Durchgang berücksichtigt.
 * Grund:
 * Bei Anlagen mit vielen Loks dauert ein ganzer Refresh Durchlauf relativ lange.
 * Da jedoch in der Regel wenig dieser Loks gleichzeitig in Betrieb sind erhaltend
 * diese häufiger einen Refresh.
 * @param busnumber
 * @return false wenn keine Lok zum Refresh vorhanden sein sollte.
 */
static bool refresh_loco(bus_t busnumber)
{
  if (__DDL->lastRefreshFast) {
    //letzter Refresh war "Fast". "Normaler" Refresh, nächster ist wieder ein "Fast"
    __DDL->lastRefreshFast = false;
    return refresh_loco_one(busnumber, false);
  }
  //letzter Refresh war kein "Fast" -> "Fast" Refresh versuchen
  if (refresh_loco_one(busnumber, true)) {
    //"Fast" Refresh gesendet, nachher kommt wieder ein "normaler".
    __DDL->lastRefreshFast = true;
    return true;
  }
  //Keine Lok für "Fast" Refresh vorhanden, "normalen" refresh senden
  return refresh_loco_one(busnumber, false);
}

/* calculate difference between two time values and return the
 * difference in microseconds */
long int compute_delta(struct timeval tv1, struct timeval tv2)
{
    long int delta_sec;
    long int delta_usec;

    delta_sec = tv2.tv_sec - tv1.tv_sec;
    if (delta_sec == 0)
        delta_usec = tv2.tv_usec - tv1.tv_usec;
    else {
        if (delta_sec == 1)
            delta_usec = tv2.tv_usec + (1000000 - tv1.tv_usec);
        else
            delta_usec =
                1000000 * (delta_sec - 1) + tv2.tv_usec + (1000000 -
                                                           tv1.tv_usec);
    }
    return delta_usec;
}


/* check if shortcut or emergengy break happened
   return "true" if power is off, return "false" if power is on */
static bool power_is_off(bus_t busnumber)
{
    static struct timeval t_rts_on;
    char msg[110];
    if (__DDL->CHECKSHORT) {
		if (buses[busnumber].power_state) {
			if (checkShortcut(busnumber) == 1) {
            	buses[busnumber].power_state = 0;
                buses[busnumber].power_changed = 1;
                strcpy(buses[busnumber].power_msg, "SHORTCUT DETECTED");
                infoPower(busnumber, msg);
                enqueueInfoMessage(msg);
             }
        }
    }
    if (buses[busnumber].power_changed == 1) {
        if (buses[busnumber].power_state == 0) {
            set_SerialLine(busnumber, SL_RTS, OFF);
            syslog_bus(busnumber, DBG_INFO, "Refresh cycle stopped.");
        }
        if (buses[busnumber].power_state == 1) {
            gettimeofday(&t_rts_on, NULL);
            set_SerialLine(busnumber, SL_RTS, ON);
            syslog_bus(busnumber, DBG_INFO, "Refresh cycle started.");
        }
        buses[busnumber].power_changed = 0;
        infoPower(busnumber, msg);
        enqueueInfoMessage(msg);
    }
    if (buses[busnumber].power_state == 0) {
        if (usleep(1000) == -1) {
            syslog_bus(busnumber, DBG_ERROR,
                       "usleep() failed: %s (errno = %d)\n",
                       strerror(errno), errno);
        }
        return true;
    }
    return false;
}

static void *thr_refresh_cycle(void *v)
{
//    ssize_t wresult;
    int result;
    struct sched_param sparam;
    int policy;
    int packet_size;
    int packet_type;
    char packet[PKTSIZE];
    int addr;
    struct _thr_param *tp = v;
    bus_t busnumber = tp->busnumber;
    struct timeval tv1;     // tv2;
    struct timezone tz;
    /* argument for nanosleep to do non-busy waiting */
    static struct timespec rqtp_sleep = { 0, 2500000 };

    nanosleep_DDL = nanosleep;
    if (__DDL->oslevel == 1) {
// da eh nur nanosleep gerufen wird        nanosleep_DDL = krnl26_nanosleep;

        result = pthread_getschedparam(pthread_self(), &policy, &sparam);
        if (result != 0) {
            syslog_bus(busnumber, DBG_ERROR,
                       "pthread_getschedparam() failed: %s (errno = %d).",
                       strerror(result), result);
            /*TODO: Add an expressive error message */
            pthread_exit((void *) 1);
        }
        
        sparam.sched_priority = 10;
        result =
            pthread_setschedparam(pthread_self(), SCHED_FIFO, &sparam);
        if (result != 0) {
            syslog_bus(busnumber, DBG_ERROR,
                       "pthread_setschedparam() failed: %s (errno = %d).",
                       strerror(result), result);
            /*TODO: Add an expressive error message */
            pthread_exit((void *) 1);
        }
    }

    struct spi_ioc_transfer spi_idlePacket;

    memset (&spi_idlePacket, 0, sizeof (spi_idlePacket)) ;
    spi_idlePacket.tx_buf =  (unsigned long)__DDL->idle_data;
    spi_idlePacket.len = MAXDATA;
    spi_idlePacket.bits_per_word = 8;
    spi_idlePacket.speed_hz = SPI_BAUDRATE_MAERKLIN_LOCO;
   
    gettimeofday(&tv1, &tz);
    for (;;) {
        if (power_is_off(busnumber)) {
           nanosleep_DDL(&rqtp_sleep, &__DDL->rmtp);
           continue;
        }

        /* Check if there are new commands and send them. */
        packet_type = queue_get(busnumber, &addr, packet, &packet_size);
        if (packet_type > QNOVALIDPKT) {
            while (packet_type > QNOVALIDPKT) {
                if (packet_type != QOVERRIDE) { //Nicht mehr gültiges Paket, bereits überholt.
                  /* if power is off, wait here until power is turned on
                   * again */
                  if (power_is_off(busnumber)) {
                    usleep(10000); //alle 10ms reicht aus
                    continue;
                  }

                  send_packet(busnumber, packet, packet_size,
                              packet_type, false);
                  if (__DDL->ENABLED_PROTOCOLS == (EP_MAERKLIN | EP_NMRADCC)) {
                      if (ioctl(buses[busnumber].device.file.fd, SPI_IOC_MESSAGE(1), &spi_nmra_idle) < 0) {
                        syslog_bus(busnumber, DBG_FATAL, "Error SPI Transfer ioctl.");
                      }
                  }
                }
                packet_type =
                    queue_get(busnumber, &addr, packet, &packet_size);
            }
        }

        /* If there are no new commands, send a loco state refresch. */
        else {
          if (power_is_off(busnumber) /*|| __DDL->MOBILE_STATION_MODE*/ ) { 
            //Im "Mobile Station Mode" nur GA's, keine Loks.
            nanosleep_DDL(&rqtp_sleep, &__DDL->rmtp);
            continue;
          }

          if (! refresh_loco(busnumber)) {
              //Keine Lok für Refreshkommando vorhanden -> Idle Data senden damit kein DC am Gleis anliegt.
              __DDL->spiLastMM = 0;
              if (ioctl(buses[busnumber].device.file.fd, SPI_IOC_MESSAGE(1), &spi_idlePacket) < 0) {
                syslog_bus(busnumber, DBG_FATAL, "Error SPI Transfer ioctl.");
              }
          }
        }
    }

    return NULL;
}

static int init_gl_DDL(bus_t bus, gl_data_t * gl, char *optData)
{
/* called old code in original version, TODO replace by new */
    switch (gl->protocol) {
        case 'M':              /* Motorola Codes */
            return (gl->protocolversion > 0 && gl->protocolversion <= 5) ? SRCP_OK : SRCP_WRONGVALUE;
            break;
        case 'N':
            return (gl->protocolversion > 0 && gl->protocolversion <= 5) ? SRCP_OK : SRCP_WRONGVALUE;
            break;
        case 'X': //MFX
            if (gl->protocolversion < 0 || gl->protocolversion > 1) {
                return SRCP_WRONGVALUE;
            }
            //Bei MFX müssen folgende zusätzlichen Daten vorhanden sein:
            //UID "LokName" fx0 ... fx15
            // - UID: 32 Bit Dekoder UID
            // - "LokName" Der Name der Lok
            // - fx0 .. fx15: 16 Fx Definition, je eine dez. Zahl die die 3 Byte Gruppe und 2 Bytes zusätzliche Informationen enthält.
            int nelem = sscanf(optData, "%u \"%[^\"]\" %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u", 
                               &gl->optData.mfx.uid, gl->optData.mfx.name, 
                               &gl->optData.mfx.fx[0], &gl->optData.mfx.fx[1], &gl->optData.mfx.fx[2], &gl->optData.mfx.fx[3],
                               &gl->optData.mfx.fx[4], &gl->optData.mfx.fx[5], &gl->optData.mfx.fx[6], &gl->optData.mfx.fx[7],
                               &gl->optData.mfx.fx[8], &gl->optData.mfx.fx[9], &gl->optData.mfx.fx[10], &gl->optData.mfx.fx[11],
                               &gl->optData.mfx.fx[12], &gl->optData.mfx.fx[13], &gl->optData.mfx.fx[14], &gl->optData.mfx.fx[15]);
            int result = nelem >= 18 ? SRCP_OK : SRCP_LISTTOOSHORT;
            if (result == SRCP_OK) {
              //Mitteilung an MFX damit ggf. neue Lokadresse gesetzt werden kann
              newGLInit(gl->id, gl->optData.mfx.uid);
            }
            return result;
            break;
    }
    return SRCP_UNSUPPORTEDDEVICEPROTOCOL;
}

/**
 * Protokoll spezifische Parameter für INIT GL Info liefern.
 * @param gl Lok, von der die Infor geliefert werden sollen.
 * @param msg Message, an die die Infos gehängt werden sollen
 */
static void describe_gl_DDL(gl_data_t *gl, char *msg) {
  switch (gl->protocol) {
    case 'X':
      describeGLmfx(gl, msg);
      break;
  }
}

static int init_ga_DDL(ga_data_t * ga)
{
    switch (ga->protocol) {
        case 'M':              /* Motorola Codes */
            return SRCP_OK;
            break;
        case 'N':
            return SRCP_OK;
            break;
    }
    return SRCP_UNSUPPORTEDDEVICEPROTOCOL;
}


int readconfig_DDL(xmlDocPtr doc, xmlNodePtr node, bus_t busnumber)
{
    struct utsname utsBuffer;
    char buf[3];
    int result;

    buses[busnumber].driverdata = malloc(sizeof(struct _DDL_DATA));

    if (buses[busnumber].driverdata == NULL) {
        syslog_bus(busnumber, DBG_ERROR,
                   "Memory allocation error in module '%s'.", node->name);
        return 0;
    }

    buses[busnumber].type = SERVER_DDL;
    buses[busnumber].init_func = &init_bus_DDL;
    buses[busnumber].init_gl_func = &init_gl_DDL;
    buses[busnumber].describe_gl_func = &describe_gl_DDL;
    buses[busnumber].init_ga_func = &init_ga_DDL;

    buses[busnumber].thr_func = &thr_sendrec_DDL;

    strcpy(buses[busnumber].description,
           "GA GL SM POWER LOCK DESCRIPTION");
    __DDL->oslevel = 1;         /* kernel 2.6 */

    /* we need to check for kernel version below 2.6 or below */
    /* the following code breaks if a kernel version 2.10 will ever occur */
    result = uname(&utsBuffer);
    if (result == -1) {
        syslog_bus(busnumber, DBG_FATAL,
                   "uname() failed: %s (errno = %d).",
                   strerror(result), result);
        /*What to do now */
    }
    snprintf(buf, sizeof(buf), "%c%c", utsBuffer.release[0],
             utsBuffer.release[2]);

    if (atoi(buf) > 25) {
        __DDL->oslevel = 1;     /* kernel 2.6 or later */
    }
    else {
        __DDL->oslevel = 0;     /* kernel 2.5 or earlier */
    }

    __DDL->number_gl = 255;
    __DDL->number_ga = 324;

    __DDL->CHECKSHORT = false;  /* default no shortcut checking */
    __DDL->SHORTCUTDELAY = 0;   /* usecs shortcut delay         */
    __DDL->ENABLED_PROTOCOLS = (EP_MAERKLIN | EP_NMRADCC);      /* enabled p's */
    __DDL->NMRA_GA_OFFSET = 0;  /* offset for ga base address 0 or 1  */
    __DDL->PROGRAM_TRACK = 1;   /* 0: suppress SM commands to PT address */

    xmlNodePtr child = node->children;
    xmlChar *txt = NULL;

    while (child != NULL) {

        if ((xmlStrncmp(child->name, BAD_CAST "text", 4) == 0) ||
            (xmlStrncmp(child->name, BAD_CAST "comment", 7) == 0)) {
            /* just do nothing, it is only formatting text or a comment */
        }

        else if (xmlStrcmp(child->name, BAD_CAST "number_gl") == 0) {
            txt = xmlNodeListGetString(doc, child->xmlChildrenNode, 1);
            if (txt != NULL) {
                __DDL->number_gl = atoi((char *) txt);
                xmlFree(txt);
            }
        }

        else if (xmlStrcmp(child->name, BAD_CAST "number_ga") == 0) {
            txt = xmlNodeListGetString(doc, child->xmlChildrenNode, 1);
            if (txt != NULL) {
                __DDL->number_ga = atoi((char *) txt);
                xmlFree(txt);
            }
        }

        else if (xmlStrcmp
                 (child->name, BAD_CAST "enable_checkshort_checking")
                 == 0) {
            txt = xmlNodeListGetString(doc, child->xmlChildrenNode, 1);
            if (txt != NULL) {
                __DDL->CHECKSHORT =
                    (xmlStrcmp(txt, BAD_CAST "yes") == 0) ? true : false;
                if (xmlStrcmp(txt, BAD_CAST "inverse") == 0)
                	__DDL->CHECKSHORT = 2;
                xmlFree(txt);
            }
        }

        else if (xmlStrcmp(child->name, BAD_CAST "enable_maerklin") == 0) {
            txt = xmlNodeListGetString(doc, child->xmlChildrenNode, 1);
            if (txt != NULL) {
                if (xmlStrcmp(txt, BAD_CAST "yes") == 0)
                    __DDL->ENABLED_PROTOCOLS |= EP_MAERKLIN;
                else
                    __DDL->ENABLED_PROTOCOLS &= ~EP_MAERKLIN;

                xmlFree(txt);
            }
        }

        else if (xmlStrcmp(child->name, BAD_CAST "enable_nmradcc") == 0) {
            txt = xmlNodeListGetString(doc, child->xmlChildrenNode, 1);
            if (txt != NULL) {
                if (xmlStrcmp(txt, BAD_CAST "yes") == 0)
                    __DDL->ENABLED_PROTOCOLS |= EP_NMRADCC;
                else
                    __DDL->ENABLED_PROTOCOLS &= ~EP_NMRADCC;

                xmlFree(txt);
            }
        }

        else if (xmlStrcmp(child->name, BAD_CAST "enable_mfx") == 0) {
            txt = xmlNodeListGetString(doc, child->xmlChildrenNode, 1);
            if (txt != NULL) {
                __DDL->uid = strtoul ((char *)txt, NULL, 10);
                if (__DDL->uid) {
                  __DDL->ENABLED_PROTOCOLS |= EP_MFX;
                }
                else {
                  __DDL->ENABLED_PROTOCOLS &= ~EP_MFX;
                }
                xmlFree(txt);
            }
            else {
              __DDL->ENABLED_PROTOCOLS &= ~EP_MFX;
            }
        }

        else if (xmlStrcmp(child->name, BAD_CAST "shortcut_failure_delay")
                 == 0) {
            txt = xmlNodeListGetString(doc, child->xmlChildrenNode, 1);
            if (txt != NULL) {
                __DDL->SHORTCUTDELAY = atoi((char *) txt);
                xmlFree(txt);
            }
        }

        else if (xmlStrcmp(child->name, BAD_CAST "nmra_ga_offset") == 0) {
            txt = xmlNodeListGetString(doc, child->xmlChildrenNode, 1);
            if (txt != NULL) {
                __DDL->NMRA_GA_OFFSET = atoi((char *) txt);
                xmlFree(txt);
            }
        }

        else if (xmlStrcmp(child->name, BAD_CAST "program_track") == 0) {
            txt = xmlNodeListGetString(doc, child->xmlChildrenNode, 1);
            if (txt != NULL) {
                __DDL->PROGRAM_TRACK = (xmlStrcmp(txt, BAD_CAST "yes")
                                        == 0) ? true : false;
                xmlFree(txt);
            }
        }
        else
            syslog_bus(busnumber, DBG_WARN,
                       "WARNING, unknown tag found: \"%s\"!\n",
                       child->name);;

        child = child->next;
    }                           /* while */

    if (init_GA(busnumber, __DDL->number_ga)) {
        __DDL->number_ga = 0;
        syslog_bus(busnumber, DBG_ERROR,
                   "Can't create array for accessories");
    }

    if (init_GL(busnumber, __DDL->number_gl)) {
        __DDL->number_gl = 0;
        syslog_bus(busnumber, DBG_ERROR,
                   "Can't create array for locomotives");
    }

    return (1);
}

/* Initialisiere den Bus, signalisiere Fehler */
/* Einmal aufgerufen mit busnummer als einzigem Parameter */
/* return code wird ignoriert (vorerst) */
int init_bus_DDL(bus_t busnumber)
{
    int i;
    static char protocols[3] = { '\0', '\0', '\0' };
    int protocol = 0;

    syslog_bus(busnumber, DBG_INFO, "DDL init with debug level %d",
               buses[busnumber].debuglevel);

    if (buses[busnumber].device.file.fd <= 0) {
      buses[busnumber].device.file.fd = init_lineDDL(busnumber);
    }

    //GPIO In-Output für Boosterstuerung setzen:
    #if defined BANANAPI
        /* TODO for BPi => complete this section */ 
        a20_gpio_write(BAPI_RTS, HIGH);     // Booster deaktiviert
        a20_gpio_set_fsel(BAPI_RTS, A20_GPIO_FSEL_OUTP); 
        a20_gpio_set_fsel(BAPI_CTS, A20_GPIO_FSEL_INPT); 
    #elif defined RASPBERRYPI
        // RPI_DTR und RPI_RTS -> Output
        // RPI_CTS und RPI_DSR -> Input
        bcm2835_gpio_fsel(RPI_DTR, BCM2835_GPIO_FSEL_OUTP);
        bcm2835_gpio_fsel(RPI_RTS, BCM2835_GPIO_FSEL_OUTP);
        bcm2835_gpio_fsel(RPI_CTS, BCM2835_GPIO_FSEL_INPT);
        bcm2835_gpio_set_pud(RPI_CTS, BCM2835_GPIO_PUD_OFF); //Kein Pull-Up-Down
        bcm2835_gpio_fsel(RPI_DSR, BCM2835_GPIO_FSEL_INPT);
        bcm2835_gpio_set_pud(RPI_DSR, BCM2835_GPIO_PUD_OFF); //Kein Pull-Up-Down
    #endif

    __DDL->short_detected = 0;

    __DDL->queue_initialized = false;
    queue_init(busnumber);

    /* generate idle_data */
    for (i = 0; i < MAXDATA; i++)
        __DDL->idle_data[i] = 0x55;     /* 0x55 = 01010101 */   /* TODO was soll das? */  
    __DDL->idle_data[MAXDATA-1] = 0;        
    for (i = 0; i < PKTSIZE; i++)
        __DDL->NMRA_idle_data[i] = 0x55;
    __DDL->NMRA_idle_data_size = PKTSIZE;

    /*
     * ATTENTION:
     *   If NMRA dcc mode is activated __DDL->idle_data[] and
     *   __DDL->NMRA_idle_data must be overridden from init_NMRAPacketPool().
     *   This means, that init_NMRAPacketPool()
     *   must be called after init_MaerklinPacketPool().
     */

    __DDL->refreshInfo.last_refreshed_maerklin_loco = 0;
    __DDL->refreshInfo.last_refreshed_maerklin_fx = -1;
    __DDL->refreshInfo.last_refreshed_nmra_loco = 0;
    __DDL->refreshInfo.last_refreshed_mfx_loco = 0;
    __DDL->refreshInfo.protocol_refresh = 1;
    __DDL->refreshInfoFast.last_refreshed_maerklin_loco = 0;
    __DDL->refreshInfoFast.last_refreshed_maerklin_fx = -1;
    __DDL->refreshInfoFast.last_refreshed_nmra_loco = 0;
    __DDL->refreshInfoFast.last_refreshed_mfx_loco = 0;
    __DDL->refreshInfoFast.protocol_refresh = 1;
    __DDL->lastRefreshFast = false;
    
    if (__DDL->ENABLED_PROTOCOLS & EP_MAERKLIN) {
        init_MaerklinPacketPool(busnumber);
        protocols[protocol++] = 'M';
    }
    else {
        __DDL->MaerklinPacketPool.NrOfKnownAddresses = 0;
    }
    if (__DDL->ENABLED_PROTOCOLS & EP_NMRADCC) {
        init_NMRAPacketPool(busnumber);
        protocols[protocol++] = 'N';
    }
    if (__DDL->ENABLED_PROTOCOLS & EP_MFX) {
        init_MFXPacketPool(busnumber);
        protocols[protocol++] = 'X';
    }
    syslog_bus(busnumber, DBG_INFO, "DDL init done");
    buses[busnumber].protocols = protocols;
    return 0;
}

/*thread cleanup routine for this bus*/
static void end_bus_thread(bus_thread_t * btd)
{
    int result;
    /* store thread return value here */
    void *pThreadReturn;
    int busnumber = btd->bus;

    /* send cancel to refresh cycle */
    result = pthread_cancel(((DDL_DATA *) buses[btd->bus].driverdata)->
                            refresh_ptid);
    if (result != 0) {
        syslog_bus(btd->bus, DBG_ERROR,
                   "pthread_cancel() failed: %s (errno = %d).",
                   strerror(result), result);
    }
    /* wait until the refresh cycle has terminated */
    result =
        pthread_join(((DDL_DATA *) buses[btd->bus].driverdata)->
                     refresh_ptid, &pThreadReturn);
    if (result != 0) {
        syslog_bus(btd->bus, DBG_ERROR,
                   "pthread_join() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    set_SerialLine(btd->bus, SL_RTS, OFF);

    /* pthread_cond_destroy(&(__DDL->refresh_cond)); */
    if (buses[btd->bus].device.file.fd != -1)
        close(buses[btd->bus].device.file.fd);

    result = pthread_mutex_destroy(&buses[btd->bus].transmit_mutex);
    if (result != 0) {
        syslog_bus(btd->bus, DBG_ERROR,
                   "pthread_mutex_destroy() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    result = pthread_cond_destroy(&buses[btd->bus].transmit_cond);
    if (result != 0) {
        syslog_bus(btd->bus, DBG_ERROR,
                   "pthread_cond_destroy() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    syslog_bus(btd->bus, DBG_INFO, "DDL bus terminated.");

    if (__DDL->ENABLED_PROTOCOLS & EP_NMRADCC) {
        reset_NMRAPacketPool(btd->bus);
    }
    if (__DDL->ENABLED_PROTOCOLS & EP_MFX) {
        reset_MFXPacketPool(btd->bus);
    }
    free(buses[btd->bus].driverdata);
    free(btd);
}


typedef struct _delayedGAResetCmdData {
    int busnumber;
    ga_data_t *gastate;
} delayedGAResetCmdData;


/* sends a GA reset command after a delay */
void *thr_delayedGAResetCmd(void *v)
{

    delayedGAResetCmdData *delgatmp = (delayedGAResetCmdData *) v;
    ga_data_t *gatmp = delgatmp->gastate;
    int busnumber = delgatmp->busnumber;
    free(v);

    if (usleep((unsigned long) gatmp->activetime * 1000) == -1) {
        syslog_bus(busnumber, DBG_ERROR,
                   "usleep() failed: %s (errno = %d)\n",
                   strerror(errno), errno);
    }
    gatmp->action = 0;
    syslog_bus(busnumber, DBG_DEBUG,
               "Delayed GA command (threaded): %c (%x) %d",
               gatmp->protocol, gatmp->protocol, gatmp->id);
    switch (gatmp->protocol) {
        case 'M':              /* Motorola Codes */
            comp_maerklin_ms(busnumber, gatmp->id, gatmp->port,
                             gatmp->action);
            break;
        case 'N':              /* NMRA DCC */
            comp_nmra_accessory(busnumber, gatmp->id, gatmp->port,
                                gatmp->action, __DDL->NMRA_GA_OFFSET);
            break;
    }
    setGA(busnumber, gatmp->id, *gatmp);
    return NULL;
}


static void *thr_sendrec_DDL(void *v)
{
    struct _SM smakt;
    gl_data_t gltmp;
    ga_data_t gatmp;
    int addr, error;
    int last_cancel_state, last_cancel_type;

    delayedGAResetCmdData *tmpv;
    pthread_t ptid_delacccmd;

    bus_thread_t *btd = (bus_thread_t *) malloc(sizeof(bus_thread_t));
    if (btd == NULL)
        pthread_exit((void *) 1);
    btd->bus = (bus_t) v;
    btd->fd = -1;

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &last_cancel_state);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &last_cancel_type);

    /*register cleanup routine */
    pthread_cleanup_push((void *) end_bus_thread, (void *) btd);

    syslog_bus(btd->bus, DBG_INFO, "DDL bus started (device = %s).",
               buses[btd->bus].device.file.path);

    buses[btd->bus].watchdog = 1;
    /*
     * Starting the thread that is responsible for the signals on 
     * serial port.
     */
    ((DDL_DATA *) buses[btd->bus].driverdata)->refresh_param.busnumber =
        btd->bus;
    error =
        pthread_create(&
                       (((DDL_DATA *) buses[btd->bus].driverdata)->
                        refresh_ptid), NULL, thr_refresh_cycle,
                       (void *)
                       &(((DDL_DATA *) buses[btd->bus].driverdata)->
                         refresh_param));

    if (error != 0) {
        syslog_bus(btd->bus, DBG_ERROR,
                   "pthread_create() failed: %s (errno = %d).",
                   strerror(error), error);
    }

    while (1) {
        pthread_testcancel();
        if (!queue_GL_isempty(btd->bus)) {
          char p;
          int pv;
          int speed;
          int direction;

          dequeueNextGL(btd->bus, &gltmp);
          if (gltmp.id > -1) {
            //Gültiger, nicht gelöschter Eintrag verarbeiten
            p = gltmp.protocol;
            /* need to compute from the n_fs and n_func parameters */
            pv = gltmp.protocolversion;
            addr = gltmp.id;
            speed = gltmp.speed;
            direction = gltmp.direction;
            syslog_bus(btd->bus, DBG_DEBUG, "Next command: %c (%x) %d %d",
                       p, p, pv, addr);
            if (addr > 127) {
                pv = 2;
            }
            switch (p) {
                case 'M':      /* Motorola Codes */
                    if (speed) {
                        speed++;        /* Never send FS1 */
                    }
                    if (direction == 2)
                        speed = 0;
                    
					if (pv == 1) {	comp_maerklin_1(btd->bus, addr,
                                            gltmp.direction, speed,
                                            gltmp.funcs & 0x01,
                                            gltmp.prio);
                            }
                    else switch (gltmp.n_fs) {
                        case 14:	comp_maerklin_2(btd->bus, addr,
                                            gltmp.direction, speed,
                                            gltmp.funcs & 0x01,
                                            ((gltmp.funcs >> 1) & 0x01),
                                            ((gltmp.funcs >> 2) & 0x01),
                                            ((gltmp.funcs >> 3) & 0x01),
                                            ((gltmp.funcs >> 4) & 0x01),
                                            gltmp.prio);
                            		break;
                         case 27:	comp_maerklin_27(btd->bus, addr,
                                            gltmp.direction, speed,
                                            gltmp.funcs & 0x01,
                                            ((gltmp.funcs >> 1) & 0x01),
                                            ((gltmp.funcs >> 2) & 0x01),
                                            ((gltmp.funcs >> 3) & 0x01),
                                            ((gltmp.funcs >> 4) & 0x01),
                                            gltmp.prio);
                            		break;
                        case 28:	comp_maerklin_28(btd->bus, addr,
                                            gltmp.direction, speed,
                                            gltmp.funcs & 0x01,
                                            ((gltmp.funcs >> 1) & 0x01),
                                            ((gltmp.funcs >> 2) & 0x01),
                                            ((gltmp.funcs >> 3) & 0x01),
                                            ((gltmp.funcs >> 4) & 0x01),
                                            gltmp.prio);
                            		break;
                    }
                    break;
                case 'N':      /* NMRA / DCC Codes */
                    if (speed)
                        speed++;
                    if (direction != 2)
                        comp_nmra_multi_func(btd->bus, addr, direction,
                                             speed, gltmp.funcs,
                                             gltmp.n_fs, gltmp.n_func, pv,
                                             gltmp.prio);
                    else
                        /* emergency halt: FS 1 */
                        comp_nmra_multi_func(btd->bus, addr, 0,
                                             1, gltmp.funcs, gltmp.n_fs,
                                             gltmp.n_func, pv,
                                             gltmp.prio);
                    break;
                case 'X':      /* MFX */
                    if (direction != 2) {
                        if (speed == 1) {
                          speed++; //1 wäre Nothalt
                        }
                        if (direction == -1) {
                          //Lok TERM hier ierst mal ignorieren, zuerst Lok stoppen
                          direction = 0;
                        }
                        comp_mfx_loco(addr, direction,
                                      speed, gltmp.funcs,
                                      gltmp.n_fs, gltmp.n_func, pv,
                                      gltmp.prio);
                    }
                    else {
                        /* emergency halt: FS 1 */
                        comp_mfx_loco(addr, 0,
                                      1, gltmp.funcs, gltmp.n_fs,
                                      gltmp.n_func, pv,
                                      gltmp.prio);
                    }
                    if (gltmp.direction == -1) { //Lok TERM
                      sendDekoderTerm(addr);
                    }
                    break;
            }
            cacheSetGL(btd->bus, addr, gltmp);
          }
        }
        if (!queue_SM_isempty(btd->bus)) {
            dequeueNextSM(btd->bus, &smakt);
            int rc = -1;
            if (smakt.protocol == PROTO_NMRA) {
                switch (smakt.command) {
                    case SET:
                        /* addr 0 and -1 are considered as programming track */
                        /* larger addresses will by considered as PoM */
                        if (smakt.addr <= 0 && (((DDL_DATA *)
                                                 buses[btd->bus].
                                                 driverdata)->
                                                PROGRAM_TRACK)) {
                            switch (smakt.type) {
                                case REGISTER:
                                    rc = protocol_nmra_sm_write_phregister
                                        (btd->bus, smakt.cv_caIndex,
                                         smakt.value);
                                    break;
                                case CV:
                                    rc = protocol_nmra_sm_write_cvbyte
                                        (btd->bus, smakt.cv_caIndex,
                                         smakt.value);
                                    break;
                                case CV_BIT:
                                    rc = protocol_nmra_sm_write_cvbit(btd->
                                                                      bus,
                                                                      smakt.cv_caIndex,
                                                                      smakt.bit_index,
                                                                      smakt.value);
                                    break;
                                case PAGE:
                                    rc = protocol_nmra_sm_write_page
                                        (btd->bus, smakt.cv_caIndex,
                                         smakt.value);
                                    break;
                                default:    ;
                            }
                        }
                        else {
                            int mode = 1;
                            /* HACK protocolversion is not yet set in SM */
                            if (smakt.addr > 127)
                                mode = 2;
                            switch (smakt.type) {
                                case CV:
                                    rc = protocol_nmra_sm_write_cvbyte_pom
                                        (btd->bus, smakt.addr,
                                         smakt.cv_caIndex, smakt.value,
                                         mode);
                                    break;
                                case CV_BIT:
                                    rc = protocol_nmra_sm_write_cvbit_pom
                                        (btd->bus, smakt.addr,
                                         smakt.cv_caIndex, smakt.bit_index,
                                         smakt.value, mode);
                                default:    ;
                            }
                        }
                        break;
                    case GET:
                        if (smakt.addr <= 0) {
                            switch (smakt.type) {
                                case REGISTER:
                                    rc = protocol_nmra_sm_get_phregister
                                        (btd->bus, smakt.cv_caIndex);
                                    break;
                                case CV:
                                    rc = protocol_nmra_sm_get_cvbyte(btd->
                                                                     bus,
                                                                     smakt.cv_caIndex);
                                    break;
                                case CV_BIT:
                                    rc = protocol_nmra_sm_verify_cvbit
                                        (btd->bus, smakt.cv_caIndex,
                                         smakt.bit_index, 1);
                                    break;
                                case PAGE:
                                    rc = protocol_nmra_sm_get_page
                                        (btd->bus, smakt.cv_caIndex);
                                    break;
                                default:    ;
                            }
                        }
                        break;
                    case VERIFY:
                        if (smakt.addr <= 0) {
                            int my_rc = 0;
                            switch (smakt.type) {
                                case REGISTER:
                                    my_rc =
                                        protocol_nmra_sm_verify_phregister
                                        (btd->bus, smakt.cv_caIndex,
                                         smakt.value);
                                    break;
                                case CV:
                                    my_rc =
                                        protocol_nmra_sm_verify_cvbyte
                                        (btd->bus, smakt.cv_caIndex,
                                         smakt.value);
                                    break;
                                case CV_BIT:
                                    my_rc =
                                        protocol_nmra_sm_verify_cvbit(btd->
                                                                      bus,
                                                                      smakt.cv_caIndex,
                                                                      smakt.bit_index,
                                                                      smakt.value);
                                    break;
                                case PAGE:
                                    my_rc = protocol_nmra_sm_verify_page
                                        (btd->bus, smakt.cv_caIndex,
                                         smakt.value);
                                    break;
                                default:    ;
                            }
                            if (my_rc == 1) {
                                rc = smakt.value;
                            }
                        }
                        break;
                    case TERM:
                        rc = 0;
                        break;
                    case INIT:
                        rc = 0;
                        break;
                }
            }
            if (smakt.protocol == PROTO_MFX) {
              if (buses[btd->bus].power_state) {
                switch (smakt.command) {
                  case SET:
                    switch (smakt.type) {
                      case CV_MFX:
                        rc = 1;
                        smMfxSetCV(smakt.addr, smakt.cv_caIndex, smakt.bit_index, smakt.value);
                        break;
                      case CA_MFX:
                        rc = smMfxSetCA(smakt.addr, smakt.blockNr, smakt.caNr, smakt.cv_caIndex, smakt.bit_index, smakt.value);
                        break;
                      default:  ;
                    }
                    break;
                  case GET:
                  case VERIFY:
                    switch (smakt.type) {
                      case CV_MFX:
                        rc = smMfxGetCV(smakt.addr, smakt.cv_caIndex, smakt.bit_index);
                        break;
                      case CA_MFX:
                        rc = smMfxGetCA(smakt.addr, smakt.blockNr, smakt.caNr, smakt.cv_caIndex, smakt.bit_index);
                        break;
                      default:  ;
                    }
                    break;
                  case TERM:
                    setMfxSM(false);
                    rc = 0;
                    break;
                  case INIT:
                    setMfxSM(true);
                    rc = 0;
                    break;
                }
              }
              else {
                rc = -1;
              }
            }
            session_endwait(btd->bus, rc);
        }
        buses[btd->bus].watchdog = 4;

        if (!queue_GA_isempty(btd->bus)) {
            char p;
            dequeueNextGA(btd->bus, &gatmp);
            addr = gatmp.id;
            p = gatmp.protocol;
            syslog_bus(btd->bus, DBG_DEBUG, "Next GA command: %c (%x) %d",
                       p, p, addr);
            switch (p) {
                case 'M':      /* Motorola Codes */
                    comp_maerklin_ms(btd->bus, addr, gatmp.port,
                                     gatmp.action);
                    break;
                case 'N':
                    comp_nmra_accessory(btd->bus, addr, gatmp.port,
                                        gatmp.action,
                                        __DDLt->NMRA_GA_OFFSET);
                    break;
            }
            setGA(btd->bus, addr, gatmp);
            buses[btd->bus].watchdog = 5;

            if (gatmp.activetime >= 0) {

                /* the handling of delayed GA commands in this way, can only 
                   be a short term improvement. If srcpd will have better
                   inter-thread communication, it should be replaced. */

                if (gatmp.activetime < 1000) {
                    if (usleep((unsigned long) gatmp.activetime * 1000) ==
                        -1) {
                        syslog_bus(btd->bus, DBG_ERROR,
                                   "usleep() failed: %s (errno = %d)\n",
                                   strerror(errno), errno);
                    }
                    gatmp.action = 0;
                    syslog_bus(btd->bus, DBG_DEBUG,
                               "Delayed GA command: %c (%x) %d", p, p,
                               addr);
                    switch (p) {
                        case 'M':      /* Motorola Codes */
                            comp_maerklin_ms(btd->bus, addr, gatmp.port,
                                             gatmp.action);
                            break;
                        case 'N':
                            comp_nmra_accessory(btd->bus, addr, gatmp.port,
                                                gatmp.action,
                                                __DDLt->NMRA_GA_OFFSET);
                            break;
                    }
                    setGA(btd->bus, addr, gatmp);
                }
                else {
                    tmpv =
                        (delayedGAResetCmdData *)
                        malloc(sizeof(delayedGAResetCmdData));
                    if (!tmpv) {
                        syslog_bus(btd->bus, DBG_ERROR,
                                   "malloc() failed!");
                        continue;
                    }
                    tmpv->busnumber = btd->bus;
                    tmpv->gastate = &gatmp;
                    error = pthread_create(&ptid_delacccmd, NULL,
                                           thr_delayedGAResetCmd, tmpv);
                    if (error == 0) {
                        pthread_detach(ptid_delacccmd);
                    }
                    else {
                        syslog_bus(btd->bus, DBG_ERROR,
                                   "pthread_create() failed: %s (errno = %d).",
                                   strerror(error), error);
                    }
                }
            }
        }
        if (usleep(3000) == -1) {
            syslog_bus(btd->bus, DBG_ERROR,
                       "usleep() failed: %s (errno = %d)\n",
                       strerror(errno), errno);
        }
    }

    /*run the cleanup routine */
    pthread_cleanup_pop(1);
    return NULL;
}
