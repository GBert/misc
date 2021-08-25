#ifndef GLEISBILDPAGE_H
#define GLEISBILDPAGE_H

#include <boolean.h>
#include <map.h>

typedef struct {
   int Id;
   char Typ[32];
   int Drehung;
   int Artikel;
   char Text[64];
   int Zustand;
   int DeviceId;
} GleisbildBildElement;

#define GleisbildBildElementSetId(Data, Val)       (Data)->Id=Val
#define GleisbildBildElementSetTyp(Data, Val)      strcpy((Data)->Typ,Val)
#define GleisbildBildElementSetDrehung(Data, Val)  (Data)->Drehung=Val
#define GleisbildBildElementSetArtikel(Data, Val)  (Data)->Artikel=Val
#define GleisbildBildElementSetText(Data, Val)     strcpy((Data)->Text,Val)
#define GleisbildBildElementSetZustand(Data, Val)  (Data)->Zustand=Val
#define GleisbildBildElementSetDeviceId(Data, Val) (Data)->DeviceId=Val

#define GleisbildBildElementGetId(Data)       (Data)->Id
#define GleisbildBildElementGetTyp(Data)      (Data)->Typ
#define GleisbildBildElementGetDrehung(Data)  (Data)->Drehung
#define GleisbildBildElementGetArtikel(Data)  (Data)->Artikel
#define GleisbildBildElementGetText(Data)     (Data)->Text
#define GleisbildBildElementGetZustand(Data)  (Data)->Zustand
#define GleisbildBildElementGetDeviceId(Data) (Data)->DeviceId

typedef struct {
   char *GleisbildName;
   char *GleisbildPageFilePath;
   int NumElements;
   int Page;
   Map *ElementDb;
} GleisbildPageStruct;

#define GleisbildPageSetGleisbildName(Data,Val)         (Data)->GleisbildName=Val
#define GleisbildPageSetGleisbildPageFilePath(Data,Val) (Data)->GleisbildPageFilePath=Val
#define GleisbildPageSetNumElements(Data,Val)           (Data)->NumElements=Val
#define GleisbildPageSetPage(Data,Val)                  (Data)->Page=Val
#define GleisbildPageSetElementDb(Data,Val)             (Data)->ElementDb=Val

#define GleisbildPageStructGetName(Data)                  (Data)->GleisbildName
#define GleisbildPageStructGetGleisbildPageFilePath(Data) (Data)->GleisbildPageFilePath
#define GleisbildPageStructGetNumElements(Data)           (Data)->NumElements
#define GleisbildPageStructGetPage(Data)                  (Data)->Page
#define GleisbildPageStructGetElementDb(Data)             (Data)->ElementDb

GleisbildPageStruct *GleisbildPageCreate(void);
void GleisbildPageDestroy(GleisbildPageStruct *Data);
void GleisbildPageInit(GleisbildPageStruct *Data, char *GleisbildPagePath,
                       char *GleisbildName, int PageNum);
void GleisbildPageExit(GleisbildPageStruct *Data);
void GleisbildPageClear(GleisbildPageStruct *Data);
void GleisbildPageInsert(GleisbildPageStruct *Data,
                         GleisbildBildElement *Element);
void GleisbildPageDelete(GleisbildPageStruct *Data,
                         GleisbildBildElement *PageItem);
GleisbildBildElement *GleisbildPageSearch(GleisbildPageStruct *Data, int Id);
BOOL GleisbildPageParseGleisbildPageCs2(GleisbildPageStruct *Data,
                                        char *Buf, int Len);
void GleisbildPageLoadGleisbildPageCs2(GleisbildPageStruct *Data);
void GleisbildPageSaveGleisbildPageCs2(GleisbildPageStruct *Data);
char *GleisbildPageGetName(GleisbildPageStruct *Data);

#endif
