#ifndef CS2PARSE_H
#define CS2PARSE_H


#include <boolean.h>
#include <scanner.h>


#define PARSER_ERROR     0x00
#define PARSER_EOF       0x01
#define PARSER_PARAGRAPH 0x02
#define PARSER_VALUE     0x03

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

#define PARSER_TYPE_LOKNAMEN          0x01
#define PARSER_TYPE_LOKINFO           0x02
#define PARSER_TYPE_GERAET_VRS        0x03
#define PARSER_TYPE_LOK_CS2           0x04
#define PARSER_TYPE_GLEISBILD_CS2     0x05
#define PARSER_TYPE_GLEISBILD_SEITE   0x06
#define PARSER_TYPE_MAGNETARTIKEL_CS2 0x07
#define PARSER_TYPE_FAHRSTRASSEN_CS2  0x08
#define PARSER_TYPE_HEADER_CS2        0x09

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

#define Cs2pSetVerbose(Data, Val) (Data)->Verbose=Val
#define Cs2pSetScanner(Data, Scn) (Data)->Scanner=Scn
#define Cs2pSetType(Data, Val)    (Data)->Type=Val
#define Cs2pSetSubType(Data, Val) (Data)->SubType=Val
#define Cs2pSetLevel(Data, Val)   (Data)->Level=Val
#define Cs2pSetName(Data, Str)    strcpy((Data)->Name, Str)
#define Cs2pSetValue(Data, Str)   strcpy((Data)->Value, Str)

#define Cs2pGetVerbose(Data) (Data)->Verbose
#define Cs2pGetScanner(Data) (Data)->Scanner
#define Cs2pGetType(Data)    (Data)->Type
#define Cs2pGetSubType(Data) (Data)->SubType
#define Cs2pGetLevel(Data)   (Data)->Level
#define Cs2pGetName(Data)    (Data)->Name
#define Cs2pGetValue(Data)   (Data)->Value

#endif
