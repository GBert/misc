// ddl.c - adapted for basrcpd project 2018 by Rainer Müller

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
    #define WRONGLEN	(len == 64)
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
    #define WRONGLEN	(len < 96)
#endif


#define __DDL ((DDL_DATA*)buses[busnumber].driverdata)
#define __DDLt ((DDL_DATA*)buses[btd->bus].driverdata)


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

static void *thr_Manage_DDL(void *);


int init_lineDDL(bus_t busnumber)
{
    /* opens and initializes the selected SPI port and returns a file handle */
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
        return -1;
    }
    uint8_t spiBPW = 8;
    if (ioctl (dev, SPI_IOC_WR_BITS_PER_WORD, &spiBPW) < 0) {
        syslog_bus(busnumber, DBG_FATAL, "SPI BPW Change failure: %s", strerror(errno));
        return -1;
    }
    int speed = SPI_BAUDRATE_MAERKLIN_LOCO_2;
    if (ioctl (dev, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
        syslog_bus(busnumber, DBG_FATAL, "SPI Speed Change failure: %s", strerror(errno));
        return -1;
    }
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
        exit(EXIT_FAILURE);
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
  if (WRONGLEN) {
    syslog_bus(busnumber, DBG_WARN, "SPI Transfer with %d Bytes -> wrong Timing", len);
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
    j = translateBitstream2Packetstream(busnumber, idle_packet, idle_pktstr);
    update_NMRAPacketPool(busnumber, 128, idle_pktstr, j, idle_pktstr, j);

    /* generate and override idle_data */
    memset (&spi_nmra_idle, 0, sizeof (spi_nmra_idle)) ;
    spi_nmra_idle.len = convertNMRAPacketToSPIStream(busnumber, idle_pktstr, __DDL->NMRA_idle_data);
    spi_nmra_idle.tx_buf =  (unsigned long)__DDL->NMRA_idle_data;
    spi_nmra_idle.bits_per_word = 8;
    spi_nmra_idle.speed_hz = SPI_BAUDRATE_NMRA_2;
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

  if (WRONGLEN) {
    syslog_bus(busnumber, DBG_WARN, "SPI Transfer with %d Bytes -> wrong Timing", len);
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

/* calculate difference between time value and now, returns it in µs */
time_t timeSince(struct timeval tv)
{
	struct timeval now = { 0, 0 };
	gettimeofday(&now, NULL);
	return (now.tv_sec - tv.tv_sec) * 1000000 + now.tv_usec - tv.tv_usec;
}

/* checks shortcut situation for persistency */
static int checkShortcut(bus_t busnumber)
{
    int arg;
    time_t short_now = 0;
    struct timeval tv_shortcut = { 0, 0 };

    // read the CTS input
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
                        int packet_size, int packet_type, int xmits)
{
    int i;
    /* arguments for nanosleep and Maerklin loco decoders (19KHz) */
    /* all using busy waiting */
//SID, 22.03.09: gemäss http://home.mnet-online.de/modelleisenbahn-digital/Dig-tutorial-start.html sind es 4.2ms .....
    /* arguments for nanosleep and Maerklin solenoids/function decoders (38KHz) */
//SID, 04.01.08 : Wartezeit wäre theoretisch schon 850us, dies geht aber mit den LDT Dekodern nicht....

    //Nur für MFX RDS 1-Bit Rückmeldung: Position und Länge im SPI Bytestream an der sich die Rückmeldung befinden muss.
    unsigned int mfxRdsLen, mfxRdsPos = 0;

    struct spi_ioc_transfer spi;	// tx_buf; rx_buf; len; speed_hz; delay_usecs;
									// bits_per_word; cs_change; pad;
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

          unsigned int pause_btw;
          unsigned int pause_end;
          if ((packet_type == QM1FUNCPKT) || (packet_type == QM1SOLEPKT)) {
            pause_btw = 12;     // for functions multiples of 52µs
            pause_end = 59;
            spi.speed_hz = SPI_BAUDRATE_MAERKLIN_FUNC_2;
          }
          else {
            pause_btw = 12;     // for locos multiples of 104µs
            pause_end = 42;
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
        case QM2FXPKT:
        case QM1FUNCPKT:
        case QM1SOLEPKT:
            for (i = 0; i < xmits; i++) {
                if (ioctl(buses[busnumber].device.file.fd, SPI_IOC_MESSAGE(1), &spi) < 0) {
                  syslog_bus(busnumber, DBG_FATAL, "Error SPI Transfer ioctl.");
                  return;
                }
            }
            break;
        case QNBLOCOPKT:
        case QNBACCPKT:
            for (i = 0; i < xmits; i++) {
            	if (ioctl(buses[busnumber].device.file.fd, SPI_IOC_MESSAGE(1), &spi) < 0) {
                	syslog_bus(busnumber, DBG_FATAL, "Error SPI Transfer ioctl.");
                	return;
                }
            }
#if 0
            if (ioctl(buses[busnumber].device.file.fd, SPI_IOC_MESSAGE(1), &spi_nmra_idle) < 0) {
                syslog_bus(busnumber, DBG_FATAL, "Error SPI Transfer ioctl.");
                return;
            }
            if (ioctl(buses[busnumber].device.file.fd, SPI_IOC_MESSAGE(1), &spi) < 0) {
                syslog_bus(busnumber, DBG_FATAL, "Error SPI Transfer ioctl.");
                return;
            }
#endif
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
            for (i=0; i<(packet_type == QMFX0PKT ? xmits : 1); i++) {
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
  bool refreshGesendet = false;
  bool busComplete = false;

  if (refreshInfo -> protocol_refresh & EP_MAERKLIN) {
    adr = __DDL->MaerklinPacketPool.knownAddresses[refreshInfo -> last_refreshed_maerklin_loco];
    if (fast != ((time(NULL) - __DDL->MaerklinPacketPool.packets[adr].timeLastUpdate) > FAST_REFRESH_TIMEOUT)) {
      //Bei "Fast" nur die Loks, die nicht zu alt sind,
      //bei "nicht fast" / "Normal" nur die Loks, die alt sind
      refreshGesendet = true;
      send_packet(busnumber, __DDL->MaerklinPacketPool.packets[adr].packet, 18, QM2LOCOPKT, 1);
      //bei jeden Durchgang (alle Loks) das nächste Fx Paket
      send_packet(busnumber, __DDL->MaerklinPacketPool.packets[adr].f_packets
	  							[refreshInfo -> last_refreshed_maerklin_fx], 18, QM2FXPKT, 1);
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
      tNMRAPacket* tNp = __DDL->NMRAPacketPool.packets[adr];
      if (tNp == NULL) return 0;
      if (fast != ((time(NULL) - tNp->timeLastUpdate) > FAST_REFRESH_TIMEOUT)) {
        refreshGesendet = true;
        send_packet(busnumber, tNp->packet, tNp->packet_size, QNBLOCOPKT, 1);
        send_packet(busnumber, tNp->fx_packet, tNp->fx_packet_size, QNBLOCOPKT, 1);
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
      tMFXPacket* tMp = __DDL->MFXPacketPool.packets[adr];
      if (tMp == NULL) return 0;
      if (fast != ((time(NULL) - tMp->timeLastUpdate) > FAST_REFRESH_TIMEOUT)) {
        refreshGesendet = true;
        send_packet(busnumber, tMp->packet, tMp->packet_size, QMFX0PKT, 1);
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
        adr = __DDL->MFXPacketPool.knownAddresses[refreshInfo -> last_refreshed_mfx_loco];
      } while (fast != ((time(NULL) - __DDL->MFXPacketPool.packets[adr] -> timeLastUpdate) <= FAST_REFRESH_TIMEOUT));
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

/* check if shortcut or emergengy break happened
   return "true" if power is off, return "false" if power is on */
bool power_is_off(bus_t busnumber)
{
    char msg[110], info[4];
    if (__DDL->CHECKSHORT) {
		if (buses[busnumber].power_state) {
			if (checkShortcut(busnumber) == 1) {
            	buses[busnumber].power_state = 0;
                buses[busnumber].power_changed = 2;
    			__DDL->short_detected = 0;
                info[0] = 2;	info[1] = 0xA;	info[2] = 1;
                info_mcs(busnumber, 1, __DDL->uid, info);
                strcpy(buses[busnumber].power_msg, "SHORTCUT DETECTED");
                infoPower(busnumber, msg);
                enqueueInfoMessage(msg);
             }
        }
    }
    if (__DDL->CHECKCLIENTS) {
		if ((getnbr_commandsessions() == 0) && (get_pingactive() == 0) &&
						(buses[busnumber].power_state)) {
            buses[busnumber].power_state = 0;
            buses[busnumber].power_changed = 2;
            info[0] = 2;	info[1] = 0xA;	info[2] = 1;
            info_mcs(busnumber, 1, __DDL->uid, info);
            strcpy(buses[busnumber].power_msg, "CONTROL LOST");
            infoPower(busnumber, msg);
            enqueueInfoMessage(msg);
        }
    }
    if (buses[busnumber].power_changed > 0) {
        if (buses[busnumber].power_state == 0) {
            set_SerialLine(busnumber, SL_RTS, OFF);
            syslog_bus(busnumber, DBG_INFO, "Rail signal generation stopped.");
        }
        if (buses[busnumber].power_state == 1) {
            set_SerialLine(busnumber, SL_RTS, ON);
            syslog_bus(busnumber, DBG_INFO, "Rail signal generation started.");
        }
    	if (buses[busnumber].power_changed == 1) {
        	info[0] = 1;
			info[1] = buses[busnumber].power_state;
        	info_mcs(busnumber, 1, __DDL->uid, info);
        	infoPower(busnumber, msg);
        	enqueueInfoMessage(msg);
        }
        buses[busnumber].power_changed = 0;
    }
    return (buses[busnumber].power_state == 0);
}

void use_pgtrack(bus_t busnumber, bool use)
{
    syslog_bus(busnumber, DBG_DEBUG,
		"Programming track usage, actual %d, requested %d", __DDL->pgtrkonly, use);
    // TODO: complete coding of this procedure, HW access
    if (!__DDL->pgtrkonly && use) {
        syslog_bus(busnumber, DBG_DEBUG, "*** Main Track suspended");
        usleep(100000);
    }
    else if (!use && __DDL->pgtrkonly) {
        syslog_bus(busnumber, DBG_DEBUG, "*** Main Track reactivated");
        usleep(100000);
    }
    __DDL->pgtrkonly = use;
}

static int init_gl_DDL(gl_data_t * gl, char *optData)
{
    switch (gl->protocol) {
        case 'M':			/* MMx */
            if (gl->n_func > 5) return SRCP_WRONGVALUE;
            switch (gl->protocolversion) {
               // M1: 14 drive steps, relative drive direction
               case 1: return (gl->n_fs == 14) ?
                               SRCP_OK : SRCP_WRONGVALUE;
               // M2: 14, 27 or 28 drive steps, 5 functions, absolute drive direction
               case 2: return (gl->n_fs == 14 || gl->n_fs == 27 || gl->n_fs == 28) ?
                               SRCP_OK : SRCP_WRONGVALUE;
               // no other protocol versions supported
               default: return SRCP_WRONGVALUE;
            }
            break;
        case 'N': 			/* DCC */
            if (gl->n_func > 32) return SRCP_WRONGVALUE;
            switch (gl->protocolversion) {
               // N1: short addresses, 14, 28 or 128 drive steps
               case 1:  return (gl->id < 128 && (gl->n_fs == 14 ||
                                gl->n_fs == 28 || gl->n_fs == 128)) ?
                               		SRCP_OK : SRCP_WRONGVALUE;
               // N2: long addresses, 28 or 128 drive steps
               case 2:  return (gl->n_fs == 28 || gl->n_fs == 128) ?
                               		SRCP_OK : SRCP_WRONGVALUE;
               default: return SRCP_WRONGVALUE;
            }
            break;
        case 'X': 			/* MFX */
            if (gl->n_func > 32) return SRCP_WRONGVALUE;
            if (gl->protocolversion < 0 || gl->protocolversion > 1) {
                return SRCP_WRONGVALUE;
            }
            // in case of MFX, UID could be appended optionally:
			sscanf(optData, "%10u", &gl->decuid);
/*
	TODO: brauchen wir Mitteilung an MFX damit ggf. neue Lokadresse gesetzt werden kann
            newGLInit(gl->id, gl->optData.mfx.uid);
*/
            return  SRCP_OK;
    }
    return SRCP_UNSUPPORTEDDEVICEPROTOCOL;
}

/**
 * Protokoll spezifische Parameter für INIT GL Info liefern.
 * @param gl Lok, von der die Infor geliefert werden sollen.
 * @param msg Message, an die die Infos gehängt werden sollen
 */
static void describe_gl_DDL(gl_data_t *gl, char *msg)
{
	switch (gl->protocol) {
    	case 'X':
//    		describeGLmfx(gl, msg);
  			sprintf(msg + strlen(msg), " %u", gl->decuid);
      		break;
	}
}

static int init_ga_DDL(ga_data_t * ga)
{
    switch (ga->protocol) {
        case 'M':              /* Motorola Codes */
            return (ga->id <= 324) ? SRCP_OK : SRCP_WRONGVALUE;
            break;
        case 'N':
            return (ga->id <= 2044) ? SRCP_OK : SRCP_WRONGVALUE;
            break;
    }
    return SRCP_UNSUPPORTEDDEVICEPROTOCOL;
}


int readconfig_DDL(xmlDocPtr doc, xmlNodePtr node, bus_t busnumber)
{
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

    buses[busnumber].thr_func = &thr_Manage_DDL;

    strcpy(buses[busnumber].description,
           "GA GL SM POWER LOCK DESCRIPTION");

    __DDL->number_gl = 255;
    __DDL->number_ga = 324;

    __DDL->CHECKSHORT = false;  /* default no shortcut checking */
    __DDL->SHORTCUTDELAY = 0;   /* usecs shortcut delay         */
    __DDL->CHECKCLIENTS = false;  /* default no client checking */
    __DDL->ENABLED_PROTOCOLS = (EP_MAERKLIN | EP_NMRADCC);      /* enabled p's */
    __DDL->NMRA_GA_OFFSET = 0;  /* offset for ga base address 0 or 1  */
    __DDL->PROGRAM_TRACK = 1;   /* 0: suppress SM commands to PT address */
    __DDL->MCS_DEVNAME[0] = 0;	/* if empty you do not use such a device */
   	__DDL->FWD_M_ACCESSORIES = busnumber;	/* default is own bus */
   	__DDL->FWD_N_ACCESSORIES = busnumber;	/* default is own bus */

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
        else if (xmlStrcmp(child->name, BAD_CAST "enable_checkshort_checking")
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
        else if (xmlStrcmp(child->name, BAD_CAST "enable_client_checking")
                 == 0) {
            txt = xmlNodeListGetString(doc, child->xmlChildrenNode, 1);
            if (txt != NULL) {
                __DDL->CHECKCLIENTS =
                    (xmlStrcmp(txt, BAD_CAST "yes") == 0) ? true : false;
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
        else if (xmlStrcmp(child->name, BAD_CAST "mcs_device") == 0) {
            txt = xmlNodeListGetString(doc, child->xmlChildrenNode, 1);
            if (txt != NULL) {
            	strncpy(__DDL->MCS_DEVNAME, (char *)txt, 15);
                xmlFree(txt);
            }
        }
        else if (xmlStrcmp(child->name, BAD_CAST "forward_mm_ga") == 0) {
            txt = xmlNodeListGetString(doc, child->xmlChildrenNode, 1);
            if (txt != NULL) {
                __DDL->FWD_M_ACCESSORIES = atoi((char *) txt);
                xmlFree(txt);
            }
        }
        else if (xmlStrcmp(child->name, BAD_CAST "forward_nmra_ga") == 0) {
            txt = xmlNodeListGetString(doc, child->xmlChildrenNode, 1);
            if (txt != NULL) {
                __DDL->FWD_N_ACCESSORIES = atoi((char *) txt);
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
    static char protocols[3] = { '\0', '\0', '\0' };
    int protocol = 0;

    syslog_bus(busnumber, DBG_INFO, "DDL start initialization (verbosity = %d).",
               buses[busnumber].debuglevel);

    if (buses[busnumber].device.file.fd <= 0) {
      buses[busnumber].device.file.fd = init_lineDDL(busnumber);
    }
    else {
    	syslog_bus(busnumber, DBG_INFO, "DDL was already active, pools must be reset.");
	    reset_NMRAPacketPool(busnumber);
	    reset_MFXPacketPool(busnumber);
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
	if (__DDL->MCS_DEVNAME[0]) init_mcs_gateway(busnumber);

    syslog_bus(busnumber, DBG_INFO, "DDL init done");
    buses[busnumber].protocols = protocols;
    return 0;
}

/*thread cleanup routine for this bus*/
static void end_bus_thread(bus_thread_t * btd)
{
    int result;
    set_SerialLine(btd->bus, SL_RTS, OFF);

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

	if (__DDLt->MCS_DEVNAME[0]) term_mcs_gateway();
    syslog_bus(btd->bus, DBG_INFO, "DDL bus terminated.");

    if (__DDLt->ENABLED_PROTOCOLS & EP_NMRADCC) {
        reset_NMRAPacketPool(btd->bus);
    }
    if (__DDLt->ENABLED_PROTOCOLS & EP_MFX) {
        reset_MFXPacketPool(btd->bus);
    }
    free(buses[btd->bus].driverdata);
    free(btd);
}

static void *thr_Manage_DDL(void *v)
{
    struct _SM smakt;
    gl_data_t *glp;
    ga_data_t gatmp;
    int gastep = 0;
    int last_cancel_state, last_cancel_type, progwin = 0;
    char * scmd, * sprot, * stype;
    time_t nextmfxman = 0;
	struct timeval tv_ga, tv_mfx = { 0, 0 };

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

    while (true) {
        pthread_testcancel();

        /* Service Mode Handling */
        if (!queue_SM_isempty(btd->bus)) {
            dequeueNextSM(btd->bus, &smakt);
			switch(smakt.protocol) {
    			case PROTO_NMRA:	sprot = "NMRA";	break;
    			case PROTO_MM:		sprot = "MM";   break;
    			case PROTO_MFX:		sprot = "MFX";	break;
    			default:			sprot = "??";
			}
			switch(smakt.command) {
    			case SET:		scmd = "SET";		break;
    			case GET:		scmd = "GET";  		break;
    			case VERIFY:	scmd = "VERIFY";	break;
    			case INIT:		scmd = "INIT";   	break;
    			case TERM:		scmd = "TERM";    	break;
    			default:		scmd = "??";
			}
			switch(smakt.type) {
    			case REGISTER:	stype = "REGISTER"; break;
    			case PAGE:		stype = "PAGE";    	break;
    			case CV:		stype = "CV";      	break;
    			case CV_BIT:	stype = "CVBIT";  	break;
                case MM_REG:    stype = "MMREG";    break;
    			case BIND_MFX:	stype = "BIND";    	break;
    			case CV_MFX:	stype = "CVMFX";  	break;
    			default:		stype = "??";
			}
            syslog_bus(btd->bus, DBG_DEBUG,
				"Next SM: %s %s %d %d %s %d Index %d Val 0x%02x", scmd, sprot,
				smakt.protocolversion, smakt.addr, stype, smakt.cvaddr,
				smakt.index, smakt.value);
            int rc = -1;
            if (smakt.protocol == PROTO_NMRA) {
                switch (smakt.command) {
                    case SET:
                        /* addr 0 and less are considered as programming track */
                        /* larger addresses will by considered as PoM */
                        if (smakt.addr <= 0 && (((DDL_DATA *)
                                                 buses[btd->bus].
                                                 driverdata)->
                                                PROGRAM_TRACK)) {
                            use_pgtrack(btd->bus, true);
                            switch (smakt.type) {
                                case REGISTER:
                                    rc = protocol_nmra_sm_write_phregister
                                        (btd->bus, smakt.cvaddr,
                                         smakt.value);
                                    break;
                                case CV:
                                    rc = protocol_nmra_sm_write_cvbyte
                                        (btd->bus, smakt.cvaddr,
                                         smakt.value);
                                    break;
                                case CV_BIT:
                                    rc = protocol_nmra_sm_write_cvbit(btd->bus,
                                        	smakt.cvaddr, smakt.index, smakt.value);
                                    break;
                                case PAGE:
                                    rc = protocol_nmra_sm_write_page
                                        (btd->bus, smakt.cvaddr, smakt.value);
                                    break;
                                default:    ;
                            }
                        }
                        else {
                            use_pgtrack(btd->bus, false);
                            int mode = 1;
                            /* HACK protocolversion is not yet set in SM */
                            if (smakt.addr > 127)
                                mode = 2;
                            switch (smakt.type) {
                                case CV:
                                    rc = protocol_nmra_sm_write_cvbyte_pom
                                        (btd->bus, smakt.addr,
                                         smakt.cvaddr, smakt.value, mode);
                                    break;
                                case CV_BIT:
                                    rc = protocol_nmra_sm_write_cvbit_pom
                                        (btd->bus, smakt.addr,
                                         smakt.cvaddr, smakt.index,
                                         smakt.value, mode);
                                default:    ;
                            }
                        }
                        break;
                    case GET:
                        if (smakt.addr <= 0) {
                            use_pgtrack(btd->bus, true);
                            switch (smakt.type) {
                                case REGISTER:
                                    rc = protocol_nmra_sm_get_phregister
                                        (btd->bus, smakt.cvaddr);
                                    break;
                                case CV:
                                    rc = protocol_nmra_sm_get_cvbyte(btd->bus,
                                                                smakt.cvaddr);
                                    break;
                                case CV_BIT:
                                    rc = protocol_nmra_sm_verify_cvbit
                                        (btd->bus, smakt.cvaddr,
                                         smakt.index, 1);
                                    break;
                                case PAGE:
                                    rc = protocol_nmra_sm_get_page
                                        (btd->bus, smakt.cvaddr);
                                    break;
                                default:    ;
                            }
                        }
                        break;
                    case VERIFY:
                        if (smakt.addr <= 0) {
                            use_pgtrack(btd->bus, true);
                            int my_rc = 0;
                            switch (smakt.type) {
                                case REGISTER:
                                    my_rc = protocol_nmra_sm_verify_phregister
                                        (btd->bus, smakt.cvaddr, smakt.value);
                                    break;
                                case CV:
                                    my_rc = protocol_nmra_sm_verify_cvbyte
                                        (btd->bus, smakt.cvaddr, smakt.value);
                                    break;
                                case CV_BIT:
                                    my_rc = protocol_nmra_sm_verify_cvbit(btd->bus,
                                        smakt.cvaddr, smakt.index, smakt.value);
                                    break;
                                case PAGE:
                                    my_rc = protocol_nmra_sm_verify_page
                                        (btd->bus, smakt.cvaddr, smakt.value);
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
            if (smakt.protocol == PROTO_MM) {
                switch (smakt.command) {
                    case SET:
                        use_pgtrack(btd->bus, true);
                        syslog_bus(btd->bus, DBG_DEBUG, "*** MM-PROG not yet implemented");
                        rc = -1;        // error indication
                        break;
                    case GET:   case VERIFY:
                        use_pgtrack(btd->bus, true);
                        syslog_bus(btd->bus, DBG_DEBUG, "*** MM-SEARCH not yet implemented");
                        rc = 79;        // faked search result
                        break;
                    case INIT:  case TERM:
                        rc = 0;
                        break;
                }
            }
            if (smakt.protocol == PROTO_MFX) {
                use_pgtrack(btd->bus, false);
//              if (buses[btd->bus].power_state) {
                switch (smakt.command) {
                  case SET:
                    switch (smakt.type) {
                      case CV_MFX:
                        rc = smMfxSetCV(smakt.addr, smakt.cvaddr, smakt.index, smakt.value);
                        break;
                      case BIND_MFX:
                      	rc = smMfxSetBind(smakt.addr, smakt.value);
                        break;
                      default:  ;
                    }
                    break;
                  case GET:
                  case VERIFY:
                    switch (smakt.type) {
                      case CV_MFX:
                        rc = smMfxGetCV(smakt.addr, smakt.cvaddr, smakt.index, smakt.value);
                        break;
                      case BIND_MFX:
                      	rc = smMfxVerBind(smakt.addr, smakt.value);
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
            session_endwait(btd->bus, rc);
            progwin = 200;
        }
        buses[btd->bus].watchdog = 4;

		/* Power State Handling */
        if (power_is_off(btd->bus)) {
            usleep(10000);		// wait 10ms before re-test
            continue;
        }
        if (__DDLt->pgtrkonly) {
            usleep(10000);
            if (progwin) {
                progwin--;
                continue;    // combine tasks on programming track
            }
            use_pgtrack(btd->bus, false);
        }

		/* Generic Loco Handling */
        glp = dequeueNextGL(btd->bus);
		if (glp) {
            char p = glp->protocol;
            switch (p) {
                case 'M':      /* Motorola Codes */
                	comp_maerklin_loco(btd->bus, glp);
                    break;
                case 'N':      /* NMRA / DCC Codes */
					comp_nmra_multi_func(btd->bus, glp);
                    break;
                case 'X':      /* MFX */
                	comp_mfx_loco(btd->bus, glp);
                    if (glp->direction == -1) { 	// mfx loco TERM
                      sendDekoderTerm(glp->id);
                    }
                    break;
            }
            cacheSetGL(btd->bus, glp, glp);
        }
        else if (! refresh_loco(btd->bus)) {
              // TODO: Keine Lok für Refreshkommando vorhanden -> Idle Data senden damit kein DC am Gleis anliegt.
            __DDLt->spiLastMM = 0;
        }

		/* mfx Management */
	    if ((__DDLt->ENABLED_PROTOCOLS & EP_MFX) &&
        		((tv_mfx.tv_sec == 0) || (timeSince(tv_mfx) >= nextmfxman))) {
        	nextmfxman = mfxManagement(btd->bus);
			gettimeofday(&tv_mfx, NULL);
		}

		/* Generic Accessory Handling */
		switch (gastep) {
			/* activate GA element */
			case 0:	if (queue_GA_isempty(btd->bus)) break;
			        dequeueNextGA(btd->bus, &gatmp);
            		syslog_bus(btd->bus, DBG_DEBUG,
							"Next GA command: %c %d Port %d activated for %d ms",
                       		gatmp.protocol, gatmp.id, gatmp.port, gatmp.activetime);
			        gettimeofday(&tv_ga, NULL);
                    /* falls through */
            case 1:	switch (gatmp.protocol) {
                		case 'M':      	/* Motorola Code */
                    		comp_maerklin_ms(btd->bus, gatmp.id, gatmp.port,
                                     	gatmp.action);
                    		break;
                		case 'N':  		/* DCC Code */
                   			comp_nmra_accessory(btd->bus, gatmp.id, gatmp.port,
                                        gatmp.action, __DDLt->NMRA_GA_OFFSET);
                    		break;
            		}
            		if (++gastep == 2) {
            			setGA(btd->bus, gatmp.id, gatmp);
                        if (gatmp.activetime <= 0)  gastep = 0;
                    }
					break;
			/* deactivate GA element */
			case 2:	if (timeSince(tv_ga) < (gatmp.activetime * 1000)) break;
					gatmp.action = 0;
                    syslog_bus(btd->bus, DBG_DEBUG, "Delayed GA command: %c %d",
                       				gatmp.protocol, gatmp.id);
					/* falls through */
            case 3:	switch (gatmp.protocol) {
                		case 'M':      	/* Motorola Code */
                    		comp_maerklin_ms(btd->bus, gatmp.id, gatmp.port,
                                     	gatmp.action);
                    		break;
                		case 'N':  		/* DCC Code */
                   			comp_nmra_accessory(btd->bus, gatmp.id, gatmp.port,
                                        gatmp.action, __DDLt->NMRA_GA_OFFSET);
                    		break;
            		}
            		if (++gastep == 4) {
            			setGA(btd->bus, gatmp.id, gatmp);
                        gastep = 0;
                    }
		}
    }

    /*run the cleanup routine */
    pthread_cleanup_pop(1);
    return NULL;
}
