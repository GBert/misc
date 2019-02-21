#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <boolean.h>
#include <map.h>
#include <cs2parse.h>
#include <write_cs2.h>
#include "gleisbild.h"
#include <stdio.h>

GleisbildStruct *GleisbildCreate(void)
{  GleisbildStruct *NewData;

   NewData = (GleisbildStruct *)malloc(sizeof(GleisbildStruct));
   if (NewData != (GleisbildStruct *)NULL)
   {
      GleisbildSetGleisbildFilePath(NewData, "/var/www/config/");
      GleisbildSetNumPages(NewData, 0);
      GleisbildSetGleisbildDb(NewData, MapCreate());
      if (GleisbildGetGleisbildDb(NewData) == (Map *)NULL)
      {
         free(NewData);
         NewData = (GleisbildStruct *)NULL;
      }
   }
   return(NewData);
}

void GleisbildDestroy(GleisbildStruct *Data)
{
   if (GleisbildGetGleisbildDb(Data) != (Map *)NULL)
      MapDestroy(GleisbildGetGleisbildDb(Data));
   free(Data);
}

static int GleisbildIdCmp(void *d1, void *d2)
{
   return((int)d1 - (int)d2);
}

void GleisbildInit(GleisbildStruct *Data, char *GleisbildPath)
{
   GleisbildSetGleisbildFilePath(Data, GleisbildPath);
   GleisbildSetNumPages(Data, 0);
   MapInit(GleisbildGetGleisbildDb(Data), (CmpFkt)GleisbildIdCmp,
           (MapKeyDelCbFkt)NULL, (MapDataDelCbFkt)free);
}

void GleisbildExit(GleisbildStruct *Data)
{
}

void GleisbildClear(GleisbildStruct *Data)
{
   if (GleisbildGetGleisbildDb(Data) != (Map *)NULL)
      MapDestroy(GleisbildGetGleisbildDb(Data));
   GleisbildSetNumPages(Data, 0);
   GleisbildSetGleisbildDb(Data, MapCreate());
   if (GleisbildGetGleisbildDb(Data) != (Map *)NULL)
   {
      MapInit(GleisbildGetGleisbildDb(Data), (CmpFkt)GleisbildIdCmp,
              (MapKeyDelCbFkt)NULL, (MapDataDelCbFkt)free);
   }
}

void GleisbildInsert(GleisbildStruct *Data, GleisbildInfo *Gleisbild)
{  int Id;
   GleisbildInfo *OldGleisbild;

   Id = GleisbildInfoGetId(Gleisbild);
   OldGleisbild = (GleisbildInfo *)MapGet(GleisbildGetGleisbildDb(Data),
                                          (MapKeyType)Id);
   if (OldGleisbild != (GleisbildInfo *)NULL)
   {
      GleisbildInfoSetId(OldGleisbild, GleisbildInfoGetId(Gleisbild));
      GleisbildInfoSetName(OldGleisbild, GleisbildInfoGetName(Gleisbild));
      GleisbildInfoSetZustand(OldGleisbild, GleisbildInfoGetZustand(Gleisbild));
   }
   else
   {
      OldGleisbild = (GleisbildInfo *)malloc(sizeof(GleisbildInfo));
      if (OldGleisbild != (GleisbildInfo *)NULL)
      {
         GleisbildInfoSetId(OldGleisbild, GleisbildInfoGetId(Gleisbild));
         GleisbildInfoSetName(OldGleisbild, GleisbildInfoGetName(Gleisbild));
         GleisbildInfoSetZustand(OldGleisbild,
	                         GleisbildInfoGetZustand(Gleisbild));
         MapSet(GleisbildGetGleisbildDb(Data),
                (MapKeyType)Id, (MapDataType)OldGleisbild);
      }
   }
}

void GleisbildDelete(GleisbildStruct *Data, GleisbildInfo *Gleisbild)
{  int Id;

   Id = GleisbildInfoGetId(Gleisbild);
   MapDel(GleisbildGetGleisbildDb(Data), (MapKeyType)Id);
}

GleisbildInfo *GleisbildSearch(GleisbildStruct *Data, int Id)
{
   return((GleisbildInfo *)MapGet(GleisbildGetGleisbildDb(Data),
                                  (MapKeyType)Id));
}

BOOL GleisbildParseGleisbildCs2(GleisbildStruct *Data, char *Buf, int Len)
{  Cs2parser *GleisbildParser;
   int NumPages, LineInfo;
   GleisbildInfo NewGleisbild;

   GleisbildParser = Cs2pCreate();
   Cs2pInit(GleisbildParser, PARSER_TYPE_GLEISBILD_CS2, Buf, Len);
   Cs2pSetVerbose(GleisbildParser, FALSE);
   NumPages = 0;
   do {
      LineInfo = Cs2pParse(GleisbildParser);
      switch (LineInfo)
      {
         case PARSER_ERROR:
            break;
         case PARSER_EOF:
            GleisbildInsert(Data, &NewGleisbild);
            break;
         case PARSER_PARAGRAPH:
            switch (Cs2pGetSubType(GleisbildParser))
            {
               default:
                  break;
            }
            break;
         case PARSER_VALUE:
            switch (Cs2pGetSubType(GleisbildParser))
            {
               case PARSER_VALUE_SEITE:
                  if (NumPages != 0)
                     GleisbildInsert(Data, &NewGleisbild);
                  NumPages++;
                  GleisbildInfoSetId(&NewGleisbild, 0);
                  GleisbildInfoSetZustand(&NewGleisbild, 0);
                  break;
               case PARSER_VALUE_VERSION:
                  break;
               case PARSER_VALUE_NAME:
                  GleisbildInfoSetName(&NewGleisbild,
                                       Cs2pGetValue(GleisbildParser));
                  break;
               case PARSER_VALUE_MAJOR:
                  break;
               case PARSER_VALUE_MINOR:
                  break;
               case PARSER_VALUE_GROESSE:
                  break;
               case PARSER_VALUE_ZULETZT_BENUTZT:
                  break;
               case PARSER_VALUE_ID:
                  GleisbildInfoSetId(&NewGleisbild,
                                     strtoul(Cs2pGetValue(GleisbildParser),
                                             NULL, 0));
                  break;
               case PARSER_VALUE_ZUSTAND:
                  GleisbildInfoSetId(&NewGleisbild,
                                     strtoul(Cs2pGetValue(GleisbildParser),
                                             NULL, 0));
                  break;
            }
            break;
      }
   } while ((LineInfo != PARSER_EOF) && (LineInfo != PARSER_ERROR));
   GleisbildSetNumPages(Data, NumPages);
   Cs2pExit(GleisbildParser);
   Cs2pDestroy(GleisbildParser);
   return(LineInfo == PARSER_EOF);
}

void GleisbildLoadGleisbildCs2(GleisbildStruct *Data)
{  FILE *GleisbildCs2Stream;
   struct stat attribut;
   char *GleisbildFileName, *GleisbildFileContent;

   if (GleisbildGetGleisbildFilePath(Data) != (char *)NULL)
   {
      GleisbildFileName = (char *)malloc(strlen(GleisbildGetGleisbildFilePath(Data)) + 
                                         strlen(CS2_FILE_STRING_GLEISBILD) + 2);
      if (GleisbildFileName != (char *)NULL)
      {
         strcpy(GleisbildFileName, GleisbildGetGleisbildFilePath(Data));
         if (GleisbildFileName[strlen(GleisbildFileName) - 1] != '/')
            strcat(GleisbildFileName, "/");
         if (strcat(GleisbildFileName, CS2_FILE_STRING_GLEISBILD) == 0)
         {
            stat(GleisbildFileName, &attribut);
            GleisbildFileContent = (char *)malloc(attribut.st_size);
            if (GleisbildFileContent != (char *)NULL)
            {
               GleisbildCs2Stream = fopen(GleisbildFileName, "r");
               if (GleisbildCs2Stream != NULL)
               {
                  fread(GleisbildFileContent, 1, attribut.st_size,
                        GleisbildCs2Stream);
                  if (!GleisbildParseGleisbildCs2(Data, GleisbildFileContent,
                                                  attribut.st_size))
                  {
                     GleisbildClear(Data);
                  }
                  Cs2Close(GleisbildCs2Stream);
               }
               free(GleisbildFileContent);
            }
         }
         free(GleisbildFileName);
      }
   }
}

static void WriteGleisbildOfGleisbildCs2(void *PrivData,
                                         MapKeyType Key, MapDataType Daten)
{  GleisbildInfo *GleisbildPage;
   FILE *GleisbildCs2Stream;

   GleisbildPage = (GleisbildInfo *)Daten;
   GleisbildCs2Stream = (FILE *)PrivData;
   Cs2WriteTitleByName(GleisbildCs2Stream, "seite", 0);
   if (GleisbildInfoGetId(GleisbildPage) != 0)
   {
      Cs2WriteIntValueByName(GleisbildCs2Stream, "id",
                             GleisbildInfoGetId(GleisbildPage), 1);
   }
   Cs2WriteStringValueByName(GleisbildCs2Stream, "name",
                             GleisbildInfoGetName(GleisbildPage), 1);
}

void GleisbildSaveGleisbildCs2(GleisbildStruct *Data)
{  FILE *GleisbildCs2Stream;
   char *GleisbildFile;
   GleisbildInfo *FirstPage;

   if (GleisbildGetGleisbildFilePath(Data) != (char *)NULL)
   {
      GleisbildFile = (char *)malloc(strlen(GleisbildGetGleisbildFilePath(Data)) +
                                     strlen(CS2_FILE_STRING_GLEISBILD) + 2);
      if (GleisbildFile != (char *)NULL)
      {
         strcpy(GleisbildFile, GleisbildGetGleisbildFilePath(Data));
         if (GleisbildFile[strlen(GleisbildFile) - 1] != '/')
            strcat(GleisbildFile, "/");
         strcat(GleisbildFile, CS2_FILE_STRING_GLEISBILD);
         GleisbildCs2Stream = Cs2OpenByName(GleisbildFile);
         if (GleisbildCs2Stream != NULL)
         {
            fchmod(fileno(GleisbildCs2Stream),
                   S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
            Cs2WriteParagraphByType(GleisbildCs2Stream,
                                    CS2_PARAGRAPH_TYPE_GLEISBILD);
            Cs2WriteTitleByName(GleisbildCs2Stream, "version", 0);
            Cs2WriteIntValueByName(GleisbildCs2Stream, "major", 0, 1);
            Cs2WriteIntValueByName(GleisbildCs2Stream, "minor", 1, 1);
            Cs2WriteTitleByName(GleisbildCs2Stream, "groesse", 0);
            FirstPage = GleisbildSearch(Data, 0);
            if (FirstPage != (GleisbildInfo *)NULL)
            {
               Cs2WriteTitleByName(GleisbildCs2Stream, "zuletztBenutzt", 0);
               Cs2WriteStringValueByName(GleisbildCs2Stream, "name",
                                         GleisbildInfoGetName(FirstPage), 1);
            }
            MapWalkAscend(GleisbildGetGleisbildDb(Data),
                          (MapWalkCbFkt)WriteGleisbildOfGleisbildCs2,
                          (void *)GleisbildCs2Stream);
            Cs2Close(GleisbildCs2Stream);
         }
         free(GleisbildFile);
      }
   }
}
