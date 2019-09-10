#ifndef CAN_IO_H
#define CAN_IO_H

/**
* @mainpage io_fkts
*
* Definitionen, um mit I/O Funktionen mit den Handling von CAN Frames
* in den common Funktionen zu kombinieren. Die common Funktionen k&ouml;nnen
* einen CAN Frame von der Ethernet Seite oder von einem CAN Bus unter
* Ber&uuml;cksichtigung der Konfiguration zur "drehscheibe" weiterleiten.
*
* Diese Lib definiert dazu eine Struktur, mit der die n&ouml;tigen
* Funktionen f&uuml;r das Senden und Empfangen den common Funktionen
* bekannt gemacht werden.
*
* Jeder Treiber, der die common Funktionen nutzt,  mu&szlig; eine Initfunktion
* haben, die die IoFktStruct mit den Funktionspointern f&uuml;llt.
*
* @author Michael Bernstein
*/

#include <boolean.h>
#include <mr_cs2ms2.h>

/** @file */

/**
* Dieser Wert wird von der Funktion GetFd() verwendet, um anzuzeigen da&szlig;
* keine weiteren Filedescriptoren vorhanden sind.
*/
#define IOFKT_INVALID_FD -1

typedef struct {
/**
* Dies ist der Zeiger auf private Daten, der von der Initfunktion des Treibers
* angelegt wurde.
*/
   void *private;
/**
* @brief N&auml,chsten filedescriptor holen
*
* Diese Funktion liefert der Reihe nach jeden Socket, den der Treiber benutzt.
* Nach dem letzten Socket liefet der n&auml;chste Aufruf den Wert
* IOFKT_INVALID_FD als Indikator, da&szlig; der letzte Socket geliefert wurde.
* Der danach n&auml;chste Aufruf f&auml;ngt wieder mit dem ersten Socket an.
* Damit kann z.B. der allgemeine Teil alle Sockets f&uuml;r den Aufruf von
* select() ermitteln.
*/
   int (*GetFd)(void *private);
/**
* @brief &Ouml;ffnen des Treibers
*
* Die Open Funktion wird als erste Funktion aufgerufen, um den Treiber zu
* &ouml;ffnen. Hier kann die Verbindung zu einem Server aufgebaut werden. Oder
* selbst einen Socket f&uuml;r einen Dienst ge&ouml;ffnet werden.
*/
   BOOL (*Open)(void *private);
/**
* @brief Schlie&szlig;en des Treibers
*
* Die Close Funktion wird zum Ende aufgerufen und kann die Sockets wieder
* schlie&szlig;en.
*/
   void (*Close)(void *private);
/**
* @brief Lesen von Daten
*
* Diese Funktion wird aufgerufen, wenn auf einem Socket Daten gelesen werden
* k&ouml;nnen. Die Funktion hat folgende Parameter:
*
* @param[in] private Zeiger auf private Daten des Treibers
* @param[in] fd Socket bzw. Filedescriptor, der Daten zum Lesen liefern kann.
* @param[in] PendingData Wird auf TRUE gesetzt, wenn der vorherige Aufruf Daten
* zu&uuml;ckgeliefert hat und der weitere Aufruf noch m&ouml;glicherweise nicht
* gelesen Daten ohne erneute aufruf von select() liefern soll. Siehe dazu die
* Benutzung im common Teil
* @param[out]CanMsg Struktur f&uuml;r eine CAN Nachricht, die Anhand der
* vorliegenden Daten passend gef&uuml;llt werden mu&szlig;.
*
* @return TRUE, wenn die Struktur f&uuml;r eine CAN Nachricht gef&uuml;llt
* wurde, deren Inhalt also g&uuml;ltig ist.
*/
   BOOL (*Read)(void *private, int fd, BOOL PendingData,
                MrCs2CanDataType *CanMsg);
/**
* @brief Schreiben von Daten
*
* Diese Funktion wird aufgerufen, wenn aus dem System verschickt werden sollen.
* Die Funktion hat folgende Parameter:
*
* @param[in] private Zeiger auf private Daten des Treibers
* @param[in] ReceiverSocket Wenn die Message eine Antwort ist, kann dieser
* Parameter den Sender der Anforderung enthalten. Dann werden die Daten nur
* versendet, wenn dies der (oder einer der) eigene Socket ist. Ansonsten hat
* der Parameter den Wert MR_IPC_SOCKET_ALL. Damit ist es z.B. m&ouml;glich,
* die Dateien nur &uuml;ber die Schnittstelle zu senden, von der sie
* angefordert wurden und damit den CAN Bus nicht zu belasten.
* @param[in] CanMsg Struktur mit der zu versendenden CAN Nachricht
*
* @return TRUE, die Daten versendet wurden.
*/
   BOOL (*Write)(void *private, int ReceiverSocket, MrCs2CanDataType *CanMsg);
} IoFktStruct;

#endif
