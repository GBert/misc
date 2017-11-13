/* +----------------------------------------------------------------------+ */
/* | DDL - Digital Direct for Linux                                       | */
/* +----------------------------------------------------------------------+ */
/* | Copyright (c) 2016 Daniel Sigg                                       | */
/* +----------------------------------------------------------------------+ */
/* | This source file is subject of the GNU general public license 2,     | */
/* | that is bundled with this package in the file COPYING, and is        | */
/* | available at through the world-wide-web at                           | */
/* | http://www.gnu.org/licenses/gpl.txt                                  | */
/* | If you did not receive a copy of the PHP license and are unable to   | */
/* | obtain it through the world-wide-web, please send a note to          | */
/* | gpl-license@vogt-it.com so we can mail you a copy immediately.       | */
/* +----------------------------------------------------------------------+ */
/* | Author:   Daniel Sigg daniel@siggsoftware.ch                         | */
/* |                                                                      | */
/* +----------------------------------------------------------------------+ */


#ifndef __DDL_MFX_H__
#define __DDL_MFX_H__

#include <stdbool.h>  
#include "config-srcpd.h"

/* RDS Rückmeldungen */
typedef enum {
  RDS_1, //1Bit über Quittungsimpuls (nur RDS Trägersignal vorhanden)
  RDS_8, //1Byte
  RDS_16, //2Byte
  RDS_32, //4Bytes
  RDS_64 //8Bytes
} MFXRDSBits;

typedef struct _tMFXRDSFeedback {
    bool ok; //true OK (Bei 1 Bit Rückmeldungen immer OK), false Error (keine Rückmeldung oder CRC Fehler)
    MFXRDSBits bits; //Anzahl Rückmeldebits
    unsigned char feedback[8]; //Die Rückmeldung (wenn ok)
} tMFXRDSFeedback;

/**
 * Starten der MFX Threads:
 * - MFX Verwaltung (Lokanmekdungen etc.)
 * - MFX RDS Rückmeldungen
 * @param busnumber SRCP Bus
 * @param fdRDSNewRx Pipe, über die der RDS Rückmeldungsthread neue Aufträge erhält.
 *                   Es wir jeweils "MFXRDSBits" übertragen.
 * @return 0 für OK, !=0 für Fehler
 */
int startMFXThreads(bus_t busnumber, int fdRDSNewRx);

/**
 * Alle MFX Threads terminieren.
 * @return 0 für OK, !=0 für Fehler
 */
int stopMFXThreads();
 

/* signal generating functions for maerklin mfx */

/**
  Generate the packet for MFX-decoder with 9-bit
  address and 128 speed steps and up to 32 functions
  @param address GL address
  @param direction
  @param speed
  @param func function bits
  @param nspeed number of speeds for this decoder
  @param nfuncs number of functions
  @param pv Protokoll Version, wird nicht verwendet
  @param prop true wenn Kommando priorisiert ist (Halt Koomandos).
  @return 0 == OK, 1 == Error
*/
int comp_mfx_loco(int address, int direction,
                  int speed, int func, int nspeed, int nfuncs,
                  int pv, bool prio);

/**
 * Neues MFX Lok Init Kommando empfangen.
 * Wenn diese Lok bereits unter anderer Adresse vorhanden ist -> neue Adresse setzen.
 * @param adresse Lokadresse (Schienenadresse)
 * @param uid Dekoder UID der Lok
 */
 void newGLInit(int adresse, uint32_t uid);

/**
 * Schienenadresse im Dekoder löschen -> Dekoder ist nicht mehr angemeldet.
 * @param adresse Aktuell zugeordnete Schienenadresse.
 */
void sendDekoderTerm(int adresse);
 
/**
 * MFX spezifische INIT Parameter ermitteln.
 * @param gl Lok, zu der die MFX spezifischen INIT Paramater ermittelt werden sollen.
 * @param msg Message, an die die Paramater gehängt werden sollen.
 */
void describeGLmfx(gl_data_t *gl, char *msg);

/**
 * MFX RDS Rückmeldung ist eingetroffen.
 * @param mfxRDSFeedback RDS Rückmeldung
 */
void newMFXRDSFeedback(tMFXRDSFeedback mfxRDSFeedback);

//--------------------------- SM -----------------------------

/**
 * Servicemode ein-ausschalten
 * @param smOn true=SM ein, false=SM aus
 */
void setMfxSM(bool smOn);

/**
 * SM SET: 1 Byte in den MFX Dekoder an CV/Index schreiben.
 * @param address Lokadresse
 * @param cv CV Adresse
 * @param index Index innerhalb CV
 * @param value Zu schreibendes Byte
 */
void smMfxSetCV(int address, int cv, int index, int value);

/**
 * SM SET: 1 Byte in den MFX Dekoder an CV/Index schreiben.
 * @param address Lokadresse
 * @param cv CV Adresse
 * @param index Index innerhalb CV
 * @return Gelesenes Byte 0..255, < 0 für Error
 */
int smMfxGetCV(int address, int cv, int index);


/**
 * SM SET: 1 Byte in den MFX Dekoder an Block/CA/Index schreiben.
 * @param address Lokadresse
 * @param blockNr Die Blocknummer, in dem CA gesucht wird
 * @param ca CA Typ
 * @param caIndex Das wivielte vorkommen des CA im Block wird gesucht (0...)
 * @param index Index innerhalb CV
 * @param value Zu schreibendes Byte
 * @return 0 : Fehler (CA nicht gefunden), 1 OK
 */
int smMfxSetCA(int address, int blockNr, int ca, int caIndex, int index, int value);

/**
 * SM SET: 1 Byte in den MFX Dekoder an Block/CA/Index schreiben.
 * @param address Lokadresse
 * @param blockNr Die Blocknummer, in dem CA gesucht wird
 * @param ca CA Typ
 * @param caIndex Das wivielte vorkommen des CA im Block wird gesucht (0...)
 * @param index Index innerhalb CV
 * @return Gelesenes Byte 0..255, < 0 für Error
 */
int smMfxGetCA(int address, int blockNr, int ca, int caIndex, int index);


#endif
