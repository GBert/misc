#ifndef GLEISBILD_H
#define GLEISBILD_H

#include <boolean.h>
#include <map.h>

#define GLEIBILD_PAGE_SUBDIR "gleisbilder"

typedef struct {
   int Id;
   char Name[32];
   int Zustand;
} GleisbildInfo;

#define GleisbildInfoSetId(Data, Val)      (Data)->Id=Val
#define GleisbildInfoSetName(Data, Val)    strcpy((Data)->Name,Val)
#define GleisbildInfoSetZustand(Data, Val) (Data)->Zustand=Val

#define GleisbildInfoGetId(Data)      (Data)->Id
#define GleisbildInfoGetName(Data)    (Data)->Name
#define GleisbildInfoGetZustand(Data) (Data)->Zustand

typedef struct {
   char *GleisbildFilePath;
   int NumPages;
   Map *GleisbildDb;
} GleisbildStruct;

#define GleisbildSetGleisbildFilePath(Data,Val) (Data)->GleisbildFilePath=Val
#define GleisbildSetNumPages(Data,Val)          (Data)->NumPages=Val
#define GleisbildSetGleisbildDb(Data,Val)       (Data)->GleisbildDb=Val

#define GleisbildGetGleisbildFilePath(Data) (Data)->GleisbildFilePath
#define GleisbildGetNumPages(Data)          (Data)->NumPages
#define GleisbildGetGleisbildDb(Data)       (Data)->GleisbildDb

GleisbildStruct *GleisbildCreate(void);
void GleisbildDestroy(GleisbildStruct *Data);
void GleisbildInit(GleisbildStruct *Data, char *GleisbildPath);
void GleisbildExit(GleisbildStruct *Data);
void GleisbildClear(GleisbildStruct *Data);
void GleisbildInsert(GleisbildStruct *Data, GleisbildInfo *Gleisbild);
void GleisbildDelete(GleisbildStruct *Data, GleisbildInfo *Gleisbild);
GleisbildInfo *GleisbildSearch(GleisbildStruct *Data, int Id);
BOOL GleisbildParseGleisbildCs2(GleisbildStruct *Data, char *Buf, int Len);
void GleisbildLoadGleisbildCs2(GleisbildStruct *Data);
void GleisbildSaveGleisbildCs2(GleisbildStruct *Data);

#endif
