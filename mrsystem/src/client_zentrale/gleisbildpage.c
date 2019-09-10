#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <boolean.h>
#include <map.h>
#include <mr_cs2ms2.h>
#include <cs2.h>
#include <cs2parse.h>
#include <write_cs2.h>
#include "gleisbildpage.h"
#include <stdio.h>

static char PageName[128];

GleisbildPageStruct *GleisbildPageCreate(void)
{  GleisbildPageStruct *NewData;

   NewData = (GleisbildPageStruct *)malloc(sizeof(GleisbildPageStruct));
   if (NewData != (GleisbildPageStruct *)NULL)
   {
      GleisbildPageSetGleisbildPageFilePath(NewData, "/var/www/config/");
      GleisbildPageSetNumElements(NewData, 0);
      GleisbildPageSetElementDb(NewData, MapCreate());
      if (GleisbildPageStructGetElementDb(NewData) == (Map *)NULL)
      {
         free(NewData);
         NewData = (GleisbildPageStruct *)NULL;
      }
   }
   return(NewData);
}

void GleisbildPageDestroy(GleisbildPageStruct *Data)
{
   if (GleisbildPageStructGetElementDb(Data) != (Map *)NULL)
      MapDestroy(GleisbildPageStructGetElementDb(Data));
   free(Data);
}

static int GleisbildPageIdCmp(void *d1, void *d2)
{
   return((int)d1 - (int)d2);
}

void GleisbildPageInit(GleisbildPageStruct *Data, char *GleisbildPagePath,
                       char *GleisbildName, int PageNum)
{
   GleisbildPageSetGleisbildPageFilePath(Data, GleisbildPagePath);
   GleisbildPageSetGleisbildName(Data, GleisbildName);
   GleisbildPageSetNumElements(Data, 0);
   GleisbildPageSetPage(Data, PageNum);
   MapInit(GleisbildPageStructGetElementDb(Data), (CmpFkt)GleisbildPageIdCmp,
           (MapKeyDelCbFkt)NULL, (MapDataDelCbFkt)free);
}

void GleisbildPageExit(GleisbildPageStruct *Data)
{
}

void GleisbildPageClear(GleisbildPageStruct *Data)
{
   if (GleisbildPageStructGetElementDb(Data) != (Map *)NULL)
      MapDestroy(GleisbildPageStructGetElementDb(Data));
   GleisbildPageSetNumElements(Data, 0);
   GleisbildPageSetElementDb(Data, MapCreate());
   if (GleisbildPageStructGetElementDb(Data) != (Map *)NULL)
   {
      MapInit(GleisbildPageStructGetElementDb(Data), (CmpFkt)GleisbildPageIdCmp,
              (MapKeyDelCbFkt)NULL, (MapDataDelCbFkt)free);
   }
}

void GleisbildPageInsert(GleisbildPageStruct *Data,
                         GleisbildBildElement *Element)
{  int Id;
   GleisbildBildElement *OldElement;

   Id = GleisbildBildElementGetId(Element);
   OldElement = (GleisbildBildElement *)MapGet(GleisbildPageStructGetElementDb(Data),
                                               (MapKeyType)Id);
   if (OldElement != (GleisbildBildElement *)NULL)
   {
      GleisbildBildElementSetId(OldElement,
                                GleisbildBildElementGetId(Element));
      GleisbildBildElementSetTyp(OldElement,
                                 GleisbildBildElementGetTyp(Element));
      GleisbildBildElementSetDrehung(OldElement,
                                     GleisbildBildElementGetDrehung(Element));
      GleisbildBildElementSetArtikel(OldElement,
                                     GleisbildBildElementGetArtikel(Element));
      GleisbildBildElementSetText(OldElement,
                                  GleisbildBildElementGetText(Element));
      GleisbildBildElementSetZustand(OldElement,
                                     GleisbildBildElementGetZustand(Element));
      GleisbildBildElementSetDeviceId(OldElement,
                                      GleisbildBildElementGetDeviceId(Element));
   }
   else
   {
      OldElement = (GleisbildBildElement *)malloc(sizeof(GleisbildBildElement));
      if (OldElement != (GleisbildBildElement *)NULL)
      {
         GleisbildBildElementSetId(OldElement,
                                   GleisbildBildElementGetId(Element));
         GleisbildBildElementSetTyp(OldElement,
                                    GleisbildBildElementGetTyp(Element));
         GleisbildBildElementSetDrehung(OldElement,
                                        GleisbildBildElementGetDrehung(Element));
         GleisbildBildElementSetArtikel(OldElement,
                                        GleisbildBildElementGetArtikel(Element));
         GleisbildBildElementSetText(OldElement,
                                     GleisbildBildElementGetText(Element));
         GleisbildBildElementSetZustand(OldElement,
                                        GleisbildBildElementGetZustand(Element));
         GleisbildBildElementSetDeviceId(OldElement,
                                         GleisbildBildElementGetDeviceId(Element));
         MapSet(GleisbildPageStructGetElementDb(Data),
                (MapKeyType)Id, (MapDataType)OldElement);
      }
   }
}

void GleisbildPageDelete(GleisbildPageStruct *Data,
                         GleisbildBildElement *Element)
{  int Id;

   Id = GleisbildBildElementGetId(Element);
   MapDel(GleisbildPageStructGetElementDb(Data), (MapKeyType)Id);
}

GleisbildBildElement *GleisbildPageSearch(GleisbildPageStruct *Data, int Id)
{
   return((GleisbildBildElement *)MapGet(GleisbildPageStructGetElementDb(Data),
                                         (MapKeyType)Id));
}

BOOL GleisbildPageParseGleisbildPageCs2(GleisbildPageStruct *Data,
                                        char *Buf, int Len)
{  Cs2parser *GleisbildPageParser;
   int NumElements, LineInfo;
   GleisbildBildElement NewElement;

   GleisbildPageParser = Cs2pCreate();
   Cs2pInit(GleisbildPageParser, PARSER_TYPE_GLEISBILD_SEITE, Buf, Len);
   Cs2pSetVerbose(GleisbildPageParser, FALSE);
   NumElements = 0;
   GleisbildBildElementSetDrehung(&NewElement, -1);
   GleisbildBildElementSetText(&NewElement, "");
   GleisbildBildElementSetZustand(&NewElement, -1);
   GleisbildBildElementSetDeviceId(&NewElement, -1);
   do {
      LineInfo = Cs2pParse(GleisbildPageParser);
      switch (LineInfo)
      {
         case PARSER_ERROR:
            break;
         case PARSER_EOF:
            GleisbildPageInsert(Data, &NewElement);
            break;
         case PARSER_PARAGRAPH:
            switch (Cs2pGetSubType(GleisbildPageParser))
            {
               default:
                  break;
            }
            break;
         case PARSER_VALUE:
            switch (Cs2pGetSubType(GleisbildPageParser))
            {
               case PARSER_VALUE_ELEMENT:
                  if (NumElements != 0)
                  {
                     if (strcmp(GleisbildBildElementGetTyp(&NewElement),
                                CS2_GLEISBILD_TYP_STRING_LEER) == 0)
                        GleisbildPageDelete(Data, &NewElement);
                     else
                        GleisbildPageInsert(Data, &NewElement);
                  }
                  NumElements++;
                  GleisbildBildElementSetDrehung(&NewElement, -1);
                  GleisbildBildElementSetText(&NewElement, "");
                  GleisbildBildElementSetZustand(&NewElement, -1);
                  GleisbildBildElementSetDeviceId(&NewElement, -1);
                  break;
               case PARSER_VALUE_VERSION:
                  break;
               case PARSER_VALUE_MAJOR:
                  break;
               case PARSER_VALUE_MINOR:
                  break;
               case PARSER_VALUE_PAGE:
                  GleisbildPageSetPage(Data,
                                       strtoul(Cs2pGetValue(GleisbildPageParser),
                                               NULL, 0));
                  break;
               case PARSER_VALUE_ID:
                  GleisbildBildElementSetId(&NewElement,
                                            strtoul(Cs2pGetValue(GleisbildPageParser),
                                                    NULL, 0));
                  break;
               case PARSER_VALUE_TYP:
                  GleisbildBildElementSetTyp(&NewElement,
                                             Cs2pGetValue(GleisbildPageParser));
                  break;
               case PARSER_VALUE_DREHUNG:
                  GleisbildBildElementSetDrehung(&NewElement,
                                                 strtoul(Cs2pGetValue(GleisbildPageParser),
                                                         NULL, 0));
                  break;
               case PARSER_VALUE_ARTIKEL:
                  GleisbildBildElementSetArtikel(&NewElement,
                                                 strtoul(Cs2pGetValue(GleisbildPageParser),
                                                         NULL, 0));
                  break;
               case PARSER_VALUE_TEXT:
                  GleisbildBildElementSetText(&NewElement,
                                              Cs2pGetValue(GleisbildPageParser));
                  break;
               case PARSER_VALUE_ZUSTAND:
                  GleisbildBildElementSetZustand(&NewElement,
                                                 strtoul(Cs2pGetValue(GleisbildPageParser),
                                                         NULL, 0));
                  break;
               case PARSER_VALUE_DEVICE_ID:
                  GleisbildBildElementSetDeviceId(&NewElement,
                                                  strtoul(Cs2pGetValue(GleisbildPageParser),
                                                          NULL, 0));
                  break;
            }
            break;
      }
   } while ((LineInfo != PARSER_EOF) && (LineInfo != PARSER_ERROR));
   GleisbildPageSetNumElements(Data, NumElements);
   Cs2pExit(GleisbildPageParser);
   Cs2pDestroy(GleisbildPageParser);
   return(LineInfo == PARSER_EOF);
}

void GleisbildPageLoadGleisbildPageCs2(GleisbildPageStruct *Data)
{  FILE *GleisbildPageCs2Stream;
   struct stat attribut;
   char *GleisbildPageFileName, *GleisbildPageFileContent;

   if (GleisbildPageStructGetGleisbildPageFilePath(Data) != (char *)NULL)
   {
      GleisbildPageFileName = (char *)malloc(strlen(GleisbildPageStructGetGleisbildPageFilePath(Data)) + 
                                             strlen(CS2_GLEISBILD_PAGE_SUBDIR) +
                                             strlen(GleisbildPageStructGetName(Data)) +
                                             strlen(CS2_FILE_EXTENSION) + 2);
      if (GleisbildPageFileName != (char *)NULL)
      {
         strcpy(GleisbildPageFileName, GleisbildPageStructGetGleisbildPageFilePath(Data));
         if (GleisbildPageFileName[strlen(GleisbildPageFileName) - 1] != '/')
            strcat(GleisbildPageFileName, "/");
         strcat(GleisbildPageFileName, CS2_GLEISBILD_PAGE_SUBDIR);
         if (GleisbildPageFileName[strlen(GleisbildPageFileName) - 1] != '/')
            strcat(GleisbildPageFileName, "/");
         strcat(GleisbildPageFileName, GleisbildPageStructGetName(Data));
         strcat(GleisbildPageFileName, CS2_FILE_EXTENSION);
         if (stat(GleisbildPageFileName, &attribut) == 0)
         {
            GleisbildPageFileContent = (char *)malloc(attribut.st_size);
            if (GleisbildPageFileContent != (char *)NULL)
            {
               GleisbildPageCs2Stream = fopen(GleisbildPageFileName, "r");
               if (GleisbildPageCs2Stream != NULL)
               {
                  fread(GleisbildPageFileContent, 1, attribut.st_size,
                        GleisbildPageCs2Stream);
                  if (!GleisbildPageParseGleisbildPageCs2(Data,
                                                          GleisbildPageFileContent,
                                                          attribut.st_size))
                  {
                     GleisbildPageClear(Data);
                  }
                  Cs2Close(GleisbildPageCs2Stream);
               }
               free(GleisbildPageFileContent);
            }
         }
         free(GleisbildPageFileName);
      }
   }
}

static void WriteElementOfGleisbildPageCs2(void *PrivData,
                                           MapKeyType Key, MapDataType Daten)
{  GleisbildBildElement *Element;
   FILE *GleisbildPageCs2Stream;

   Element = (GleisbildBildElement *)Daten;
   GleisbildPageCs2Stream = (FILE *)PrivData;
   Cs2WriteTitleByName(GleisbildPageCs2Stream, "element", 0);
   Cs2WriteHexLongValueByName(GleisbildPageCs2Stream, "id",
                              (long)GleisbildBildElementGetId(Element), 1);
   Cs2WriteStringValueByName(GleisbildPageCs2Stream, "typ",
                             GleisbildBildElementGetTyp(Element), 1);
   if (GleisbildBildElementGetDrehung(Element) != -1)
      Cs2WriteIntValueByName(GleisbildPageCs2Stream, "drehung",
                             GleisbildBildElementGetDrehung(Element), 1);
   Cs2WriteIntValueByName(GleisbildPageCs2Stream, "artikel",
                          GleisbildBildElementGetArtikel(Element), 1);
   if (strlen(GleisbildBildElementGetText(Element)) != 0)
      Cs2WriteStringValueByName(GleisbildPageCs2Stream, "text",
                                GleisbildBildElementGetText(Element), 1);
   if (GleisbildBildElementGetZustand(Element) != -1)
      Cs2WriteIntValueByName(GleisbildPageCs2Stream, "zustand",
                             GleisbildBildElementGetZustand(Element), 1);
   if (GleisbildBildElementGetDeviceId(Element) != -1)
      Cs2WriteIntValueByName(GleisbildPageCs2Stream, "deviceId",
                             GleisbildBildElementGetDeviceId(Element), 1);
}

void GleisbildPageSaveGleisbildPageCs2(GleisbildPageStruct *Data)
{  FILE *GleisbildPageCs2Stream;
   char *GleisbildPageFile;

   if (GleisbildPageStructGetGleisbildPageFilePath(Data) != (char *)NULL)
   {
      GleisbildPageFile = (char *)malloc(strlen(GleisbildPageStructGetGleisbildPageFilePath(Data)) +
                                         strlen(CS2_GLEISBILD_PAGE_SUBDIR) +
                                         strlen(GleisbildPageStructGetName(Data)) + 2);
      if (GleisbildPageFile != (char *)NULL)
      {
         strcpy(GleisbildPageFile, GleisbildPageStructGetGleisbildPageFilePath(Data));
         if (GleisbildPageFile[strlen(GleisbildPageFile) - 1] != '/')
            strcat(GleisbildPageFile, "/");
         strcat(GleisbildPageFile, CS2_GLEISBILD_PAGE_SUBDIR);
         if (GleisbildPageFile[strlen(GleisbildPageFile) - 1] != '/')
            strcat(GleisbildPageFile, "/");
         strcat(GleisbildPageFile, GleisbildPageStructGetName(Data));
         GleisbildPageCs2Stream = Cs2OpenByName(GleisbildPageFile);
         if (GleisbildPageCs2Stream != NULL)
         {
            fchmod(fileno(GleisbildPageCs2Stream),
                   S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
            Cs2WriteParagraphByType(GleisbildPageCs2Stream,
                                    CS2_PARAGRAPH_TYPE_GLEISBILDSEITE);
            Cs2WriteTitleByName(GleisbildPageCs2Stream, "version", 0);
            Cs2WriteIntValueByName(GleisbildPageCs2Stream, "major", 0, 1);
            Cs2WriteIntValueByName(GleisbildPageCs2Stream, "minor", 1, 1);
            MapWalkAscend(GleisbildPageStructGetElementDb(Data),
                          (MapWalkCbFkt)WriteElementOfGleisbildPageCs2,
                          (void *)GleisbildPageCs2Stream);
            Cs2Close(GleisbildPageCs2Stream);
         }
         free(GleisbildPageFile);
      }
   }
}

char *GleisbildPageGetName(GleisbildPageStruct *Data)
{
   strcpy(PageName, CS2_GLEISBILD_PAGE_SUBDIR);
   if (PageName[strlen(PageName) - 1] != '/')
      strcat(PageName, "/");
   strcat(PageName, GleisbildPageStructGetName(Data));
   strcat(PageName, CS2_FILE_EXTENSION);
   return(PageName);
}
