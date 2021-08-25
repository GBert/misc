#ifndef MAGNETARTIKEL_H
#define MAGNETARTIKEL_H

#include <boolean.h>
#include <map.h>

typedef struct {
   int Id;
   char Name[32];
   char Typ[32];
   int Stellung;
   int Schaltzeit;
   int Ungerade;
   char Decoder[20];
   char Dectyp[20];
} MagnetartikelInfo;

#define MagnetartikelInfoSetId(Data, Val)         (Data)->Id=Val
#define MagnetartikelInfoSetName(Data, Val)       strcpy((Data)->Name,Val)
#define MagnetartikelInfoSetTyp(Data, Val)        strcpy((Data)->Typ,Val)
#define MagnetartikelInfoSetStellung(Data, Val)   (Data)->Stellung=Val
#define MagnetartikelInfoSetSchaltzeit(Data, Val) (Data)->Schaltzeit=Val
#define MagnetartikelInfoSetUngerade(Data, Val)   (Data)->Ungerade=Val
#define MagnetartikelInfoSetDecoder(Data, Val)    strcpy((Data)->Decoder,Val)
#define MagnetartikelInfoSetDectyp(Data, Val)     strcpy((Data)->Dectyp,Val)

#define MagnetartikelInfoGetId(Data)         (Data)->Id
#define MagnetartikelInfoGetName(Data)       (Data)->Name
#define MagnetartikelInfoGetTyp(Data)        (Data)->Typ
#define MagnetartikelInfoGetStellung(Data)   (Data)->Stellung
#define MagnetartikelInfoGetSchaltzeit(Data) (Data)->Schaltzeit
#define MagnetartikelInfoGetUngerade(Data)   (Data)->Ungerade
#define MagnetartikelInfoGetDecoder(Data)    (Data)->Decoder
#define MagnetartikelInfoGetDectyp(Data)     (Data)->Dectyp

typedef struct {
   char *MagnetartikelFilePath;
   int NumArtikel;
   Map *MagnetartikelDb;
} MagnetartikelStruct;

#define MagnetartikelSetMagnetartikelFilePath(Data,Val) (Data)->MagnetartikelFilePath=Val
#define MagnetartikelSetNumArtikel(Data,Val)            (Data)->NumArtikel=Val
#define MagnetartikelSetMagnetartikelDb(Data,Val)       (Data)->MagnetartikelDb=Val

#define MagnetartikelGetMagnetartikelFilePath(Data) (Data)->MagnetartikelFilePath
#define MagnetartikelGetNumArtikel(Data)            (Data)->NumArtikel
#define MagnetartikelGetMagnetartikelDb(Data)       (Data)->MagnetartikelDb

MagnetartikelStruct *MagnetartikelCreate(void);
void MagnetartikelDestroy(MagnetartikelStruct *Data);
void MagnetartikelInit(MagnetartikelStruct *Data, char *MagnetartikelPath);
void MagnetartikelExit(MagnetartikelStruct *Data);
void MagnetartikelClear(MagnetartikelStruct *Data);
void MagnetartikelInsert(MagnetartikelStruct *Data,
                         MagnetartikelInfo *Magnetartikel);
void MagnetartikelDelete(MagnetartikelStruct *Data,
                         MagnetartikelInfo *Magnetartikel);
MagnetartikelInfo *MagnetartikelSearch(MagnetartikelStruct *Data, int Id);
BOOL MagnetartikelParseMagnetartikelCs2(MagnetartikelStruct *Data, char *Buf, int Len);
void MagnetartikelLoadMagnetartikelCs2(MagnetartikelStruct *Data);
void MagnetartikelSaveMagnetartikelCs2(MagnetartikelStruct *Data);

#endif
