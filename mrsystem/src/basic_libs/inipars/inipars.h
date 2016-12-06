#ifndef INIPARS_H
#define INIPARS_H


#include <boolean.h>
#include <map.h>
#include <scanner.h>

#define INI_PARS_FIRST_KEYWORD_TOK (NoSy + 1)

typedef enum { IniParsString, IniParsInt } IniParsDataType;
typedef struct {   IniParsDataType DataType;   int IntValue;   char StringValue[SCAN_BLOCK_LAENGE];} IniValue;
typedef struct {
   char Keyword[SCAN_BLOCK_LAENGE];
   int Symbol;
   IniParsDataType DataType;
} IniParsKeyword;

typedef struct {
   Scanner *IniScanner;
   int AnzKeywords;
   IniParsKeyword *Keywords;
} IniParsStruct;

IniParsStruct *IniParsCreate(void);
void IniParsDestroy(IniParsStruct *Data);
void IniParsInit(IniParsStruct *Data, char *DateiName, int AnzKeywords,
                 IniParsKeyword *Keywords);
void IniParsExit(IniParsStruct *Data);
BOOL IniParsReadFile(IniParsStruct *Data, Map *CfgValues);

#define IniParsSetScanner(Data,Scn)     (Data)->IniScanner=Scn
#define IniParsSetAnzKeywords(Data,Anz) (Data)->AnzKeywords=Anz
#define IniParsSetKeywords(Data,Keys)   (Data)->Keywords=Keys
#define IniParsGetScanner(Data)     (Data)->IniScanner
#define IniParsGetAnzKeywords(Data) (Data)->AnzKeywords
#define IniParsGetKeywords(Data)    (Data)->Keywords

#endif
