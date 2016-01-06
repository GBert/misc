#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include <string.h>
#include <boolean.h>

#define SCAN_BLOCK_LAENGE 80

typedef enum {PlusSy, MinusSy, SternSy, TeilSy, GleichSy, KleinerSy,
             GroesserSy, PunktSy, KommaSy, SemicolonSy, PfeilSy,
             KlammeraufSy, KlammerzuSy, MengeaufSy, MengezuSy,
             UngleichSy, KleinergleichSy, GroessergleichSy, HashSy,
             DoppelpunktSy, BezeichnerSy, ZeichenkettenSy, GzahlSy,
             Rzahlsy, EolnSy, EofSy, NoSy} ScanBasicSymbol;

typedef struct {
   char Keyword[SCAN_BLOCK_LAENGE];
   int Symbol;
} ScanKeyword;

typedef struct {
   int AktZeichen;
   int AnzZeichen;
   FILE *ScanFile;
   char *Puffer;
   char StaticPuffer[SCAN_BLOCK_LAENGE + 1];
   int Sym;
   int NumKeywords;
   ScanKeyword *Keywords;
   char zk[SCAN_BLOCK_LAENGE];
   int i;
} Scanner;

typedef int ScanSymbol;

Scanner *ScanCreate(void);
void ScanDestroy(Scanner *Handle);
ScanSymbol ScanEol(Scanner *Handle);
void ScanInit(Scanner *Handle, char *DateiName, char *Zeile, int Laenge,
              int AnzKeywords, ScanKeyword *Keywords);
void ScanExit(Scanner *Handle);
ScanSymbol Scan(Scanner *Handle);
BOOL ScanSuche(Scanner *Handle, char *Wen);
void ScanUeberlesen(Scanner *Handle);

#define ScanString(Handle,str) strcpy(str,Handle->zk)
#define ScanGetString(Handle) Handle->zk
#define ScanGanz(Handle) Handle->i
#define ScanReal(Handle) Handle->i

#endif
