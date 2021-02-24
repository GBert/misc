// ddl_mfx.c - adapted for basrcpd project 2018 by Rainer Müller 

/* +----------------------------------------------------------------------+ */
/* | DDL - Digital Direct for Linux                                       | */
/* +----------------------------------------------------------------------+ */
/* | Copyright (c) 2016 Daniel Sigg                                       | */
/* +----------------------------------------------------------------------+ */
/* | This source file is subject of the GNU general public license 2,     | */
/* | that is bundled with this package in the file COPYING, and is        | */
/* | available at through the world-wide-web                              | */
/* +----------------------------------------------------------------------+ */
/* | Author:   Daniel Sigg daniel@siggsoftware.ch                         | */
/* |                                                                      | */
/* +----------------------------------------------------------------------+ */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <semaphore.h>
#include "config.h"
#include "ddl_mfx.h"
#include "syslogmessage.h"
#include "ddl.h"
#include "srcp-gl.h"
#include "srcp-error.h"

#ifndef SYSCONFDIR
	#define SYSCONFDIR "/etc"
#endif

//Zentrale UID, Dekoder Existenzabfrage und Dekodersuche Intervall (us)
#define INTERVALL_UID 500000
//Anzahl Durchläufe "Zentrale UID" (INTERVALL_UID) nach der wieder eine Adresszuweisung wiederholt wird
#define LOOP_UID_SID_REPEAT 10
//Pause nach MFX Read Befehlen(us)
#define PAUSE_MFX_READ 50000

//Max. MFX RDS Leseversuche
#define MAX_RDS_REPEATS 10

//Anzahl MFX Funktionen
#define MFX_FX_COUNT 16

#if defined BANANAPI
    #include <a20hw.h>
  /* TODO for BPi*/    //Auf dem BananaPI muss das noch geklärt werden.
  #define BAPI_MFX_RDS_QAL 16
  #define BAPI_MFX_RDS_CLK 18
  #define BAPI_MFX_RDS_DAT 22
#elif defined RASPBERRYPI
    #include <bcm2835.h>
//RDS Input Ports
//- QUAL : GPIO23, PIN16
//- CLK  : GPIO24, PIN18
//- DATA : GPIO25, PIN22
#ifdef RASPBERRYPI_BOARD_V1
  #define RPI_MFX_RDS_QAL RPI_GPIO_P1_16
  #define RPI_MFX_RDS_CLK RPI_GPIO_P1_18
  #define RPI_MFX_RDS_DAT RPI_GPIO_P1_22
#elif defined RASPBERRYPI_BOARD_V2
  //Auf dem Board V2 wurden ein paar Leitungen anders gemacht, warum auch immer...
  #define RPI_MFX_RDS_QAL RPI_V2_GPIO_P1_16
  #define RPI_MFX_RDS_CLK RPI_V2_GPIO_P1_18
  #define RPI_MFX_RDS_DAT RPI_V2_GPIO_P1_22
#elif defined RASPBERRYPI_MODEL_2_3
  #define RPI_MFX_RDS_QAL RPI_BPLUS_GPIO_J8_16
  #define RPI_MFX_RDS_CLK RPI_BPLUS_GPIO_J8_18
  #define RPI_MFX_RDS_DAT RPI_BPLUS_GPIO_J8_22
#else
  #error "Unsupported new RaspberryPI Board Version"
#endif

#endif


//MFX Verwaltungsthread
//static pthread_t mfxManageThread;
static uint16_t registrationCounter;
static int searchstep = 0;

//MFX RDS Feedback thread
static pthread_t mfxRDSThread;//Global zugängliche Parameter -> Code ist nur für eine DDL Instanz zu gebrauchen...
static bus_t busnumber = 0;
static int fdRDSNewRx; //Handle auf Pipe über die der RDS Rückmelde Thread Aufträge erhält

static tMFXRDSFeedback rdsFeedback; //RDS Rückmeldungen
static sem_t semRDSFeedback; //Semaphore zur Medlung von neuen RDS Daten

//Konfig Cache einer Lok, alle Konfigregister [CVNr10Bit][Index6Bit]
#define CV_SIZE 1024
#define CVINDEX_SIZE 64

#if 0
typedef struct _tMFXKonfigCache {
  //Die Dekoder Adresse
  int adresse;
  //Die Konfigdaten
  uint8_t cache[CV_SIZE][CVINDEX_SIZE];
  //Gültigkeit des Cache
  bool valid[CV_SIZE][CVINDEX_SIZE];
} tMFXKonfigCache;
//Es wird jeweils eine Lok im Cache gehalten, mit der gerade Konfig-Variablen gelesen / geschrieben werden.
static tMFXKonfigCache mfxKonfigCache = {.adresse=0, .cache={{0}}, .valid={{0}}};
#endif

/**
 * Servicemode (SM) aktiv oder nicht.
 * Wenn aktiv werden keine weiteren, automatischen, MFX RDS Abfragen mehr gesendet.
 * Grund: Bei den MFX RDS Rückmeldungen müsste sonst unterschieden werden, wohin sie müssen.
 */
static bool sm = false;

/**
 * MFX RDS Rückmeldung ist eingetroffen.
 * @param mfxRDSFeedback RDS Rückmeldung
 */
void newMFXRDSFeedback(tMFXRDSFeedback mfxRDSFeedback) {
  rdsFeedback = mfxRDSFeedback;
  if (sem_post(&semRDSFeedback) != 0) {
    syslog_bus(busnumber, DBG_ERROR, "newMFXRDSFeedback sem_post fail");
  }
}

/**
 * Warten auf eine neue RDS Antwort.
 * @return true wenn ok, false = Fehler
 */
static bool waitRDS() {
  struct timespec timeout;
  timeout.tv_sec = 1;
  timeout.tv_nsec = 0;
  while (true) {
    if (sem_timedwait(&semRDSFeedback, &timeout) == 0) {
      return true;
    }
    if (! buses[busnumber].power_state) {
      //Power Off -> nicht mehr warten
      //printf("waitRDS sem_wait Timeout\n");
      return false;
    }
  }
}

#if 0
// HACK: das brauchen wir wohl nicht
/**
 * MFX spezifische INIT Parameter als String.
 * @param uid
 * @param name
 * @param fx Array mit MFX_FX_COUNT Elementen zur Funktionsbeschreibung
 * @param msg Message, an die die Paramater gehängt werden sollen.
 *            Minimale Länge muss für alles ausreichend sein:
 *            Länge UID und fx max 10, Name 16+2 plus Spaces
 */
void getMFXInitParam(uint32_t uid, /*char *name, uint32_t *fx,*/ char *msg) 
{
  sprintf(msg + strlen(msg), " %u", uid);
/*  sprintf(msg + strlen(msg), " %u \"%s\"", uid, name);
  int i;
  for (i = 0; i<MFX_FX_COUNT; i++) {
    sprintf(msg + strlen(msg), " %u", fx[i]);
  }	*/
}

/**
 * MFX spezifische INIT Parameter aus Lokdaten ermitteln.
 * @param gl Lok, zu der die MFX spezifischen INIT Paramater ermittelt werden sollen.
 * @param msg Message, an die die Paramater gehängt werden sollen.
 */
void describeGLmfx(gl_data_t *gl, char *msg) 
{
  if (gl->protocol == 'X') {
//    getMFXInitParam(gl->decuid, /* gl->optData.mfx.name, gl->optData.mfx.fx,*/ msg);
  	sprintf(msg + strlen(msg), " %u", gl->decuid);
  }
}
#endif
/**
 * Fügt n Bits in einen Ausgabestrem hinzu.
 * @param bits Bis zu 32 Bits die hinzugefügt werden sollen.
 *             MSB wird immer zuerst.
 * @param len Anzahl Bits die hinzugefügt werden soll (ab LSB Bit in "bits")
 * @param stream Ausgabestream, zu dem die Bits hibzugefügt werden sollen.
 *               Es wird mit Position 0 mit dem LSB in stream[1] begonnen.
 *               Muss mit 0 initialisiert sein!
 * @param pos Position, an der eingefügt werden soll. Wird um "len" inkrementiert.
 */
static void addBits(unsigned int bits, unsigned int len, char *stream, unsigned int *pos) {
  unsigned int i;
  unsigned int maske = 1 << (len - 1);
  for (i = 0; i<len; i++) {
    if (bits & maske) {
      stream[1 + (*pos / 8)] |= 1 << (*pos % 8);
    }
    else {
    }
    (*pos)++;
    maske >>= 1;
  }
}

/**
 * Fügt den MFX Adresse ein.
 * @param address Adresse die eingefügt werden soll
 * @param stream Strem in dem der CRC an "pos" eingefügt werden soll.
 *               CRC wird ab stream[1] LSB bis pos-1 berechnet.
 * @param pos Position, an der eingefügt werden soll. Wird um CRC Länge (8) inkrementiert.
 */
static void addAdrBits(int address, char *stream, unsigned int *pos) {
  if (address < 128) {
    addBits(0b10, 2, stream, pos); //Adr 7 Bit
    addBits(address, 7, stream, pos);
  }
  else if (address < 512) {
    addBits(0b110, 3, stream, pos); //Adr 9 Bit
    addBits(address, 9, stream, pos);
  }
  else if (address < 2048) {
    addBits(0b1110, 4, stream, pos); //Adr 11 Bit
    addBits(address, 11, stream, pos);
  }
  else {
    addBits(0b1111, 4, stream, pos); //Adr 14 Bit
    addBits(address, 14, stream, pos);
  }
}

/**
 * Fügt den MFX CRC ein.
 * Da der CRC am Schluss ist wird an [0] die total Anzahl Bits gesetzt
 * @param stream Strem in dem der CRC an "pos" eingefügt werden soll.
 *               CRC wird ab stream[1] LSB bis pos-1 berechnet.
 * @param pos Position, an der eingefügt werden soll. Wird um CRC Länge (8) inkrementiert.
 */
static void addCRCBits(char *stream, unsigned int *pos) {
  uint16_t crc = 0x007F; // nicht nur 8 Bit, um Carry zu erhalten
  unsigned int i;
  const int CRC_LEN = 8;
  unsigned int count = *pos + CRC_LEN;
  for (i = 0; i < count; i++) {
    crc = (crc << 1) + ((stream[1 + (i / 8)] & (1 << (i % 8))) ? 1 : 0);
    if ((crc & 0x0100) > 0) {
      crc = (crc & 0x00FF) ^ 0x07;
    }
  }
  addBits(crc, CRC_LEN, stream, pos);
  stream[0] = *pos;
}

/**
 * MFX Paket versenden.
 * @param address Lokadresse oder 0 (Broadcast)
 * @param stream zu versendendes MFX Paket. an [0] muss die Anzahl Bist stehen, die ab [1] LSB folgen.
 * @param packetTyp MFX Pakettyp (QMFX?PKT)
 * @return Anzahl Bytes aus Stream, die gesendet wurden.
 */
static unsigned int sendMFXPaket(int address, char *stream, int packetTyp, int xmits)
{
  //Da nur ein Paket mit MFX Rückmeldung unterwegs sein kann, muss die Semaphore zur RDS Rückmeldung hier immer auf 0 stehen.
  //Durch Timeout beim warten wegen z.B. Power Off und doch noch Ausführen Feedbackbefehl bei Power On
  //könnte es sein, dass die Semaphore deshalb bereits auf >0 steht.
  if (packetTyp != QMFX0PKT) {
    int value;
    while (true) {
      if (sem_getvalue(&semRDSFeedback, &value) == 0) {
        if (value > 0) {
          sem_trywait(&semRDSFeedback);
        }
        else {
          break;
        }
      }
      else {
        //Sollte nicht vorkommen ....
        break;
      }
    }
  }
  
	unsigned int sizeStream = ((stream[0] + 7) / 8) + 1; //+7 damit immer bei int Division aufgerundet wird
	send_packet(busnumber, stream, sizeStream, packetTyp, xmits);
	return sizeStream;
}

/**
  Generate the packet for MFX-decoder with 9-bit
  address and 128 speed steps and up to 16 functions
  @param pointer to GL data
*/

void comp_mfx_loco(bus_t bus, gl_data_t *glp)
{
	char packetstream[PKTSIZE];
	memset(packetstream, 0, PKTSIZE);

    int address = glp->id;
    int speed = glp->speed;
    int direction = glp->direction;
	uint32_t funcs = glp->funcs;
    uint8_t nspeed = glp->n_fs;
    uint8_t	nfuncs = glp->n_func;

    if (glp->speedchange & SCEMERG) {   // Emergency Stop
        speed = 1;
        direction = glp->cacheddirection;
        glp->speedchange &= ~SCEMERG;
    }
    else if (speed) speed++;        	// Never send FS1

    if (direction == -1) direction = 0; // mfx loco TERM	
  	if (speed > 127) speed = 127;
    glp->speedchange &= ~(SCSPEED | SCDIREC);   // handled now
    
	syslog_bus(bus, DBG_DEBUG,
             "command for MFX protocol received addr:%d "
             "dir:%d speed:%d nspeeds:%d nfunc:%d funcs %x",
             address, direction, speed, nspeed, nfuncs, funcs);

  //packetstream Format:
  //1. Byte länge
  //Ab 2. Byte Low Bit: jedes Bit bis Anzahl Bits Länge
  
  //Format des Bitstreams für Fahrstufe ist (9 Bit Adresse, 127 Fahrstufen):
  //<=4 Fn       : 110AAAAAAAAA001RSSSSSSS010FFFFCCCCCCCC
  //>4 .. <=8 Fn : 110AAAAAAAAA001RSSSSSSS0110FFFFFFFFCCCCCCCC
  //>8 Fn        : 110AAAAAAAAA001RSSSSSSS0111FFFFFFFFFFFFFFFFCCCCCCCC
  //if (speed mod 16)==0 :     000RSSS.....
  //A=Adresse, für Adressen <128 wird die 7 Bit Adressierung verwendet
  //R=Richtung
  //S=Fahrstufe
  //F=F0 bis F15
  //C=Checksumme

	unsigned int pos = 0;
  	addAdrBits(address, packetstream, &pos);
  	// prefer short speed format
	if ((speed & 15) == 0) {
		addBits(0b000, 3, packetstream, &pos);	// drive using 3 Bits
    	addBits(~direction, 1, packetstream, &pos);
    	addBits(speed >> 4, 3, packetstream, &pos);
  	}
  	else {
    	addBits(0b001, 3, packetstream, &pos); 	// drive using 7 Bits
    	addBits(~direction, 1, packetstream, &pos);
    	addBits(speed, 7, packetstream, &pos);
  	}
	if (nfuncs <=4) {
		addBits(0b010, 3, packetstream, &pos);	// upto 4 functions
		addBits(funcs, 4, packetstream, &pos);
  	}
	else if (nfuncs <=8) {
		addBits(0b0110, 4, packetstream, &pos);	// upto 8 functions
		addBits(funcs, 8, packetstream, &pos);
  	}
  	else {
		addBits(0b0111, 4, packetstream, &pos);	// upto 16 functions
		addBits(funcs, MFX_FX_COUNT, packetstream, &pos);
  	}
  	addCRCBits(packetstream, &pos);
  	unsigned int sizeStream = sendMFXPaket(address, packetstream, QMFX0PKT, 2);
  	update_MFXPacketPool(bus, address, packetstream, sizeStream);

  //Wenn mehr als 16 MFX Funktionen vorhanden sind -> geänderte Funktionen extra senden
  //Format des Bitstreams für Funktion Einzelansteuerung (9 Bit Adresse):
  //110AAAAAAAAA100NNNNNNN0FCCCCCCCC
  if (nfuncs > 16) {
      uint32_t fxChange = glp->funcschange & 0xffff0000;
      glp->funcschange &= ~fxChange;
      int i;
      for (i=16; i<32; i++) {
        if ((fxChange & (1U << i)) != 0) {
			syslog_bus(bus, DBG_DEBUG, "COMP MFX Loco. Adr=%d, nfuncs=%d, Fx%d=%d", 
		  						address, nfuncs, i, (funcs >> i) & 1);
          	memset(packetstream, 0, PKTSIZE);
          	pos = 0;
          	addAdrBits(address, packetstream, &pos);
          	addBits(0b100, 3, packetstream, &pos); 		// switch one function
          	addBits(i, 7, packetstream, &pos); 			// number of function
          	addBits(0, 1, packetstream, &pos);
          	addBits((funcs >> i) & 1, 1, packetstream, &pos); // new state
          	addCRCBits(packetstream, &pos);
          	sendMFXPaket(address, packetstream, QMFX0PKT, 2);
		}
      }
  }
}

#define REG_COUNTER_FILE SYSCONFDIR"/srcpd.regcount"
/**
 * Den gespeicherten Neuanmeldezähler laden.
 * @return Aktuell gespeicherter Neuanmeldezähler, wenn nichts gespeichert ist 0.
 */
static uint16_t loadRegistrationCounter() {
  syslog_bus(busnumber, DBG_INFO, 
  			"Load ReRegistration counter from file %s", REG_COUNTER_FILE);
  int regCountFile = open(REG_COUNTER_FILE, O_RDONLY);
  if (regCountFile < 0) {
    //Kein Reg.Counter File vorhanden -> Start bei 0
    return 0;
  }
  char buffer[10];
  //Lesen und sicherstellen dass mit 0 terminiert wird.
  buffer[read(regCountFile, buffer, sizeof(buffer) - 1)] = 0;
  close(regCountFile);
  return atoi(buffer);
}
/**
 * Den veränderten Neuanmeldezähler speichern.
 * @regCounter Aktueller Neuanmeldezähler, der gespeichert werden soll.
 */
static void saveRegistrationCounter(uint16_t regCounter) {
	int regCountFile = open(REG_COUNTER_FILE, O_WRONLY | O_CREAT | O_TRUNC,
												S_IRUSR | S_IWUSR);
	if (regCountFile < 0) {
		syslog_bus(busnumber, DBG_ERROR,
                "Error %d when trying to store ReRegistration counter", errno);
    	return;
  	}
  	char buffer[10];
  	sprintf(buffer, "%d", regCounter);
  	write(regCountFile, buffer, strlen(buffer));
  	close (regCountFile);
}

/**
 * MFX Paket mit UID der Zentrale und Neuanmeldezähler versenden.
 * @param uid
 * @param registrationCounter
 */
static void sendUIDandRegCounter(uint32_t uid, uint16_t registrationCounter) {
  //printf("sendUIDandRegCounter uid=%u regCounter=%u\n", uid, registrationCounter);
  char packetstream[PKTSIZE];
  memset(packetstream, 0, PKTSIZE);
  //Format des Bitstreams:
  //10AAAAAAA111101UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUZZZZZZZZZZZZZZZZCCCCCCCC
  //A=0 (Broadcast)
  //U=32 Bit UID
  //Z=16 Bit Neuanmeldezähler
  //C=Checksumme
  unsigned int pos = 0;
  addAdrBits(0, packetstream, &pos); //Adresse 0
  addBits(0b111101, 6, packetstream, &pos); //Kommando UID & RegCounter
  addBits(uid, 32, packetstream, &pos); //32 Bit UID
  addBits(registrationCounter, 16, packetstream, &pos); //16 Bit Neuanmeldezähler
  addCRCBits(packetstream, &pos);
    
  sendMFXPaket(0, packetstream, QMFX0PKT, 1);
}

/**
 * MFX Dekoder Existanzabfrage senden.
 * Kann mit dekoderUID=0 zum löschend er SID im Dekoder mit anschliessender Neuanmeldung verwendet werden
 * @param adresse SID des Dekoders
 * @param dekoderUID Die UID des Dekoders (0=SID im Dekoder löschen)
 * @param waitDekoderQuittung true: Warten auf Quittung vom Dekoder (wird bei UID=0 ignoriert)
 *                            false: Keine Dekoder Rückmeldung abwarten
 * @return true Wenn der Dekoder vorhanden ist und antwortet, sonst false.
 *              Beim löschen SID im Dekoder (dekoderUID=0) oder wenn waitDekoderQuittung=false wird immer true zurückgegeben.
 */
static bool sendDekoderExist(int adresse, uint32_t dekoderUID, bool waitDekoderQuittung) {
  char packetstream[PKTSIZE];
  memset(packetstream, 0, PKTSIZE);
  //Format des Bitstreams:
  //10AAAAAAA111100UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUCCCCCCCC
  //Adresse
  //U=32 Bit UID
  //C=Checksumme
  unsigned int pos = 0;
  addAdrBits(adresse, packetstream, &pos); //Adresse 0
  addBits(0b111100, 6, packetstream, &pos); //Kommando Dekoder Existanzabfrage
  addBits(dekoderUID, 32, packetstream, &pos); //32 Bit UID
  addCRCBits(packetstream, &pos);

  unsigned int packetTyp;
  if (dekoderUID == 0) {
    //Löschen Zuordung, keine 1 Bit Rückmeldung
    packetTyp = QMFX0PKT;
  }
  else {
    //Dekoder Existenzabfrage, 1 Bit Rückmeldung
    if (waitDekoderQuittung) {
      packetTyp = QMFX1PKT;
    }
    else {
      //Nur Dekoder Existenzabfrage senden, Rückmeldung egal (nur damit die MFX Dekoder zufrieden sind)
      packetTyp = QMFX1DPKT;
    }
  }
  sendMFXPaket(0, packetstream, packetTyp, 1);
  if (packetTyp != QMFX1PKT) {
    //keine Rückmeldung
    return true;
  }
  //Dekoder Rückmeldung auswerten
  //Warten auf Antwort
  if (! waitRDS()) {
    return false;
  }
  return rdsFeedback.feedback[0];
}

/**
 * MFX Paket zur Suche nach neuen, noch nicht angemeldeten Dekodern wird versandt.
 * Es wird hier auf die Antwort gewartet.
 * @param countUIDBits Anzahl Bits die im Dekoder Übereinstimmen müssen.
 * @param dekoderUID Dekoder UID die gesucht wird. 0 Beim Start der Suche.
 *                   Wenn ein neuer Dekoder gefunden wurde wird dessen UID zurückgegeben.
 * @return true Wenn ein Dekoder auf die Suche reagiert hat und neu angemeldet werden muss.
 */
static bool sendSearchNewDecoder(unsigned int countUIDBits, uint32_t *dekoderUID) {
  //printf("Suche Dekoder C=%d, UID=%u\n", countUIDBits, *dekoderUID);
  char packetstream[PKTSIZE];
  memset(packetstream, 0, PKTSIZE);
  //Format des Bitstreams:
  //10AAAAAAA111010CCCCCCUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUCCCCCCCC
  //A=0 (Broadcast)
  //C=6 Bit Anzahl Bits aus U die im Dekoder übereinstimmen müssen
  //U=32 Bit UID des Dekoder, der gesucht wird.
  //C=Checksumme
  unsigned int pos = 0;
  addAdrBits(0, packetstream, &pos); //Adresse 0
  addBits(0b111010, 6, packetstream, &pos); //Kommando Dekoder Suche
  addBits(countUIDBits, 6, packetstream, &pos); //6 Bit Count relevante UID Bits
  addBits(*dekoderUID, 32, packetstream, &pos); //32 Bit UID
  addCRCBits(packetstream, &pos);
    
  sendMFXPaket(0, packetstream, QMFX1PKT, 1);
  
  //Warten auf Antwort
  if (! waitRDS()) {
    return false;
  }
  //Falls ein Dekoder geantwortet hat wird nun das nächste Bit der UID gesucht
  if (rdsFeedback.feedback[0]) {
    //printf("MFX Dekoder suche OK C=%d, UID=%u\n", countUIDBits, *dekoderUID);
    if (countUIDBits >= 32) {
      //Alle UID Bist sind ermittelt, Dekoder ist gefunden.
      //printf("MFX Dekoder UID ermittelt:%u\n", *dekoderUID);
      //Falls UID==0 ist, war das eine Störung, kann kein Dekoder sein.
      return (*dekoderUID != 0);
    }
    countUIDBits++;
    //Rekursiv weiter suchen. Nächstes Bit 0.
    if (! sendSearchNewDecoder(countUIDBits, dekoderUID)) {
      //Keine Antwort, mit UID Bit 1 testen
      *dekoderUID |= 0x80000000 >> (countUIDBits - 1);
      if (! sendSearchNewDecoder(countUIDBits, dekoderUID)) {
        //Fehler, Dekoder hat auf einem weiteren Bit mit 0 und 1 nicht geantwortet -> Abbruch
        return false;
      }
    }
    //Nächstes Bit ist ermittelt
    return true;
  }
  //printf("Keine Antwort\n");
  //Kein Dekoder hat geantwortet
  return false;
}

/**
 * Neuer Lok Schienenadresse zuweisen.
 * @param dekoderUID UID des neuen Dekoders
 * @return Die neu zugewiesene Schienenadresse. 0 wenn nicht möglich weil keine freie Adresse gefunden wurde.
 */
void assignSID(uint32_t dekoderUID, unsigned int sid) 
{
  syslog_bus(busnumber, DBG_INFO, "New SID %d for UID %u", sid, dekoderUID);
  char packetstream[PKTSIZE];
  memset(packetstream, 0, PKTSIZE);
  //Format des Bitstreams:
  //10AAAAAAA111011AAAAAAAAAAAAAAUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUCCCCCCCC
  //1.A=0 (Broadcast)
  //2.A die neue Adresse
  //U=32 Bit UID des Dekoder.
  //C=Checksumme
  unsigned int pos = 0;
  addAdrBits(0, packetstream, &pos); //Adresse 0
  addBits(0b111011, 6, packetstream, &pos); //Kommando Dekoder Schienenadresse Zuweiseung
  addBits(sid, 14, packetstream, &pos); //14 Bit neue Adresse
  addBits(dekoderUID, 32, packetstream, &pos); //32 Bit UID
  addCRCBits(packetstream, &pos);
    
  sendMFXPaket(0, packetstream, QMFX0PKT, 2);
}

#if 0
/**
 * Prüft, ob der MFX Konfig Cache für Lokadresse "adresse" gesetzt ist.
 * Wenn nict, wird der Cache nue initialisiert.
 * @param adresse
 */
static void checkMFXConfigCache(int adresse) {
  if (mfxKonfigCache.adresse != adresse) {
    //Bis jetzt ist eine andere Lok im Cache -> alles neu
    mfxKonfigCache.adresse = adresse;
    memset(mfxKonfigCache.valid, false, sizeof(mfxKonfigCache.valid));
  }
}
#endif

/**
 * CV einer Lok abrufen.
 * Bei einem Fehler wird bis zu 10 mal wiederholt.
 * @param adresse Schienenadresse des Dekoders
 * @param cv Nummer des CV's (10 Bit)
 * @param index Index im CV (6 Bit)
 * @param bytes Anzahl Bytes die Ab diesem CV ausgelesen werden sollen (1, 2, 4, 8)
 * @param buffer Buffer in den die ausgelesen Bytes geschrieben werden.
 * @return true wenn ohne Fehler ausgelesen werden konnte.
 */
static bool readCV(int adresse, uint16_t cv, uint16_t index, unsigned int bytes, char *buffer) 
{
//  checkMFXConfigCache(adresse);
  if ((cv >= CV_SIZE) || (index >= CVINDEX_SIZE)) {
    //Ungültige CV / Index Angaben
    return false;
  }
  int i;
#if 0
  //Falls der Wert bereits im Cache ist wird dieser zurückgegeben
  bool cacheOK = true;
  for (i=0; i<bytes;i++) {
    if (! mfxKonfigCache.valid[cv][index+i]) {
      cacheOK = false;
      break;
    }
  }
  if (cacheOK) {
    //Werte auc Cache zurückgeben
    memcpy(buffer, &(mfxKonfigCache.cache[cv][index]), bytes);
    return true;
  }
#endif
  //Werte von Lokdekoder lesen
  char packetstream[PKTSIZE];
  memset(packetstream, 0, PKTSIZE);
  unsigned int packetTyp;
  unsigned int pos = 0;
  addAdrBits(adresse, packetstream, &pos); //Adresse
  addBits(0b111000, 6, packetstream, &pos); //KommandoCV lesen
  addBits(cv, 10, packetstream, &pos); //10 Bit CV
  addBits(index, 6, packetstream, &pos); //6 Bit Index
  switch (bytes) { //2 Bits für Anzahl Bytes
    case 2:
      addBits(0b01, 2, packetstream, &pos);
      packetTyp = QMFX16PKT;
      break;
    case 4:
      addBits(0b10, 2, packetstream, &pos);
      packetTyp = QMFX32PKT;
      break;
    case 8:
      addBits(0b11, 2, packetstream, &pos);
      packetTyp = QMFX64PKT;
      break;
    default:
      //Im Zweifelsfall 1 Byte
      bytes = 1;
      addBits(0b00, 2, packetstream, &pos);
      packetTyp = QMFX8PKT;
  }
  addCRCBits(packetstream, &pos);

  for (i=0; i<MAX_RDS_REPEATS; i++) {
    sendMFXPaket(0, packetstream, packetTyp, 1);
  
    //Auf Rückmeldung warten
    if (! waitRDS()) {
      return false;
    }
    if (rdsFeedback.ok) {
      break;
    }
    /*
    syslog_bus(busnumber, DBG_INFO,
               "RDS Feedback ungültig");
    printf("RDS Feedback ungültig\n");*/
  }
  if (rdsFeedback.ok) {
    memcpy(buffer, rdsFeedback.feedback, bytes);
    //printf("readCV return buffer[0]=0x%x\n", buffer[0]);
    //Cache aktualisieren
//    memcpy(&(mfxKonfigCache.cache[cv][index]), rdsFeedback.feedback, bytes);
//    memset(&(mfxKonfigCache.valid[cv][index]), true, bytes);
  }
  else {
    syslog_bus(busnumber, DBG_WARN, "RDS Feedback Abbruch. Zuviele ungültige Versuche.");
  }
  //Kurzs Pause damit nicht alles mit MFX Lesebefehlen geflutet wird.
  usleep(PAUSE_MFX_READ);
  return rdsFeedback.ok;
}

#if 0
// HACK: da die einzige Stelle an der das benötigt wurde ausge-IF-t wurde
/**
 * Nächste MFX Lok ermitteln.
 * @param adr Aktuelle Lokadresse.
 *            Es wird die nächste gefunde MFX Lok zurückgegeben.
 *            Bei erreichen der max. Lokadresse wird wieder von vorne begonnen.
 * @return Wenn eine MFX Lok gefunden wurde: die UID dieser, sonst 0.
 */
static uint32_t getNextMFXLok(int *adr) {
  int maxAddr = getMaxAddrGL(busnumber);
  unsigned int i;
//  int glAddr = 0;
  //Maximal alle Loks durchgehen
  for (i=1; i<=maxAddr; i++) {
    (*adr)++;
    if (*adr > maxAddr) {
      *adr = 1;
    }
    gl_data_t gl;
    if (cacheGetGL(busnumber, *adr, &gl) == SRCP_OK) {
      if (gl.protocol == 'X') {
        //Nächste (oder wieder dieselbe) MFX Lok gefunden
        return gl.decuid;
      }
    }
  }
  return 0;
}
#endif

/**
 * Vorhandene Loks nach UID durchsuchen.
 * @param adrBekannt Bekannte Lokadresse mit "uid". Diese wird bei der Suche ignoriert.
 *        Wenn 0 übergeben wird: keine bekannte Lokadresse vorhanden
 * @param uid Lokdekoder UID die gesucht werden soll.
 * @retun Lokadresse der Lok mit "uid", resp. 0 wenn nicht gefunden
 */
static int searchLokUID(int adrBekannt, uint32_t uid) {
  int maxAddr = getMaxAddrGL(busnumber);
  unsigned int i;
  int glAddr = 0;
  //Zuerst suchen, ob die UID schon bekannt ist
  for (i=1; i<=maxAddr; i++) {
    if (adrBekannt != i) { //Bekannte Adresse übergehen
      gl_data_t gl;
      if (cacheGetGL(busnumber, i, &gl) == SRCP_OK) {
        if ((gl.protocol == 'X') && (gl.decuid == uid)) {
          //Lok gibt es schon
          glAddr = i;
          break;
        }
      }
    }
  }
  return glAddr;
}

/**
 * Neues MFX Lok Init Kommando empfangen.
 * Wenn diese Lok bereits unter anderer Adresse vorhanden ist -> neue Adresse übernehmen, alte Lok löschen, neue Adresszuordnung senden.
 * @param adresse Lokadresse (Schienenadresse)
 * @param uid Dekoder UID der Lok
 */
void newGLInit(int adresse, uint32_t uid) {
  int glAddr = searchLokUID(adresse, uid);
  if (glAddr > 0) {
    //printf("Neues INIT Adr %d zu bestehender Adr %d für UID=%u\n", adresse, glAddr, uid);
    //Lok Schienenadresse setzen
    assignSID(uid, adresse);
    //Alten Lokeintrag löschen
    if (cacheTermGL(busnumber, glAddr) != SRCP_OK) {
      syslog_bus(busnumber, DBG_ERROR,
                 "Neues INIT Adr %d zu bestehender Adr %d für UID=%u, alte Lok kann nicht gelöscht werden!", adresse, glAddr, uid);
    }
  }
}

#if 0
/**
 * GL Adresse ermitteln, diese der Lok als Schienenadresse zuweisen.
 * Falls die UID der Lok schon als bekannte MFX Lok vorhanden ist,
 * wird deren Adresse geliefert. Wenn nicht, die erste freie gefundene.
 * @param dekoderUID UID des neuen Dekoders
 * @param bereitsVorhanden Wird auf true gesetzt, wenn Lok UID bereits vorhanden war, false wenn es eine neue UID ist.
 * @return Die neu zugewiesene Schienenadresse. 0 wenn nicht möglich weil keine freie Adresse gefunden wurde.
 */
static unsigned int getSID(uint32_t dekoderUID, bool *bereitsVorhanden) {
  int maxAddr = getMaxAddrGL(busnumber);
  unsigned int i;
  int glAddr = searchLokUID(0, dekoderUID);
  if (glAddr == 0) {
    //Noch nichts gefunden -> neue Lok, erste freie nehmen
    *bereitsVorhanden = false;
    for (i=1; i<=maxAddr; i++) {
      if (! isInitializedGL(busnumber, i)) {
        glAddr = i;
        break;
      }
    }
  }
  else {
    *bereitsVorhanden = true;
  }

  if (glAddr == 0) {
    syslog_bus(busnumber, DBG_WARN,
               "Keine freie GL Adresse vorhanden, neue MFX Lok kann nicht angemeldet werden");
    return 0;
  }
  //Adresse zuweisen
  assignSID(dekoderUID, glAddr);
  return glAddr;
}
#endif

/**
 * MFX Verwaltungsthread:
 * - Periodisches Versenden UID Zentrale und Neuanmeldezähler
 * - Periodisches suchen von noch nicht angemeldeten Dekodern, wenn welche gefunden werden,
 *   dann werden diese angemeldet.
 * @param 
 */
time_t mfxManagement(bus_t busnum)
{
	DDL_DATA *ddl = (DDL_DATA*)buses[busnum].driverdata;
	if (searchstep == 0) sendUIDandRegCounter(ddl -> uid, registrationCounter);

	// TODO: search for new decoders and increment searchstep accordingly
    uint32_t dekoderUID = 0;
    if (searchstep) sendSearchNewDecoder(0, &dekoderUID);
    
    return ((searchstep == 0) ? 1000000 : 100000);	// next call in about 1s or 0.1
}

#if 0
static void *thr_mfxManageThread(void *threadParam) {
  DDL_DATA *ddl = (DDL_DATA*)buses[busnumber].driverdata;
  //UID dieser Zentrale
  uint32_t uid = ddl -> uid;
  //Neuanmeldezähler mit letztem gespeichertem Wert laden
  /*uint16_t*/ registrationCounter = loadRegistrationCounter();
  syslog_bus(busnumber, DBG_INFO, "ReRegistration counter = %d", registrationCounter);
  //Aktuelle Lok SID für periodische Dekoder Existenzabfrage
//  int lokAdrExist = 0;
  //Erste MFX UID, zu der ein "DekoderExist" gesendet wurde. Zur Erkennung, wenn ein Durchgang über alle MFX Loks fertig ist
//  uint32_t firstUID = 0;
  //Zähler Anzahl "assignSID" Durchläufe nach Power Up-Down
//  int countUIDLoops = 0;
  for (;;) {
    if (buses[busnumber].power_state) {
      //Bus ist ein -> UID / Neuanmeldezähler versenden, neue Dekoder suchen
      sendUIDandRegCounter(uid, registrationCounter);
      usleep(INTERVALL_UID);
#if 0
// HACK: disable automatic PING and re-BIND by server
      //Periodische Existenzabfrage an alle angemeldeten Dekoder.
      //Rückmeldung wird nie ausgewertet, ich überwache sowieso nicht die ganze Anlage mit MFX fähigen Boostern
      uint32_t uidExist = getNextMFXLok(&lokAdrExist);
      if (uidExist != 0) {
        sendDekoderExist(lokAdrExist, uidExist, false);
        //Hier kommt noch ein kleiner Kunstgriff:
        //Periodische Widerholung MFX Schienenadresszuordnungen.
        //Damit werden Loks, die ihre Zuordnung gelöscht haben, gleich wieder angemeldet, auch dann, wenn sie auf einem
        //nicht MFX rückmeldefähigen Abschnitt (Booster) stehen.
        if (firstUID == uidExist) {
          //Ein Durchgang über alle MFX Lok abgeschlossen
          countUIDLoops++;
        }
        if ((countUIDLoops % LOOP_UID_SID_REPEAT) == 0) {
          assignSID(uidExist, lokAdrExist);
        }
        if (firstUID == 0) {
          //1. UID Merken
          firstUID = uidExist;
        }
      }
#endif
      //Wenn SM aktiv ist machen wir hier nichts mehr. Loksuche ist unterbrochen bis SM wieder ausgeschaltet wird.
      if (0) {		// HACK: disable temporarely (! sm) {
        //Suche nach noch nicht angemeldeten MFX Dekodern
        uint32_t dekoderUID = 0;
        //Wenn eine Dekoder UID 0 gefunden wurde, wird dies ignoriert. Die sollte es nicht geben,
        //kann aber ermittelt werden, wenn durch eine Störung die RDS 1 Bit Rückmeldung dauerend erkannt wird.
        if ((sendSearchNewDecoder(0, &dekoderUID)) && (dekoderUID != 0)) {
          //Neuem MFX Dekoder die nächste freie Schienenadresse zuweisen
          bool bereitsVorhanden;
          int adresse = getSID(dekoderUID, &bereitsVorhanden);
          syslog_bus(busnumber, DBG_INFO,
                     "Neue MFX Lok gefunden. UID=%u, zugewiesene Adresse=%d", dekoderUID, adresse);
          //Neuanmeldezähler Inkrement
          registrationCounter++;
          saveRegistrationCounter(registrationCounter);
#if 0
// HACK: das brauchen wir wohl nicht
          //Lokname und Funktionen abfragen, wenn es eine neue Lok ist
          if (! bereitsVorhanden) {
            char lokName[17];
            memset(lokName, 0, sizeof(lokName));
            uint32_t lokFunktionen[MFX_FX_COUNT];
            memset(lokFunktionen, 0, sizeof(lokFunktionen));
            if (readLokNameFx(adresse, lokName, lokFunktionen)) {
              //Alle notwendigen Dekodereigenschaften konnten ausgelesen werden
              //->SRCP INIT Meldung der neuen Lok absetzen.
              //Zusätzliche MFX Daten so  zusammenstellen, wie sie mit "INIT" kommen müssen:
              //<UID> <"Lokname"> <fx0> .. <fx15>
              char optData[(MFX_FX_COUNT + 1) * (10+1) + 16 + 2 + 1 + 10]; //Länge UID und fx max 10, Name 16+2 plus Spaces und etwas Reserve ...
              optData[0] = 0;
              getMFXInitParam(dekoderUID, /*lokName, lokFunktionen,*/ optData);
              //printf("INIT MFX Lok Optdata: %s\n", optData);
              if (cacheInitGL(busnumber, adresse, 'X', 0, 128, MFX_FX_COUNT, optData) != SRCP_OK) {
                syslog_bus(busnumber, DBG_ERROR, "MFX cacheInitGL fail");
              }
            }
            else {
              syslog_bus(busnumber, DBG_WARN,
                         "MFX Lokname und Funktionen konnten nicht gelesen werden. SID=%d Neuanmeldung", adresse);
              //Dekoder SID löschen, Dekoder wird sich wieder neu anmleden
              sendDekoderTerm(adresse);
            }
          }
#endif
        }
      }
    }
#if 0
    else {
      //Beim nächsten Power Up wider von vorne beginnen mit SID Zuordnungen senden
      firstUID = 0;
      lokAdrExist = 0;
      countUIDLoops = 0;
    }
#endif
    usleep(INTERVALL_UID);
  }
  return NULL;
}
#endif

/**
 * Liest die RDS QUAL Leitung ein.
 * @return 0/1
 */
static bool getRDSQual() {
#if defined BANANAPI
  /* TODO for BPi*/    //Auf dem BananaPI muss das noch geklärt werden.
  // BAPI_MFX_RDS_QAL 
    return 0;  
#elif defined RASPBERRYPI
    return  bcm2835_gpio_lev(RPI_MFX_RDS_QAL) == HIGH;
#else
  return false;
#endif
}
/**
 * Liest die RDS CLK Leitung ein.
 * @return 0/1
 */
static bool getRDSClk() {
#if defined BANANAPI
  /* TODO for BPi*/    //Auf dem BananaPI muss das noch geklärt werden.
  // BAPI_MFX_RDS_CLK 
    return 0;
#elif defined RASPBERRYPI
    return  bcm2835_gpio_lev(RPI_MFX_RDS_CLK) == HIGH;
#else
  return false;
#endif
}
/**
 * Liest die RDS DATA Leitung ein.
 * @return 0/1
 */
static bool getRDSData() {
#if defined BANANAPI
  /* TODO for BPi*/    //Auf dem BananaPI muss das noch geklärt werden.
  // BAPI_MFX_RDS_DAT 
    return 0;
#elif defined RASPBERRYPI
    return bcm2835_gpio_lev(RPI_MFX_RDS_DAT) == HIGH;
#else
  return false;
#endif
}

/**
 * MFX Verwaltungsthread:
 * - Periodisches Versenden UID Zentrale und Neuanmeldezähler
 * - Periodisches suchen von noch nicht angemeldeten Dekodern, wenn welche gefunden werden,
 *   dann werden diese angemeldet.
 */
static void *thr_mfxRDSThread(void *threadParam) 
{
  MFXRDSBits mfxRDSBits;
  
#if defined BANANAPI
  /* TODO for BPi*/    //Auf dem BananaPI muss das noch geklärt werden.
  // BAPI_MFX_RDS_QAL 
  // BAPI_MFX_RDS_CLK 
  // BAPI_MFX_RDS_DAT 
#elif defined RASPBERRYPI
  //Notwendige Pins auf Input schalten
  bcm2835_gpio_fsel(RPI_MFX_RDS_QAL, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_set_pud(RPI_MFX_RDS_QAL, BCM2835_GPIO_PUD_OFF); //Kein Pull-Up-Down
  bcm2835_gpio_fsel(RPI_MFX_RDS_CLK, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_set_pud(RPI_MFX_RDS_CLK, BCM2835_GPIO_PUD_OFF); //Kein Pull-Up-Down
  bcm2835_gpio_fsel(RPI_MFX_RDS_DAT, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_set_pud(RPI_MFX_RDS_DAT, BCM2835_GPIO_PUD_OFF); //Kein Pull-Up-Down
#else
  syslog_bus(busnumber, DBG_ERROR,
             "Kein MFX RDS Support auf dieser Plattform");
#endif
  
  for (;;) {
    //Warten auf einen Auftrag
    read(fdRDSNewRx, &mfxRDSBits, sizeof(mfxRDSBits));
    unsigned int rdsBitCount = 0;
    uint16_t rdsCheckSumme = 0;
    switch (mfxRDSBits) {
      case RDS_8: //1Byte
        rdsBitCount = 8;
        break;
      case RDS_16: //2Byte
        rdsBitCount = 16;
        break;
      case RDS_32: //4Bytes
        rdsBitCount = 32;
        break;
      case RDS_64: //8Bytes
        rdsBitCount = 64;
        break;
      default:  ;
    }

    //Es muss nun eine RDS Rückmeldung erfolgen.
    // - Warten bis RDS QUAL Meldung vorliegt
    // - Max. 23 mal 1, dann 010 (Startkennung)
    // - Anzahl erwartetet Datenbits
    // - 8 Bit Checksumme
    //Alles zusammen darf max. 200ms dauern, sonst wird abgebrochen
    struct timeval timeStart;
    gettimeofday(&timeStart, NULL);
    enum state_e {STATE_START1, STATE_START010, STATE_DATA, STATE_CHECK, STATE_FINAL};
    enum state_e state = STATE_START1; //Warten auf QUAL und Data 1
    unsigned int count = 0;
    //Die Antwort
    tMFXRDSFeedback mfxRDSFeedback;
    memset(&mfxRDSFeedback, 0, sizeof(mfxRDSFeedback));
    mfxRDSFeedback.ok = true;

    //RDS Antwort einlesen und verarbeiten    
    while ((state != STATE_FINAL)) {
      //Warte auf nächsten Clock, positive Flanke
      bool clkOld = getRDSClk();
      for (;;) {
        //Daten kommen etwa im 1ms Takt. Mit 200us Wartezeit sollte nichts verpasst werden
        usleep(200);
        bool clk = getRDSClk();
        if ((!clkOld) && clk) {
          //Pos. Flanke erkannt
          //printf("RDS Clk.\n");
          break;
        }
        clkOld = clk;
        //ggf. Abbruch wegen Timeout
		if (timeSince(timeStart) > 200000) {
          //printf("RDS Timeout\n");
          mfxRDSFeedback.ok = false;
          break;
        }
      }
      if (! mfxRDSFeedback.ok) {
        break;
      }
      else {
        switch (state) {
          case STATE_START1:
            //Während Sync Sequenz sollte RDS Qual Meldung vorhandens ein.
            //Wenn nicht -> von vorne
            if (! getRDSQual()) {
              count = 0;
              //printf("RDS Sync Abbruch. QUAL=0.\n");
            }
            else {
              if (getRDSData()) {
                count++; //Wieder ein 1 der Sync. Sequenz eingelesen
                //printf("RDS STATE_START1. count=%d.\n", count);
              }
              else {
                //Von den 23 Bits 1 in der Sync. Sequenz will ich min. die letzten 3 gesetzt sehen, dann kann dieses 0 die Startsquenz sein
                if (count >= 3) {
                  //printf("RDS new STATE_START010.\n", count);
                  count = 0;
                  state = STATE_START010;
                }
                else {
                  //Wieder von vorne beginnen
                  //printf("RDS STATE_START1 Restart. count=0.\n");
                  count = 0;
                }
              }
            }
            break;
          case STATE_START010:
            //Erstes 0 wurde bereits gelesen, es wird noch 10 erwartet
            //printf("RDS STATE_START010 count=%d.\n", count);
            if (count == 0) {
              if (getRDSData()) {
                //1 gelesen, alles OK
                count++;
              }
              else {
                //0, Falsch, Abbruch
                count = 0;
                state = STATE_START1;
                //printf("RDS STATE_START010 Abbruch -> STATE_START1.\n");
              }
            }
            else {
              if (getRDSData()) {
                //1 gelesen, Abbruch
                count = 0;
                state = STATE_START1;
                //printf("RDS STATE_START010 Abbruch -> STATE_START1.\n");
              }
              else {
                //0, OK, Startsquenz ist fertig!
                count = 0;
                state = STATE_DATA;
                //printf("RDS STATE_START010 -> STATE_DATA.\n");
              }
            }
            break;
          case STATE_DATA:
            mfxRDSFeedback.feedback[count / 8] = (mfxRDSFeedback.feedback[count / 8] << 1) | getRDSData();
            count++;
            if (count >= rdsBitCount) {
              state = STATE_CHECK;
              count = 0;
              //printf("RDS STATE_DATA -> STATE_CHECK.\n");
            }
            break;
          case STATE_CHECK:
            rdsCheckSumme = (rdsCheckSumme << 1) | getRDSData();
            count++;
            if (count >= 8) {
              //Checksumme vollständig eingelesen
              state = STATE_FINAL;
              //printf("RDS STATE_CHECK -> STATE_FINAL.\n");
            }
            break;
          default:  ;
        }
      }
    }
    if (state == STATE_FINAL) {
      //Noch Checksumme prüfen
      uint16_t checksum = 0x00FF;
      int i;
      for (i = 0; i < (rdsBitCount / 8); i++) {
        checksum = checksum ^ (checksum << 1) ^ (checksum << 2);
        checksum = checksum ^ mfxRDSFeedback.feedback[i];
        if (checksum & 0x0100) {
          checksum = checksum ^ 0x0107;
        }
        if (checksum & 0x0200) {
          checksum = checksum ^ 0x020E;
        }
      }
      if (checksum == rdsCheckSumme) {
        mfxRDSFeedback.ok = true;
        //printf("RDS Checksumme 0x%x OK. Byte 0=0x%x\n", rdsCheckSumme, mfxRDSFeedback.feedback[0]);
      }
      else {
        mfxRDSFeedback.ok = false;
        //printf("RDS Checksumme falsch.\n");
      }
    }
    //Antwort absenden
    newMFXRDSFeedback(mfxRDSFeedback);
  }
  return NULL;
}

/**
 * Starten der MFX Threads:
 * - MFX Verwaltung (Lokanmekdungen etc.)
 * - MFX RDS Rückmeldungen
 * @param busnumber SRCP Bus
 * @param fdRDSNewRx Pipe, über die der RDS Rückmeldungsthread neue Aufträge erhält.
 *                   Es wir jeweils "MFXRDSBits" übertragen.
 * @return 0 für OK, !=0 für Fehler
 */
int startMFXThreads(bus_t _busnumber, int _fdRDSNewRx)
{
	if (busnumber) return 2;		// don't start threads twice
  	busnumber = _busnumber;
  	fdRDSNewRx = _fdRDSNewRx;
  	
	registrationCounter = loadRegistrationCounter();
	syslog_bus(busnumber, DBG_INFO, 
				"ReRegistration counter initialized with %d", registrationCounter);

  if (sem_init(&semRDSFeedback, 0, 0) != 0) {
    syslog_bus(busnumber, DBG_ERROR,
               "sem_init fail");
    return 1;
  }
  if (pthread_create(&mfxRDSThread, NULL, thr_mfxRDSThread, NULL) != 0) {
    syslog_bus(busnumber, DBG_ERROR,
               "pthread_create thr_mfxRDSThread fail");
    return 1;
  }
  return 0;
}

/**
 * Alle MFX Threads terminieren.
 * @return 0 für OK, !=0 für Fehler
 */
int stopMFXThreads() 
{
  if (pthread_cancel(mfxRDSThread) != 0) {
    return 1;
  }
  if (sem_destroy(&semRDSFeedback) != 0) {
    return 1;
  }
//  saveRegistrationCounter(registrationCounter);
  return 0;
}

/**
 * Schienenadresse im Dekoder löschen -> Dekoder ist nicht mehr angemeldet.
 * @param adresse Aktuell zugeordnete Schienenadresse.
 */
void sendDekoderTerm(int adresse) {
  sendDekoderExist(adresse, 0, false);
}

//---------------------------- SM ----------------------------------------

/**
 * Servicemode ein-ausschalten
 * @param smOn true=SM ein, false=SM aus
 */
void setMfxSM(bool smOn) {
  sm = smOn;
  //printf("MFX SM %d\n", sm);
}

/**
 * SM SET: 1 Byte in den MFX Dekoder an CV/Index schreiben.
 * @param address Lokadresse
 * @param cv CV Adresse
 * @param index Index innerhalb CV
 * @param value Zu schreibendes Byte
 */
int smMfxSetCV(int address, int cv, int index, int value)
{
	if ((cv >= CV_SIZE) || (index >= CVINDEX_SIZE)) return -1;
    if (! buses[busnumber].power_state) return -2;
  
//  checkMFXConfigCache(address);
  //Was neu geschrieben wurde wird im Cache als ungültig markiert damit sichergestellt ist, dass es bei einem Read von der Lok gelesen wird (Verify)
//  mfxKonfigCache.valid[cv][index] = false;
  
	syslog_bus(busnumber, DBG_INFO, "SM MFX Adr=%d, Set CV=%d, Index=%d, Val=%d",
									address, cv, index, value);
  char packetstream[PKTSIZE];
  memset(packetstream, 0, PKTSIZE);
  unsigned int pos = 0;
  addAdrBits(address, packetstream, &pos); //Adresse
  addBits(0b111001, 6, packetstream, &pos); //Kommando CV Schreiben
  addBits(cv, 10, packetstream, &pos); //10 Bit CV
  addBits(index, 6, packetstream, &pos); //6 Bit Index
  addBits(0b00, 2, packetstream, &pos); //Immer 0 (1 Byte)
  addBits(value, 8, packetstream, &pos); //Das zu schreibende Byte
  addCRCBits(packetstream, &pos);
    
  	sendMFXPaket(address, packetstream, QMFX0PKT, 2);
	return 0;
}

/**
 * SM SET: 1 Byte in den MFX Dekoder an CV/Index schreiben.
 * @param address Lokadresse
 * @param cv CV Adresse
 * @param index Index innerhalb CV
 * @return Gelesenes Byte 0..255, < 0 für Error
 */
int smMfxGetCV(int address, int cv, int index, int nmbr) 
{
  //Nur erlaubt wenn SM Initialisiert ist, MFX Loksuche ausgeschaltet
	if (! sm) return -1;
    if (! buses[busnumber].power_state) return -2;
    
  	char result = 0;
	syslog_bus(busnumber, DBG_INFO, "SM GET MFX Adr=%d, Get CV=%d, Index=%d, Len=%d",
									address, cv, index, nmbr);
  	if (readCV(address, cv, index, nmbr, &result)) {
    //printf("SM GET MFX Adr=%d, Get CV=%d, Index=%d Result=%d\n", address, cv, index, result);
    	return result;
  	}
  	//printf("SM GET MFX Adr=%d, Get CV=%d, Index=%d FAIL!\n", address, cv, index);
  	return -1;
}


// new from R.M. in 2018

/* MFX BIND */
// if address == 0 uid is used to set the registration counter
// TODO: clarify type and meaning of return value [-2 ... pos32] 
int smMfxSetBind(int address, uint32_t uid)
{
	if (address == 0) {
		if (registrationCounter != uid) 
  			saveRegistrationCounter(uid);  
		registrationCounter = uid;
		syslog_bus(busnumber, DBG_INFO, "** mfx RegCount set to %x", uid);
		return uid;
	}
    if (! buses[busnumber].power_state) return -2;

	assignSID(uid, address);
	return 0;
}

/* MFX VERIFY */
int smMfxVerBind(int address, uint32_t uid)
{
	if (address == 0) return registrationCounter;

    if (! buses[busnumber].power_state) return -2;
    
	sendDekoderExist(address, uid, false);	// HACK: don't wait for result
	syslog_bus(busnumber, DBG_WARN,
		"***** FAKE RESULT - mfx verify not yet implemented");
	return 123;
}
