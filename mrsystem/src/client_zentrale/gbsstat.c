#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <boolean.h>
#include <map.h>
#include <cs2parse.h>
#include <write_cs2.h>
#include "gbsstat.h"

void GbsStatInsert(GleisbildStruct *Data, GleisbildInfo *Gleisbild)
{  int Id;
   GleisbildInfo *OldGleisbild;

   Id = GleisbildInfoGetId(Gleisbild);
   OldGleisbild = (GleisbildInfo *)MapGet(GleisbildGetGleisbildDb(Data),
                                          (MapKeyType)Id);
   if (OldGleisbild != (GleisbildInfo *)NULL)
   {
      GleisbildInfoSetZustand(OldGleisbild, GleisbildInfoGetZustand(Gleisbild));
   }
}

void GbsStatParseGbsStatSr2(GleisbildStruct *Data, char *Buf, int Len)
{  Cs2parser *GbsStatParser;
   int NumPages, LineInfo;
   GleisbildInfo NewGleisbild;

   GbsStatParser = Cs2pCreate();
   Cs2pInit(GbsStatParser, PARSER_TYPE_GLEISBILD_CS2, Buf, Len);
   Cs2pSetVerbose(GbsStatParser, FALSE);
   NumPages = 0;
   do {
      LineInfo = Cs2pParse(GbsStatParser);
      switch (LineInfo)
      {
         case PARSER_ERROR:
            break;
         case PARSER_EOF:
            GbsStatInsert(Data, &NewGleisbild);
            break;
         case PARSER_PARAGRAPH:
            switch (Cs2pGetSubType(GbsStatParser))
            {
               default:
                  break;
            }
            break;
         case PARSER_VALUE:
            switch (Cs2pGetSubType(GbsStatParser))
            {
               case PARSER_VALUE_SEITE:
                  if (NumPages != 0)
                     GbsStatInsert(Data, &NewGleisbild);
                  NumPages++;
                  GleisbildInfoSetId(&NewGleisbild, 0);
                  GleisbildInfoSetZustand(&NewGleisbild, 0);
                  break;
               case PARSER_VALUE_VERSION:
                  break;
               case PARSER_VALUE_NAME:
                  GleisbildInfoSetName(&NewGleisbild,
                                       Cs2pGetValue(GbsStatParser));
                  break;
               case PARSER_VALUE_MAJOR:
                  break;
               case PARSER_VALUE_MINOR:
                  break;
               case PARSER_VALUE_ELEMENT:
                  break;
               case PARSER_VALUE_ID:
                  GleisbildInfoSetId(&NewGleisbild,
                                     strtoul(Cs2pGetValue(GbsStatParser),
                                             NULL, 0));
                  break;
               case PARSER_VALUE_ZUSTAND:
                  GleisbildInfoSetId(&NewGleisbild,
                                     strtoul(Cs2pGetValue(GbsStatParser),
                                             NULL, 0));
                  break;
            }
            break;
      }
   } while (LineInfo != PARSER_EOF);
   Cs2pExit(GbsStatParser);
   Cs2pDestroy(GbsStatParser);
}

void GbsStatLoadGbsStatSr2(GleisbildStruct *Data)
{  FILE *GbsStatSr2Stream;
   struct stat attribut;
   char *GbsStatFileName, *GbsStatFileContent;

   if (GleisbildGetGleisbildFilePath(Data) != (char *)NULL)
   {
      GbsStatFileName = (char *)malloc(strlen(GleisbildGetGleisbildFilePath(Data)) + 
                                       strlen(CS2_FILE_STRING_STATUS_GLEISBILD) +
                                       2);
      if (GbsStatFileName != (char *)NULL)
      {
         strcpy(GbsStatFileName, GleisbildGetGleisbildFilePath(Data));
         if (GbsStatFileName[strlen(GbsStatFileName) - 1] != '/')
            strcat(GbsStatFileName, "/");
         if (strcat(GbsStatFileName, CS2_FILE_STRING_STATUS_GLEISBILD) == 0)
         {
            stat(GbsStatFileName, &attribut);
            GbsStatFileContent = (char *)malloc(attribut.st_size);
            if (GbsStatFileContent != (char *)NULL)
            {
               GbsStatSr2Stream = fopen(GbsStatFileName, "r");
               if (GbsStatSr2Stream != NULL)
               {
                  fread(GbsStatFileContent, 1, attribut.st_size,
                        GbsStatSr2Stream);
                  GleisbildParseGleisbildCs2(Data, GbsStatFileContent,
                                             attribut.st_size);
                  Cs2Close(GbsStatSr2Stream);
               }
               free(GbsStatFileContent);
            }
         }
         free(GbsStatFileName);
      }
   }
}

static void WriteGbsStatOfGbsStatSr2(void *PrivData, MapKeyType Key,
                                     MapDataType Daten)
{  GleisbildInfo *GleisbildPage;
   FILE *GbsStatSr2Stream;

   GleisbildPage = (GleisbildInfo *)Daten;
   GbsStatSr2Stream = (FILE *)PrivData;
   if (GleisbildInfoGetId(GleisbildPage) != 0)
   {
      Cs2WriteTitleByName(GbsStatSr2Stream, "element", 0);
      Cs2WriteIntValueByName(GbsStatSr2Stream, "id",
                             GleisbildInfoGetId(GleisbildPage), 1);
      if (GleisbildInfoGetZustand(GleisbildPage) != 0)
      {
         Cs2WriteIntValueByName(GbsStatSr2Stream, "zustand",
                                GleisbildInfoGetZustand(GleisbildPage), 1);
      }
   }
}

void GbsStatSaveGbsStatSr2(GleisbildStruct *Data)
{  FILE *GbsStatSr2Stream;
   char *GbsStatFile;

   if (GleisbildGetGleisbildFilePath(Data) != (char *)NULL)
   {
      GbsStatFile = (char *)malloc(strlen(GleisbildGetGleisbildFilePath(Data)) +
                                   strlen(CS2_FILE_STRING_STATUS_GLEISBILD) +
                                   2);
      if (GbsStatFile != (char *)NULL)
      {
         strcpy(GbsStatFile, GleisbildGetGleisbildFilePath(Data));
         if (GbsStatFile[strlen(GbsStatFile) - 1] != '/')
            strcat(GbsStatFile, "/");
         strcat(GbsStatFile, CS2_FILE_STRING_STATUS_GLEISBILD);
         GbsStatSr2Stream = Cs2OpenByName(GbsStatFile);
         if (GbsStatSr2Stream != NULL)
         {
            fchmod(fileno(GbsStatSr2Stream),
                   S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
            Cs2WriteParagraphByType(GbsStatSr2Stream,
                                    CS2_PARAGRAPH_TYPE_GLEISBILD);
            Cs2WriteTitleByName(GbsStatSr2Stream, "version", 0);
            Cs2WriteIntValueByName(GbsStatSr2Stream, "major", 0, 1);
            Cs2WriteIntValueByName(GbsStatSr2Stream, "minor", 1, 1);
            MapWalkAscend(GleisbildGetGleisbildDb(Data),
                          (MapWalkCbFkt)WriteGbsStatOfGbsStatSr2,
                          (void *)GbsStatSr2Stream);
            Cs2Close(GbsStatSr2Stream);
         }
         free(GbsStatFile);
      }
   }
}
