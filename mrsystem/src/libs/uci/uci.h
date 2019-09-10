#ifndef UCI_H
#define UCI_H

/**
* @mainpage uci
*
* Die Bibliothek uci dient zum Schreiben der Dateien f&uuml;r das PHP System
* UCI als universelles Konfigurationssystem. Es werden die 3 Dateitypem .uci,
* .def und .cfg unterst&uuml;tzt.
*
* Es wird immer zuerst die entsprechende UciOpen*() Funktion aufgerufen. Danach
* kann mit de UciWr*Hdr ein Header in die Datei geschrieen werden (wenn
* unterst&uuml;tzt). Anschlie&szlig;end werden mit den UciWr* Funktionen die
* Werte geschrieben. Und zum Schlu&szlig; wird die Datei mit dem UciClose*
* Makro geschlossen.
*
* @author Michael Bernstein
*/

#include <stdio.h>

/** @file */

#define UCI_UCI_EXTENSION ".uci"
#define UCI_CFG_EXTENSION ".cfg"
#define UCI_DEF_EXTENSION ".def"
#define UCI_HLP_EXTENSION ".hlp"

#define UCI_TYPE_STR "str"
#define UCI_TYPE_IP  "ip"
#define UCI_TYPE_INT "int"
#define UCI_TYPE_CB  "cb"
#define UCI_TYPE_STR "str"
#define UCI_TYPE_SEL "sel"
#define UCI_TYPE_RB  "rb"
#define UCI_TYPE_BIT "bits"
#define UCI_TYPE_TXT "txt"

#define UciCloseUci(FilePointer) fclose(FilePointer)
#define UciCloseCfg(FilePointer) fclose(FilePointer)
#define UciCloseDef(FilePointer) fclose(FilePointer)
#define UciCloseHlp(FilePointer) fclose(FilePointer)

typedef enum { UciSelTypeBut, UciSelTypeTab, UciSelTypeSel } UciSelType;

FILE *UciOpenWr(char *Pfad, char *BaseName, char *Extension);
FILE *UciOpenUciWr(char *Pfad, char *BaseName);
void UciWrUciHdr(FILE *FilePointer, char *CfgName, UciSelType SelType);
void UciWrUciTab(FILE *FilePointer, char *TabName, char *SubDir);
FILE *UciOpenCfgWr(char *Pfad, char *BaseName);
void UciWrCfgInt(FILE *FilePointer, char *Name, int Wert);
void UciWrCfgStr(FILE *FilePointer, char *Name, char *Wert);
FILE *UciOpenDefWr(char *Pfad, char *BaseName);
void UciWrDefHdr(FILE *FilePointer, char *BaseName);
void UciWrDefTxt(FILE *FilePointer, char *Name, unsigned int MinChars,
                 unsigned int MaxChars, char *HelpText);
void UciWrDefStr(FILE *FilePointer, char *Name, unsigned int MinChars,
                 unsigned int MaxChars, char *HelpText);
void UciWrDefInt(FILE *FilePointer, char *Name, int MinVal, int MaxVal,
                 char *HelpText);
void UciWrDefIp(FILE *FilePointer, char *Name, char *HelpText);
void UciWrDefCb(FILE *FilePointer, char *Name, char *HelpText);
void UciWrDefSel(FILE *FilePointer, char *Name, char *SelText, char *HelpText);
void UciWrDefRb(FILE *FilePointer, char *Name, char *RbText, char *HelpText);
void UciWrDefBits(FILE *FilePointer, char *Name, char *BitText, char *HelpText);
void UciWrTxt(FILE *DefFilePointer, FILE *CfgFilePointer, char *Name,
              unsigned int MinChars, unsigned int MaxChars, char *Value);
void UciWrInt(FILE *DefFilePointer, FILE *CfgFilePointer, char *Name,
              int MinVal, int MaxVal, int Value);
FILE *UciOpenHlpWr(char *Pfad, char *BaseName);
void UciWrHlp(FILE *FilePointer, char *Name, char *Beschreibung);

#endif
