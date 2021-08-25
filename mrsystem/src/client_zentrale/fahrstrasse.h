#ifndef FAHRSTRASSE_H
#define FAHRSTRASSE_H

#include <boolean.h>
#include <map.h>

typedef struct {
   int FsVerweis;
   char Typ[32];
   char Lok[32];
   int Magnetartikel;
   int Stellung;
   float Sekunde;
   int Idx;
   int Wert;
} FahrstrasseItem;

#define FahrstrasseItemSetFsVerweis(Data, Val)     (Data)->FsVerweis=Val
#define FahrstrasseItemSetTyp(Data, Val)           strcpy((Data)->Typ,Val)
#define FahrstrasseItemSetLok(Data, Val)           strcpy((Data)->Lok,Val)
#define FahrstrasseItemSetMagnetartikel(Data, Val) (Data)->Magnetartikel=Val
#define FahrstrasseItemSetStellung(Data, Val)      (Data)->Stellung=Val
#define FahrstrasseItemSetSekunde(Data, Val)       (Data)->Sekunde=Val
#define FahrstrasseItemSetIdx(Data, Val)           (Data)->Idx=Val
#define FahrstrasseItemSetWert(Data, Val)          (Data)->Wert=Val

#define FahrstrasseItemGetFsVerweis(Data)     (Data)->FsVerweis
#define FahrstrasseItemGetTyp(Data)           (Data)->Typ
#define FahrstrasseItemGetLok(Data)           (Data)->Lok
#define FahrstrasseItemGetMagnetartikel(Data) (Data)->Magnetartikel
#define FahrstrasseItemGetStellung(Data)      (Data)->Stellung
#define FahrstrasseItemGetSekunde(Data)       (Data)->Sekunde
#define FahrstrasseItemGetIdx(Data)           (Data)->Idx
#define FahrstrasseItemGetWert(Data)          (Data)->Wert

typedef struct {
   int Id;
   char Name[32];
   int On;
   int S88;
   int S88Ein;
   int Extern;
   Map *ItemDb;
} FahrstrasseInfo;

#define FahrstrasseInfoSetId(Data, Val)     (Data)->Id=Val
#define FahrstrasseInfoSetName(Data, Val)   strcpy((Data)->Name,Val)
#define FahrstrasseInfoSetOn(Data, Val)     (Data)->On=Val
#define FahrstrasseInfoSetS88(Data, Val)    (Data)->S88=Val
#define FahrstrasseInfoSetS88Ein(Data, Val) (Data)->S88Ein=Val
#define FahrstrasseInfoSetExtern(Data, Val) (Data)->Extern=Val
#define FahrstrasseInfoSetItemDb(Data, Val) (Data)->ItemDb=Val

#define FahrstrasseInfoGetId(Data)     (Data)->Id
#define FahrstrasseInfoGetName(Data)   (Data)->Name
#define FahrstrasseInfoGetOn(Data)     (Data)->On
#define FahrstrasseInfoGetS88(Data)    (Data)->S88
#define FahrstrasseInfoGetS88Ein(Data) (Data)->S88Ein
#define FahrstrasseInfoGetExtern(Data) (Data)->Extern
#define FahrstrasseInfoGetItemDb(Data) (Data)->ItemDb

typedef struct {
   char *FahrstrasseFilePath;
   int NumPages;
   Map *FahrstrasseDb;
} FahrstrasseStruct;

#define FahrstrasseSetFahrstrasseFilePath(Data,Val) (Data)->FahrstrasseFilePath=Val
#define FahrstrasseSetNumPages(Data,Val)            (Data)->NumPages=Val
#define FahrstrasseSetFahrstrasseDb(Data,Val)       (Data)->FahrstrasseDb=Val

#define FahrstrasseGetFahrstrasseFilePath(Data) (Data)->FahrstrasseFilePath
#define FahrstrasseGetNumPages(Data)            (Data)->NumPages
#define FahrstrasseGetFahrstrasseDb(Data)       (Data)->FahrstrasseDb

FahrstrasseStruct *FahrstrasseCreate(void);
void FahrstrasseDestroy(FahrstrasseStruct *Data);
void FahrstrasseInit(FahrstrasseStruct *Data, char *FahrstrassePath);
void FahrstrasseExit(FahrstrasseStruct *Data);
void FahrstrasseClear(FahrstrasseStruct *Data);
void FahrstrasseInsert(FahrstrasseStruct *Data, FahrstrasseInfo *Fahrstrasse);
void FahrstrasseDelete(FahrstrasseStruct *Data, FahrstrasseInfo *Fahrstrasse);
FahrstrasseInfo *FahrstrasseSearch(FahrstrasseStruct *Data, int Id);
void FahrstrasseItemInsert(FahrstrasseInfo *Data, int Nr,
                            FahrstrasseItem *Item);
void FahrstrasseDeleteItem(FahrstrasseInfo *Data, int Nr);
FahrstrasseItem *FahrstrasseSearchItem(FahrstrasseInfo *Data, int Nr);
BOOL FahrstrasseParseFahrstrasseCs2(FahrstrasseStruct *Data, char *Buf, int Len);
void FahrstrasseLoadFahrstrasseCs2(FahrstrasseStruct *Data);
void FahrstrasseSaveFahrstrasseCs2(FahrstrasseStruct *Data);

#endif
