#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <boolean.h>
#include <map.h>
#include <cs2parse.h>
#include <write_cs2.h>
#include "fahrstrasse.h"
#include <stdio.h>

FahrstrasseStruct *FahrstrasseCreate(void)
{  FahrstrasseStruct *NewData;

   NewData = (FahrstrasseStruct *)malloc(sizeof(FahrstrasseStruct));
   if (NewData != (FahrstrasseStruct *)NULL)
   {
      FahrstrasseSetFahrstrasseFilePath(NewData, "/var/www/config/");
      FahrstrasseSetNumPages(NewData, 0);
      FahrstrasseSetFahrstrasseDb(NewData, MapCreate());
      if (FahrstrasseGetFahrstrasseDb(NewData) == (Map *)NULL)
      {
         free(NewData);
         NewData = (FahrstrasseStruct *)NULL;
      }
   }
   return(NewData);
}

void FahrstrasseDestroy(FahrstrasseStruct *Data)
{
   if (FahrstrasseGetFahrstrasseDb(Data) != (Map *)NULL)
      MapDestroy(FahrstrasseGetFahrstrasseDb(Data));
   free(Data);
}

static int FahrstrasseIdCmp(void *d1, void *d2)
{
   return((int)d1 - (int)d2);
}

static void FahrstrassenItemDelCb(MapDataType Daten)
{  FahrstrasseInfo *Fahrstrasse;

   Fahrstrasse = (FahrstrasseInfo *)Daten;
   if (FahrstrasseInfoGetItemDb(Fahrstrasse) != (Map *)NULL)
      MapDestroy(FahrstrasseInfoGetItemDb(Fahrstrasse));
   free(Fahrstrasse);
}

void FahrstrasseInit(FahrstrasseStruct *Data, char *FahrstrassePath)
{
   FahrstrasseSetFahrstrasseFilePath(Data, FahrstrassePath);
   FahrstrasseSetNumPages(Data, 0);
   MapInit(FahrstrasseGetFahrstrasseDb(Data), (CmpFkt)FahrstrasseIdCmp,
           (MapKeyDelCbFkt)NULL, (MapDataDelCbFkt)FahrstrassenItemDelCb);
}

void FahrstrasseExit(FahrstrasseStruct *Data)
{
}

void FahrstrasseClear(FahrstrasseStruct *Data)
{
   if (FahrstrasseGetFahrstrasseDb(Data) != (Map *)NULL)
      MapDestroy(FahrstrasseGetFahrstrasseDb(Data));
   FahrstrasseSetNumPages(Data, 0);
   FahrstrasseSetFahrstrasseDb(Data, MapCreate());
   if (FahrstrasseGetFahrstrasseDb(Data) != (Map *)NULL)
   {
      MapInit(FahrstrasseGetFahrstrasseDb(Data), (CmpFkt)FahrstrasseIdCmp,
              (MapKeyDelCbFkt)NULL, (MapDataDelCbFkt)FahrstrassenItemDelCb);
   }
}

void FahrstrasseInsert(FahrstrasseStruct *Data, FahrstrasseInfo *Fahrstrasse)
{  int Id;
   FahrstrasseInfo *OldFahrstrasse;

   Id = FahrstrasseInfoGetId(Fahrstrasse);
   OldFahrstrasse = (FahrstrasseInfo *)MapGet(FahrstrasseGetFahrstrasseDb(Data),
                                              (MapKeyType)Id);
   if (OldFahrstrasse != (FahrstrasseInfo *)NULL)
   {
      MapDel(FahrstrasseGetFahrstrasseDb(Data), (MapKeyType)Id);
   }
   OldFahrstrasse = (FahrstrasseInfo *)malloc(sizeof(FahrstrasseInfo));
   if (OldFahrstrasse != (FahrstrasseInfo *)NULL)
   {
      FahrstrasseInfoSetId(OldFahrstrasse,
                           FahrstrasseInfoGetId(Fahrstrasse));
      FahrstrasseInfoSetName(OldFahrstrasse,
                             FahrstrasseInfoGetName(Fahrstrasse));
      FahrstrasseInfoSetOn(OldFahrstrasse,
                           FahrstrasseInfoGetOn(Fahrstrasse));
      FahrstrasseInfoSetS88(OldFahrstrasse,
                            FahrstrasseInfoGetS88(Fahrstrasse));
      FahrstrasseInfoSetS88Ein(OldFahrstrasse,
                               FahrstrasseInfoGetS88Ein(Fahrstrasse));
      FahrstrasseInfoSetExtern(OldFahrstrasse,
                               FahrstrasseInfoGetExtern(Fahrstrasse));
      FahrstrasseInfoSetItemDb(OldFahrstrasse,
                               FahrstrasseInfoGetItemDb(Fahrstrasse));
      MapSet(FahrstrasseGetFahrstrasseDb(Data),
             (MapKeyType)Id, (MapDataType)OldFahrstrasse);
   }
}

void FahrstrasseDelete(FahrstrasseStruct *Data, FahrstrasseInfo *Fahrstrasse)
{  int Id;

   Id = FahrstrasseInfoGetId(Fahrstrasse);
   MapDel(FahrstrasseGetFahrstrasseDb(Data), (MapKeyType)Id);
}

FahrstrasseInfo *FahrstrasseSearch(FahrstrasseStruct *Data, int Id)
{
   return((FahrstrasseInfo *)MapGet(FahrstrasseGetFahrstrasseDb(Data),
                                    (MapKeyType)Id));
}

static int FahrstrasseItemNrCmp(void *d1, void *d2)
{
   return((int)d1 - (int)d2);
}

void FahrstrasseItemInsert(FahrstrasseInfo *Data, int Nr,
                            FahrstrasseItem *Item)
{  FahrstrasseItem *OldItem;

   OldItem = (FahrstrasseItem *)MapGet(FahrstrasseInfoGetItemDb(Data),
                                       (MapKeyType)Nr);
   if (OldItem != (FahrstrasseItem *)NULL)
   {
      FahrstrasseItemSetFsVerweis(OldItem,
                                  FahrstrasseItemGetFsVerweis(Item));
      FahrstrasseItemSetTyp(OldItem,
                            FahrstrasseItemGetTyp(Item));
      FahrstrasseItemSetLok(OldItem,
                            FahrstrasseItemGetLok(Item));
      FahrstrasseItemSetMagnetartikel(OldItem,
                                      FahrstrasseItemGetMagnetartikel(Item));
      FahrstrasseItemSetStellung(OldItem,
                                 FahrstrasseItemGetStellung(Item));
      FahrstrasseItemSetSekunde(OldItem,
                                FahrstrasseItemGetSekunde(Item));
      FahrstrasseItemSetIdx(OldItem,
                            FahrstrasseItemGetIdx(Item));
      FahrstrasseItemSetWert(OldItem,
                             FahrstrasseItemGetWert(Item));
   }
   else
   {
      OldItem = (FahrstrasseItem *)malloc(sizeof(FahrstrasseItem));
      if (OldItem != (FahrstrasseItem *)NULL)
      {
         FahrstrasseItemSetFsVerweis(OldItem,
                                     FahrstrasseItemGetFsVerweis(Item));
         FahrstrasseItemSetTyp(OldItem,
                               FahrstrasseItemGetTyp(Item));
         FahrstrasseItemSetLok(OldItem,
                               FahrstrasseItemGetLok(Item));
         FahrstrasseItemSetMagnetartikel(OldItem,
                                         FahrstrasseItemGetMagnetartikel(Item));
         FahrstrasseItemSetStellung(OldItem,
                                    FahrstrasseItemGetStellung(Item));
         FahrstrasseItemSetSekunde(OldItem,
                                   FahrstrasseItemGetSekunde(Item));
         FahrstrasseItemSetIdx(OldItem,
                               FahrstrasseItemGetIdx(Item));
         FahrstrasseItemSetWert(OldItem,
                                FahrstrasseItemGetWert(Item));
         MapSet(FahrstrasseInfoGetItemDb(Data),
                (MapKeyType)Nr, (MapDataType)OldItem);
      }
   }
}

void FahrstrasseDeleteItem(FahrstrasseInfo *Data, int Nr)
{
   MapDel(FahrstrasseInfoGetItemDb(Data), (MapKeyType)Nr);
}

FahrstrasseItem *FahrstrasseSearchItem(FahrstrasseInfo *Data, int Nr)
{
   return((FahrstrasseItem *)MapGet(FahrstrasseInfoGetItemDb(Data),
                                    (MapKeyType)Nr));
}

BOOL FahrstrasseParseFahrstrasseCs2(FahrstrasseStruct *Data, char *Buf, int Len)
{  Cs2parser *FahrstrasseParser;
   int NumFahrstrassen, NumItem, LineInfo;
   FahrstrasseInfo NewFahrstrasse;
   FahrstrasseItem NewFahrstrasseItem;

   FahrstrasseParser = Cs2pCreate();
   Cs2pInit(FahrstrasseParser, PARSER_TYPE_FAHRSTRASSEN_CS2, Buf, Len);
   Cs2pSetVerbose(FahrstrasseParser, FALSE);
   NumFahrstrassen = 0;
   NumItem = 0;
   FahrstrasseInfoSetOn(&NewFahrstrasse, 0);
   FahrstrasseInfoSetS88(&NewFahrstrasse, -1);
   FahrstrasseInfoSetS88Ein(&NewFahrstrasse, -1);
   FahrstrasseInfoSetExtern(&NewFahrstrasse, -1);
   FahrstrasseInfoSetItemDb(&NewFahrstrasse, MapCreate());
   if (FahrstrasseInfoGetItemDb(&NewFahrstrasse) != (Map *)NULL)
   {
      MapInit(FahrstrasseInfoGetItemDb(&NewFahrstrasse),
              (CmpFkt)FahrstrasseItemNrCmp,
              (MapKeyDelCbFkt)NULL, (MapDataDelCbFkt)free);
   }
   do {
      LineInfo = Cs2pParse(FahrstrasseParser);
      switch (LineInfo)
      {
         case PARSER_ERROR:
            break;
         case PARSER_EOF:
            if (FahrstrasseInfoGetItemDb(&NewFahrstrasse) != (Map *)NULL)
            {
               if (NumItem != 0)
                  FahrstrasseItemInsert(&NewFahrstrasse, NumItem,
                                        &NewFahrstrasseItem);
            }
            if (NumItem != 0)
               FahrstrasseInsert(Data, &NewFahrstrasse);
            else
               FahrstrasseDelete(Data, &NewFahrstrasse);
            break;
         case PARSER_PARAGRAPH:
            switch (Cs2pGetSubType(FahrstrasseParser))
            {
               default:
                  break;
            }
            break;
         case PARSER_VALUE:
            switch (Cs2pGetSubType(FahrstrasseParser))
            {
               case PARSER_VALUE_FAHRSTRASSE:
                  if (NumFahrstrassen != 0)
                  {
                     if (NumItem != 0)
                     {
                        FahrstrasseInsert(Data, &NewFahrstrasse);
                        NumItem = 0;
                        FahrstrasseInfoSetOn(&NewFahrstrasse, 0);
                        FahrstrasseInfoSetS88(&NewFahrstrasse, -1);
                        FahrstrasseInfoSetS88Ein(&NewFahrstrasse, -1);
                        FahrstrasseInfoSetExtern(&NewFahrstrasse, -1);
                        FahrstrasseInfoSetItemDb(&NewFahrstrasse, MapCreate());
                        if (FahrstrasseInfoGetItemDb(&NewFahrstrasse) != (Map *)NULL)
                        {
                           MapInit(FahrstrasseInfoGetItemDb(&NewFahrstrasse),
                                   (CmpFkt)FahrstrasseItemNrCmp,
                                   (MapKeyDelCbFkt)NULL, (MapDataDelCbFkt)free);
                        }
                     }
                     else
                     {
                        FahrstrasseDelete(Data, &NewFahrstrasse);
                        FahrstrasseInfoSetS88(&NewFahrstrasse, -1);
                        FahrstrasseInfoSetS88Ein(&NewFahrstrasse, -1);
                        FahrstrasseInfoSetExtern(&NewFahrstrasse, -1);
                        MapPurge(FahrstrasseInfoGetItemDb(&NewFahrstrasse));
                     }
                  }
                  NumFahrstrassen++;
                  break;
               case PARSER_VALUE_VERSION:
                  break;
               case PARSER_VALUE_MAJOR:
                  break;
               case PARSER_VALUE_MINOR:
                  break;
               case PARSER_VALUE_ID:
                  FahrstrasseInfoSetId(&NewFahrstrasse,
                                       strtoul(Cs2pGetValue(FahrstrasseParser),
                                               NULL, 0));
                  break;
               case PARSER_VALUE_NAME:
                  FahrstrasseInfoSetName(&NewFahrstrasse,
                                         Cs2pGetValue(FahrstrasseParser));
                  break;
               case PARSER_VALUE_S88:
                  FahrstrasseInfoSetS88(&NewFahrstrasse,
                                        strtoul(Cs2pGetValue(FahrstrasseParser),
                                                NULL, 0));
                  break;
               case PARSER_VALUE_S88EIN:
                  FahrstrasseInfoSetS88Ein(&NewFahrstrasse,
                                           strtoul(Cs2pGetValue(FahrstrasseParser),
                                                   NULL, 0));
                  break;
               case PARSER_VALUE_EXTERN:
                  FahrstrasseInfoSetExtern(&NewFahrstrasse,
                                           strtoul(Cs2pGetValue(FahrstrasseParser),
                                                   NULL, 0));
                  break;
               case PARSER_VALUE_ITEM:
                  if (FahrstrasseInfoGetItemDb(&NewFahrstrasse) != (Map *)NULL)
                  {
                     if (NumItem != 0)
                        FahrstrasseItemInsert(&NewFahrstrasse, NumItem,
                                              &NewFahrstrasseItem);
                     NumItem++;
                     FahrstrasseItemSetFsVerweis(&NewFahrstrasseItem, -1);
                     FahrstrasseItemSetTyp(&NewFahrstrasseItem, "");
                     FahrstrasseItemSetLok(&NewFahrstrasseItem, "");
                     FahrstrasseItemSetMagnetartikel(&NewFahrstrasseItem, -1);
                     FahrstrasseItemSetStellung(&NewFahrstrasseItem, -1);
                     FahrstrasseItemSetSekunde(&NewFahrstrasseItem, 0.0);
                     FahrstrasseItemSetIdx(&NewFahrstrasseItem, -1);
                     FahrstrasseItemSetWert(&NewFahrstrasseItem, -1);
                  }
                  break;
               case PARSER_VALUE_FSVERWEIS:
                  FahrstrasseItemSetFsVerweis(&NewFahrstrasseItem,
                                              strtoul(Cs2pGetValue(FahrstrasseParser),
                                                      NULL, 0));
                  break;
               case PARSER_VALUE_TYP:
                  FahrstrasseItemSetTyp(&NewFahrstrasseItem,
                                        Cs2pGetValue(FahrstrasseParser));
                  break;
               case PARSER_VALUE_MAGNETARTIKEL:
                  FahrstrasseItemSetMagnetartikel(&NewFahrstrasseItem,
                                                  strtoul(Cs2pGetValue(FahrstrasseParser),
                                                          NULL, 0));
                  break;
               case PARSER_VALUE_STELLUNG:
                  FahrstrasseItemSetStellung(&NewFahrstrasseItem,
                                             strtoul(Cs2pGetValue(FahrstrasseParser),
                                                     NULL, 0));
                  break;
               case PARSER_VALUE_LOK:
                  FahrstrasseItemSetLok(&NewFahrstrasseItem,
                                        Cs2pGetValue(FahrstrasseParser));
                  break;
               case PARSER_VALUE_SEKUNDE:
                  FahrstrasseItemSetSekunde(&NewFahrstrasseItem,
                                            atof(Cs2pGetValue(FahrstrasseParser)));
                  break;
               case PARSER_VALUE_IDX:
                  FahrstrasseItemSetIdx(&NewFahrstrasseItem,
                                        strtoul(Cs2pGetValue(FahrstrasseParser),
                                                NULL, 0));
                  break;
               case PARSER_VALUE_WERT:
                  FahrstrasseItemSetWert(&NewFahrstrasseItem,
                                         strtoul(Cs2pGetValue(FahrstrasseParser),
                                                 NULL, 0));
                  break;
            }
            break;
      }
   } while ((LineInfo != PARSER_EOF) && (LineInfo != PARSER_ERROR));
   FahrstrasseSetNumPages(Data, NumFahrstrassen);
   Cs2pExit(FahrstrasseParser);
   Cs2pDestroy(FahrstrasseParser);
   return(LineInfo == PARSER_EOF);
}

void FahrstrasseLoadFahrstrasseCs2(FahrstrasseStruct *Data)
{  FILE *FahrstrasseCs2Stream;
   struct stat attribut;
   char *FahrstrasseFileName, *FahrstrasseFileContent;

   if (FahrstrasseGetFahrstrasseFilePath(Data) != (char *)NULL)
   {
      FahrstrasseFileName = (char *)malloc(strlen(FahrstrasseGetFahrstrasseFilePath(Data)) + 
                                           strlen(CS2_FILE_STRING_FAHRSTRASSE) + 2);
      if (FahrstrasseFileName != (char *)NULL)
      {
         strcpy(FahrstrasseFileName, FahrstrasseGetFahrstrasseFilePath(Data));
         if (FahrstrasseFileName[strlen(FahrstrasseFileName) - 1] != '/')
            strcat(FahrstrasseFileName, "/");
         strcat(FahrstrasseFileName, CS2_FILE_STRING_FAHRSTRASSE);
         if (stat(FahrstrasseFileName, &attribut) == 0)
         {
            FahrstrasseFileContent = (char *)malloc(attribut.st_size);
            if (FahrstrasseFileContent != (char *)NULL)
            {
               FahrstrasseCs2Stream = fopen(FahrstrasseFileName, "r");
               if (FahrstrasseCs2Stream != NULL)
               {
                  fread(FahrstrasseFileContent, 1, attribut.st_size,
                        FahrstrasseCs2Stream);
                  if (!FahrstrasseParseFahrstrasseCs2(Data, FahrstrasseFileContent,
                                                      attribut.st_size))
                  {
                     FahrstrasseClear(Data);
                  }
                  Cs2Close(FahrstrasseCs2Stream);
               }
               free(FahrstrasseFileContent);
            }
         }
         free(FahrstrasseFileName);
      }
   }
}

static void WriteItemOfFahrstrasseCs2(void *PrivData,
                                      MapKeyType Key, MapDataType Daten)
{  FahrstrasseItem *Item;
   FILE *FahrstrasseCs2Stream;

   Item = (FahrstrasseItem *)Daten;
   FahrstrasseCs2Stream = (FILE *)PrivData;
   Cs2WriteTitleByName(FahrstrasseCs2Stream, "item", 1);
   if (FahrstrasseItemGetFsVerweis(Item) != -1)
      Cs2WriteIntValueByName(FahrstrasseCs2Stream, "fsverweis",
                             FahrstrasseItemGetFsVerweis(Item), 2);
   if (strcmp(FahrstrasseItemGetTyp(Item), "") != 0)
      Cs2WriteStringValueByName(FahrstrasseCs2Stream, "typ",
                                FahrstrasseItemGetTyp(Item), 2);
   if (FahrstrasseItemGetMagnetartikel(Item) != -1)
      Cs2WriteIntValueByName(FahrstrasseCs2Stream, "magnetartikel",
                             FahrstrasseItemGetMagnetartikel(Item), 2);
   if (FahrstrasseItemGetStellung(Item) != -1)
      Cs2WriteIntValueByName(FahrstrasseCs2Stream, "stellung",
                             FahrstrasseItemGetStellung(Item), 2);
   if (FahrstrasseItemGetSekunde(Item) != 0.0)
      Cs2WriteFloatValueByName(FahrstrasseCs2Stream, "sekunde",
                               FahrstrasseItemGetSekunde(Item), 2);
   if (FahrstrasseItemGetIdx(Item) != -1)
      Cs2WriteIntValueByName(FahrstrasseCs2Stream, "idx",
                             FahrstrasseItemGetIdx(Item), 2);
   if (FahrstrasseItemGetWert(Item) != -1)
      Cs2WriteIntValueByName(FahrstrasseCs2Stream, "wert",
                             FahrstrasseItemGetWert(Item), 2);
}

static void WriteFahrstrasseOfFahrstrasseCs2(void *PrivData,
                                             MapKeyType Key, MapDataType Daten)
{  FahrstrasseInfo *FahrstrassePage;
   FILE *FahrstrasseCs2Stream;

   FahrstrassePage = (FahrstrasseInfo *)Daten;
   FahrstrasseCs2Stream = (FILE *)PrivData;
   Cs2WriteTitleByName(FahrstrasseCs2Stream, "fahrstrasse", 0);
   Cs2WriteIntValueByName(FahrstrasseCs2Stream, "id",
                          FahrstrasseInfoGetId(FahrstrassePage), 1);
   Cs2WriteStringValueByName(FahrstrasseCs2Stream, "name",
                             FahrstrasseInfoGetName(FahrstrassePage), 1);
   if (FahrstrasseInfoGetS88(FahrstrassePage) != -1)
      Cs2WriteIntValueByName(FahrstrasseCs2Stream, "s88",
                             FahrstrasseInfoGetS88(FahrstrassePage), 1);
   if (FahrstrasseInfoGetS88Ein(FahrstrassePage) != -1)
      Cs2WriteIntValueByName(FahrstrasseCs2Stream, "s88Ein",
                             FahrstrasseInfoGetS88Ein(FahrstrassePage), 1);
   if (FahrstrasseInfoGetExtern(FahrstrassePage) != -1)
      Cs2WriteIntValueByName(FahrstrasseCs2Stream, "extern",
                             FahrstrasseInfoGetExtern(FahrstrassePage), 1);
   MapWalkAscend(FahrstrasseInfoGetItemDb(FahrstrassePage),
                 (MapWalkCbFkt)WriteItemOfFahrstrasseCs2,
                 (void *)FahrstrasseCs2Stream);
}

void FahrstrasseSaveFahrstrasseCs2(FahrstrasseStruct *Data)
{  FILE *FahrstrasseCs2Stream;
   char *FahrstrasseFile;

   if (FahrstrasseGetFahrstrasseFilePath(Data) != (char *)NULL)
   {
      FahrstrasseFile = (char *)malloc(strlen(FahrstrasseGetFahrstrasseFilePath(Data)) +
                                       strlen(CS2_FILE_STRING_FAHRSTRASSE) + 2);
      if (FahrstrasseFile != (char *)NULL)
      {
         strcpy(FahrstrasseFile, FahrstrasseGetFahrstrasseFilePath(Data));
         if (FahrstrasseFile[strlen(FahrstrasseFile) - 1] != '/')
            strcat(FahrstrasseFile, "/");
         strcat(FahrstrasseFile, CS2_FILE_STRING_FAHRSTRASSE);
         FahrstrasseCs2Stream = Cs2OpenByName(FahrstrasseFile);
         if (FahrstrasseCs2Stream != NULL)
         {
            fchmod(fileno(FahrstrasseCs2Stream),
                   S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
            Cs2WriteParagraphByType(FahrstrasseCs2Stream,
                                    CS2_PARAGRAPH_TYPE_FAHRSTRASSEN);
            Cs2WriteTitleByName(FahrstrasseCs2Stream, "version", 0);
            Cs2WriteIntValueByName(FahrstrasseCs2Stream, "major", 0, 1);
            Cs2WriteIntValueByName(FahrstrasseCs2Stream, "minor", 1, 1);
            MapWalkAscend(FahrstrasseGetFahrstrasseDb(Data),
                          (MapWalkCbFkt)WriteFahrstrasseOfFahrstrasseCs2,
                          (void *)FahrstrasseCs2Stream);
            Cs2Close(FahrstrasseCs2Stream);
         }
         free(FahrstrasseFile);
      }
   }
}
