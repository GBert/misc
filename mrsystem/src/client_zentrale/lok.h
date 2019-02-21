#ifndef LOK_H
#define LOK_H

#include <boolean.h>
#include <map.h>

#define LOK_MAX_LOKS 40
#define LOK_NUM_FUNCTIONS 32

typedef struct {
   int Typ;
   int Dauer;
   int Wert;
} LokFktTyp;

#define LokFktSetTyp(Data, Val)   (Data)->Typ=Val
#define LokFktSetDauer(Data, Val) (Data)->Dauer=Val
#define LokFktSetWert(Data, Val)  (Data)->Wert=Val

#define LokFktGetTyp(Data, Val)   (Data)->Typ
#define LokFktGetDauer(Data, Val) (Data)->Dauer
#define LokFktGetWert(Data, Val)  (Data)->Wert

typedef struct {
   BOOL IsDeleted;
   unsigned long Uid;
   char Name[17];
   int Adresse;
   char Typ[20];
   unsigned long MfxUid;
   int Symbol;
   int Av;
   int Bv;
   int Volume;
   int Velocity;
   int Richtung;
   int Vmax;
   int Vmin;
   LokFktTyp Fkt[LOK_NUM_FUNCTIONS];
} LokInfo;

#define LokInfoSetIsDeleted(Data, Val) (Data)->IsDeleted=Val
#define LokInfoSetUid(Data, Val)       (Data)->Uid=Val
#define LokInfoSetName(Data, Val)      strcpy((Data)->Name,Val)
#define LokInfoSetAdresse(Data, Val)   (Data)->Adresse=Val
#define LokInfoSetTyp(Data, Val)       strcpy((Data)->Typ,Val)
#define LokInfoSetMfxUid(Data, Val)    (Data)->MfxUid=Val
#define LokInfoSetSymbol(Data, Val)    (Data)->Symbol=Val
#define LokInfoSetAv(Data, Val)        (Data)->Av=Val
#define LokInfoSetBv(Data, Val)        (Data)->Bv=Val
#define LokInfoSetVolume(Data, Val)    (Data)->Volume=Val
#define LokInfoSetVelocity(Data, Val)  (Data)->Velocity=Val
#define LokInfoSetRichtung(Data, Val)  (Data)->Richtung=Val
#define LokInfoSetVmax(Data, Val)      (Data)->Vmax=Val
#define LokInfoSetVmin(Data, Val)      (Data)->Vmin=Val

#define LokInfoSetFktTyp(Data, Idx, Val)   (Data)->Fkt[Idx].Typ=Val
#define LokInfoSetFktDauer(Data, Idx, Val) (Data)->Fkt[Idx].Dauer=Val
#define LokInfoSetFktWert(Data, Idx, Val)  (Data)->Fkt[Idx].Wert=Val

#define LokInfoGetIsDeleted(Data) (Data)->IsDeleted
#define LokInfoGetUid(Data)       (Data)->Uid
#define LokInfoGetName(Data)      (Data)->Name
#define LokInfoGetAdresse(Data)   (Data)->Adresse
#define LokInfoGetTyp(Data)       (Data)->Typ
#define LokInfoGetMfxUid(Data)    (Data)->MfxUid
#define LokInfoGetSymbol(Data)    (Data)->Symbol
#define LokInfoGetAv(Data)        (Data)->Av
#define LokInfoGetBv(Data)        (Data)->Bv
#define LokInfoGetVolume(Data)    (Data)->Volume
#define LokInfoGetVelocity(Data)  (Data)->Velocity
#define LokInfoGetRichtung(Data)  (Data)->Richtung
#define LokInfoGetVmax(Data)      (Data)->Vmax
#define LokInfoGetVmin(Data)      (Data)->Vmin

#define LokInfoGetFktTyp(Data, Idx)   (Data)->Fkt[Idx].Typ
#define LokInfoGetFktDauer(Data, Idx) (Data)->Fkt[Idx].Dauer
#define LokInfoGetFktWert(Data, Idx)  (Data)->Fkt[Idx].Wert

typedef struct {
   char *LocFilePath;
   int NumLoks;
   int NumLokFkts;
   BOOL IsChanged;
   Map *LokDb;
} LokStruct;

#define LokSetLocFilePath(Data,Val) (Data)->LocFilePath=Val
#define LokSetNumLoks(Data,Val)     (Data)->NumLoks=Val
#define LokSetNumLokFkts(Data,Val)  (Data)->NumLokFkts=Val
#define LokSetIsChanged(Data,Val)   (Data)->IsChanged=Val
#define LokSetLokDb(Data,Val)       (Data)->LokDb=Val

#define LokGetLocFilePath(Data) (Data)->LocFilePath
#define LokGetNumLoks(Data)     (Data)->NumLoks
#define LokGetNumLokFkts(Data)  (Data)->NumLokFkts
#define LokGetIsChanged(Data)   (Data)->IsChanged
#define LokGetLokDb(Data)       (Data)->LokDb

LokStruct *LokCreate(void);
void LokDestroy(LokStruct *Data);
void LokInit(LokStruct *Data, char *LocPath, int NumLokFkts);
void LokExit(LokStruct *Data);
void LokClear(LokStruct *Data);
void LokInfoInit(LokInfo *Data);
void LokInsert(LokStruct *Data, LokInfo *Lok);
LokInfo *LokSearch(LokStruct *Data, unsigned long Addr);
BOOL LokParseLokomotiveCs2(LokStruct *Data, char *Buf, int Len);
void LokLoadLokomotiveCs2(LokStruct *Data);
void LokSaveLokomotiveCs2(LokStruct *Data);
void LokMarkAllDeleted(LokStruct *Data);
void LokMarkDeleted(LokStruct *Data, char *Name);
void LokMarkAllUndeleted(LokStruct *Data);
BOOL LokHaveDeleted(LokStruct *Data);

#endif
