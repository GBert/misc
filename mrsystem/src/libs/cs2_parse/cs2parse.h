#ifndef CS2PARSE_H
#define CS2PARSE_H

/**
* @mainpage cs2_parse
*
* Funktionen f&uuml;r das Einlesen einer *.cs2 Datei
*
* Nach dem Anlegen eines Cs2parser Objektes kann der Parser f&uuml;r
* verschiedene *.cs Dateien initialisiert werden. Danach wird die Parse
* Funktion in einer Schleife so lange aufgerufen, bis das Ende der Daten
* erreicht ist.
*
* @author Michael Bernstein
*/

#include <boolean.h>
#include <scanner.h>

/** @file */

/**
* @addtogroup CS2P_PARSE
*
* @brief Konstanten f&uuml;r den Returnwert der Parse-Funktion
*
* @{
*/
#define PARSER_ERROR     0x00
#define PARSER_EOF       0x01
#define PARSER_PARAGRAPH 0x02
#define PARSER_VALUE     0x03
/** @} */

/**
* @brief Konstanten f&uuml;r den erkannten Paragraphen
*/
#define PARSER_PARAGRAPH_UNDEFINED      0x00
#define PARSER_PARAGRAPH_LOK            0x01
#define PARSER_PARAGRAPH_NUMLOKS        0x02
#define PARSER_PARAGRAPH_LOKOMOTIVE     0x03
#define PARSER_PARAGRAPH_GERAET         0x04
#define PARSER_PARAGRAPH_GLEISBILD      0x05
#define PARSER_PARAGRAPH_MAGNETARTIKEL  0x06
#define PARSER_PARAGRAPH_FAHRSTRASSEN   0x07
#define PARSER_PARAGRAPH_GLEISBILDSEITE 0x08
#define PARSER_PARAGRAPH_LOKSTATUS      0x09
#define PARSER_PARAGRAPH_LOKLISTE       0x0A

/**
* @brief Konstanten f&uuml;r den erkannten Wert
*/
#define PARSER_VALUE_LOK             0x01
#define PARSER_VALUE_NAME            0x02
#define PARSER_VALUE_WERT            0x03
#define PARSER_VALUE_UID             0x04
#define PARSER_VALUE_ADRESSE         0x05
#define PARSER_VALUE_TYP             0x06
#define PARSER_VALUE_MFXUID          0x07
#define PARSER_VALUE_SYMBOL          0x08
#define PARSER_VALUE_AV              0x09
#define PARSER_VALUE_BV              0x0a
#define PARSER_VALUE_VOLUME          0x0b
#define PARSER_VALUE_VELOCITY        0x0c
#define PARSER_VALUE_RICHTUNG        0x0d
#define PARSER_VALUE_VMAX            0x0e
#define PARSER_VALUE_VMIN            0x0f
#define PARSER_VALUE_FKT             0x10
#define PARSER_VALUE_DAUER           0x11
#define PARSER_VALUE_GERAET          0x12
#define PARSER_VALUE_VERSION         0x13
#define PARSER_VALUE_MINOR           0x14
#define PARSER_VALUE_SERNUM          0x15
#define PARSER_VALUE_GFPUID          0x16
#define PARSER_VALUE_GUIUID          0x17
#define PARSER_VALUE_HARDVERS        0x18
#define PARSER_VALUE_MAJOR           0x19
#define PARSER_VALUE_SESSION         0x20
#define PARSER_VALUE_ID              0x21
#define PARSER_VALUE_LOKOMOTIVE      0x22
#define PARSER_VALUE_SID             0x24
#define PARSER_VALUE_ICON            0x25
#define PARSER_VALUE_TACHOMAX        0x26
#define PARSER_VALUE_XPROTOKOLL      0x27
#define PARSER_VALUE_MFXTYP          0x28
#define PARSER_VALUE_STAND           0x29
#define PARSER_VALUE_FAHRT           0x2a
#define PARSER_VALUE_FUNKTIONEN      0x2b
#define PARSER_VALUE_NR              0x2c
#define PARSER_VALUE_VORWAERTS       0x2d
#define PARSER_VALUE_RUECKWAERTS     0x2e
#define PARSER_VALUE_INTRAKTION      0x2f
#define PARSER_VALUE_SEITE           0x30
#define PARSER_VALUE_GROESSE         0x31
#define PARSER_VALUE_ZULETZT_BENUTZT 0x32
#define PARSER_VALUE_ARTIKEL         0x33
#define PARSER_VALUE_STELLUNG        0x34
#define PARSER_VALUE_SCHALTZEIT      0x35
#define PARSER_VALUE_UNGERADE        0x36
#define PARSER_VALUE_DECODER         0x37
#define PARSER_VALUE_DECTYP          0x38
#define PARSER_VALUE_FAHRSTRASSE     0x39
#define PARSER_VALUE_S88             0x3a
#define PARSER_VALUE_S88EIN          0x3b
#define PARSER_VALUE_EXTERN          0x3c
#define PARSER_VALUE_ITEM            0x3d
#define PARSER_VALUE_FSVERWEIS       0x3e
#define PARSER_VALUE_MAGNETARTIKEL   0x3f
#define PARSER_VALUE_ELEMENT         0x40
#define PARSER_VALUE_DREHUNG         0x41
#define PARSER_VALUE_TEXT            0x48
#define PARSER_VALUE_ZUSTAND         0x49
#define PARSER_VALUE_DEVICE_ID       0x50
#define PARSER_VALUE_PAGE            0x51
#define PARSER_VALUE_SEKUNDE         0x52
#define PARSER_VALUE_IDX             0x53
#define PARSER_VALUE_ON              0x54
#define PARSER_VALUE_DV              0x55
#define PARSER_VALUE_LLINDEX         0x56
#define PARSER_VALUE_CRC             0x57

/**
* @addtogroup TEST_GROUP
*
* @brief Konstanten f&uuml;r den Typ der Konfigurationsdatei
*
* @{
*/
#define PARSER_TYPE_LOKNAMEN          0x01
#define PARSER_TYPE_LOKINFO           0x02
#define PARSER_TYPE_GERAET_VRS        0x03
#define PARSER_TYPE_LOK_CS2           0x04
#define PARSER_TYPE_GLEISBILD_CS2     0x05
#define PARSER_TYPE_GLEISBILD_SEITE   0x06
#define PARSER_TYPE_MAGNETARTIKEL_CS2 0x07
#define PARSER_TYPE_FAHRSTRASSEN_CS2  0x08
/** Hiermit wird nur der Header einer *.cs2 kompatiblen Datenstruktur gelesen,
* um zu ermitteln ob es sich um einen Auszug der Loks, Fahrstra&szlig;en,
* Gleisbilder oder Magnetartikel handelt. Damit kann dann der entsprechende
* Parser initialisiert werden. Diese Parsertype kann verwendet werden, wenn
* eine CS2 Slave eine  Konfigdatei ver&auml;ndert und anmschlie&szlig;end die
* &Auml;nderungen zur&uuml;ck sendet.
*/
#define PARSER_TYPE_HEADER_CS2        0x09
#define PARSER_TYPE_LOKLISTE          0x0A
/** @} */

/**
* @brief Datentypen f&uuml;r Funktionen der Konfigurationslib
*/
typedef struct {
   BOOL Verbose;
   Scanner *Scanner;
   int Type;
   int SubType;
   int Level;
   char Name[20];
   char Value[80];
} Cs2parser;

Cs2parser *Cs2pCreate(void);
void Cs2pDestroy(Cs2parser *Data);
void Cs2pInit(Cs2parser *Data, int Type, char *InputLine, int Len);
#define Cs2pExit(Data)
int Cs2pParse(Cs2parser *Data);

/**
* @brief Makros, um Felder im Kommando zu setzen
*/
#define Cs2pSetVerbose(Data, Val) (Data)->Verbose=Val
#define Cs2pSetScanner(Data, Scn) (Data)->Scanner=Scn
#define Cs2pSetType(Data, Val)    (Data)->Type=Val
#define Cs2pSetSubType(Data, Val) (Data)->SubType=Val
#define Cs2pSetLevel(Data, Val)   (Data)->Level=Val
#define Cs2pSetName(Data, Str)    strcpy((Data)->Name, Str)
#define Cs2pSetValue(Data, Str)   strcpy((Data)->Value, Str)

/**
* @brief Makros, um Felder im Kommando zu lesen
*
* Um weitere Informationen, wie den Typ (Verwendung) der erkannten Variable
* oder den Wert selbst zu ermitteln, gibt es eine Reihe von Makros um auf die
* CS3parser Struktur zuzugreifen und Informationen zu lesen bzw. zu setzen.
* Dazu gibt es die folgenden Makros:
*/
#define Cs2pGetVerbose(Data) (Data)->Verbose
#define Cs2pGetScanner(Data) (Data)->Scanner
/** Diese Makro liefert nochmals, welcher Konfigurationstyp (Paragraph, Wert,
* ...) erkant wurde. Also der Returnwert der Funktion Cs2pParse()
*/
#define Cs2pGetType(Data)    (Data)->Type
/** Wurde ein neuer Paragraph oder ein neuer Value gefunden, dann liefert das
* Makro Cs2pGetSubType um welchen Paragraphen bzw. welche Wert es sich handelt.
*/
#define Cs2pGetSubType(Data) (Data)->SubType
/** Die *.cs2 Dateien haben eine Baumstruktur, wobei die Ebene durch die
* Anzahl Punkte vor dem Schl&uuml;sselwort festgelegt ist. Dieses Makro
* liefert die Ebene bzw. die Anzahl Punkte.
*/
#define Cs2pGetLevel(Data)   (Data)->Level
#define Cs2pGetName(Data)    (Data)->Name
/** Dieses Makrot liefert den Wert einer Variable als Zeichenkette. Also so,
* wie sie in der Datei steht. Wenn diese Zeichenkette einen numerischen Wert
* repr&auml;sentieren soll, mu&szlig; er noch in eine Zahl umgewandelt werden.
*/
#define Cs2pGetValue(Data)   (Data)->Value

#endif
