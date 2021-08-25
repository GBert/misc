/*
 *  Created by Maximilian Goldschmidt <maxigoldschmidt@gmail.com>
 *  If you need Help, feel free to contact me any time!
 *  Do with this whatever you want, but keep thes Header and tell
 *  the others what you changed!
 *
 *  Last changed: 2017-01-06
 * 
 *  Joerg Dorchain <joerg@dorchain.net> 26.12.2017
 *  Ported to C and linux socketcan
 */


#ifndef _MCAN_H_
#define _MCAN_H_
#include <stdint.h>
#include <stdbool.h>
#include <linux/can.h>

/*
 *  Gerätetypen (MäCAN):
 *  Dienen nur zur Unterscheidung beim Ping, hat keine auswirkungen auf den Betrieb.
 */
#define MCAN_MAGNET	0x0050
#define MCAN_SERVO 	0x0051
#define MCAN_RELAIS	0x0052
#define MCAN_STELLPULT	0x0053
#define MCAN_S88_GBS	0x0054


/*
 *  Adressbereich der Local-IDs:
 */
#define MM_ACC		0x3000	//Magnetartikel Motorola
#define DCC_ACC		0x3800	//Magbetartikel NRMA_DCC
#define MM_TRACK	0x0000	//Gleissignal Motorola
#define DCC_TRACK	0xC000	//Gleissignal NRMA_DCC

/*
 *  CAN-Befehle (Märklin)
 */
#define SYS_CMD		0x00 	//Systembefehle
 #define SYS_STOP 	0x00 	//System - Stopp
 #define SYS_GO		0x01	//System - Go
 #define SYS_HALT	0x02	//System - Halt
 #define SYS_STAT	0x0b	//System - Status (sendet geänderte Konfiguration oder übermittelt Messwerte)
 #define SYS_ID		0x0c	//System - ID Frage / setzen fuer Rueckmelder
#define SWITCH_ACC 	0x0b	//Magnetartikel Schalten
#define S88_EVENT	0x11	//Rückmelde-Event
#define PING		0x18	//CAN-Teilnehmer anpingen
#define CONFIG		0x1d	//Konfiguration

typedef struct {
	uint8_t cmd;			//CAN-Befehl (siehe oben)
	uint16_t hash;		//zuvor mit generateHash berechneter Hash
	bool resp_bit;		//gesetztes oder nicht gesetztes Response Bit
	uint8_t dlc;			//Anzahl der Datenbytes
	uint8_t data[8];	//Datenbytes
} MCANMSG;

typedef struct {
	uint8_t versHigh;		//Versionsnummer vor dem Punkt
	uint8_t versLow;		//Versionsnummer nach dem Punkt
	char *name;			  //Name des Geräts
	char *artNum;			//Artikelnummer des Geräts
	int boardNum;			  //Nummer des Geräts
	uint16_t hash;			//Hash des Geräts (muss vor her mit generateHash() berechnet werden)
	uint32_t uid;			  //UID des Geräts
	uint16_t type;			//Typ des Geräts (z.B. MäCAN Magnetartikeldecoder: 0x0050)
} CanDevice;

/* debug */

void print_net_frm(struct can_frame *frm);
void print_canmsg(MCANMSG *msg);

/******************************************************************************
 *  Name: generateHash
 *  Funktion: Hash aus der UID berechnen
 *  Parameter: zuvor festgelegte UID (auf der Anlage einzigartig!)
 ******************************************************************************/
uint16_t generateHash(uint32_t uid);

/******************************************************************************
 *  Name: generateLocId
 *  Funktion: Local-ID aus der Adresse und dem Protokoll berechnen.
 *  Parameter: Protokollbasis (siehe hoben), Adresse
 ******************************************************************************/
uint16_t generateLocId(uint16_t prot, uint16_t adrs);

/******************************************************************************
 *  Name: getadrs
 *  Funktion: Adress aus der Local-ID und dem Protokoll berechnen.
 *  Parameter: Protokollbasis (siehe hoben), Local-ID
 ******************************************************************************/
uint16_t getadrs(uint16_t prot, uint16_t locid);

/******************************************************************************
 *  Name: initMCAN
 *  Funktion: Initialisiert den CAN-Bus mit allen spezifischen Einstellungen
 *  Parameter: keiner oder "true", um den Debug-Modus zu nutzen.
 ******************************************************************************/
void initMCAN();

/******************************************************************************
 *  Name: sendCanFrame
 *  Funktion: Versendet eine CAN-Nachricht über den Bus (siehe MCANMSG)
 *  Parameter: CAN-Nachricht (MCANMSG)
 ******************************************************************************/
void sendCanFrame(int canfd, MCANMSG can_frame);

/******************************************************************************
 *  Name: sendDeviceInfo
 *  Funktion: Übertragt Gerateinformationen (CanDevice, siehe oben) an die
 *            Zentrale. Antwort auf Konfig-kanal 0.
 *  Parameter: Geräteinformationen (CanDevice), Anzahl der Konfig-Kanäle.
 ******************************************************************************/
void sendDeviceInfo(int canfd, CanDevice device, int configNum);		//IMMER Kanal 0!

/******************************************************************************
 *  Name: sendConfigDropdown
 *  Funktion: Übermittelt einen Konfigrationspunkt mit Auswahlliste an die CS2.
 *  Parameter: Geräteinformationen, Konfig-Kanal, Anzahl der
 *             Auswahlmöglichkeiten, Standar-/derzeitige Einstellung,
 *             String (genauere erklärung folgt).
 ******************************************************************************/
void sendConfigInfoDropdown(int canfd, CanDevice device, uint8_t configChanel, uint8_t numberOfOptions, uint8_t defaultSetting, char *settings);

/******************************************************************************
 *  Name: sendConfigInfoSlider
 *  Funktion: Übermittelt einen Konfigrationspunkt mit Wertauswahl an die CS2.
 *  Parameter: Geräteinformationen, Konfig-Kanal, Minimalwert, Maximalwert,
 *             Standard-/derzeitiger Wert, String (genauere erklärung folgt).
 ******************************************************************************/
void sendConfigInfoSlider(int canfd, CanDevice device, uint8_t configChanel, uint16_t lowerValue, uint16_t upperValue, uint16_t defaultValue, char *settings);

/******************************************************************************
 *  Name: sendPingFrame
 *  Funktion: Sendet eine Pinganfrage oder -antwort.
 *  Parameter: Geräteinformationen, Anfrage (false) oder Antwort (true).
 ******************************************************************************/
void sendPingFrame(int canfd, CanDevice device, bool response);

/******************************************************************************
 *  Name: switchAccResponse
 *  Funktion: Meldung nach erfolgreichem Schalten.
 *  Parameter: Geräteinformationen, Local-ID des geschaltenden Zubehörs,
 *             Zustand des Zubehörs.
 ******************************************************************************/
void switchAccResponse(int canfd, CanDevice device, uint32_t locId, bool state);

/******************************************************************************
 *  Name: sendAccessoryFrame
 *  Funktion: Sendet einen Schaltauftrag oder die Antwort auf einen.
 *  Parameter: Geräteinformationen, Local-ID des zu schaltenden Zubehörs,
 *             Zustand des Zubehörs, Auftrag (false) oder Antwort (true).
 ******************************************************************************/
void sendAccessoryFrame(int canfd, CanDevice device, uint32_t locId, bool state, bool response, bool power);

/******************************************************************************
 *  Name: checkS88StateFrame
 *  Funktion: Fragt den aktuellen Zustand eines Rückmeldekontakt ab.
 *  Parameter: Geräteinformationen, Gerätekennung, Kontaktkennung
 *              des abzufragenden Kontakt.
 ******************************************************************************/
void checkS88StateFrame(int canfd, CanDevice device, uint16_t dev_id, uint16_t contact_id);

/******************************************************************************
 *  Name: sendS88Event
 *  Funktion: Sendet aktuellen und alten Zustand eines Rueckmeldekontakts
 *  Parameter: Geräteinformationen, Gerätekennung, Kontaktkennung,
 *              alter Zustand, neuer Zustand des Kontakt.
 ******************************************************************************/
void sendS88Event(int canfd, CanDevice device, uint16_t dev_id, uint16_t contact_id, uint8_t old, uint8_t new);


/******************************************************************************
 *  Name: getCanFrame
 *  Funktion: Liest eine CAN-Nachricht aus falls verfügbar (Interrupt LOW).
 *  Rückgabe: CAN-Nachricht (MCANMSG).
 ******************************************************************************/
MCANMSG getCanFrame();

/******************************************************************************
 *  Name: checkAccessoryFrame
 *  Funktion: Prüft, ob ein eingehender Schaltauftrag an die gespeicherten
 *            Local-IDs gerichtet ist.
 *  Parameter: CAN-Nachricht, Feld der Local-IDs, anzahl der Local-IDs, auf
 *             Auftrag (false) oder Antwort (true) prüfen.
 *  Rückgabe: Nummer der Local-ID aus dem Feld, -1 bei unbekannter Local-ID
 *            oder wenn es sich nicht um einen Schaltauftrag handelt.
 ******************************************************************************/
int checkAccessoryFrame(MCANMSG can_frame, uint16_t locIds[], int accNum, bool response);

/******************************************************************************
 *  Name: statusResponse
 *  Funktion: Sendet Antwort auf Konfigurationskanal
 *  Parameter: Konfigurationskanal
 ******************************************************************************/
void statusResponse(int canfd, CanDevice device, int chanel, int success);

#endif /* _MCAN_H_ */
