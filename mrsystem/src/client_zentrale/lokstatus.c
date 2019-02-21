#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <boolean.h>
#include <cs2parse.h>
#include <write_cs2.h>
#include "lok.h"
#include "lokstatus.h"

void LokStatusInsert(LokStruct *Data, LokInfo *Lok)
{  char *Name;
   LokInfo *OldLok;
   int FktIdx;

   Name = LokInfoGetName(Lok);
   if (strlen(Name) > 0)
   {
      OldLok = (LokInfo *)MapGet(LokGetLokDb(Data), (MapKeyType)Name);
      if (OldLok != (LokInfo *)NULL)
      {
         LokInfoSetVelocity(OldLok, LokInfoGetVelocity(Lok));
         for (FktIdx = 0; FktIdx < LOK_NUM_FUNCTIONS; FktIdx++)
         {
            LokInfoSetFktWert(OldLok, FktIdx, LokInfoGetFktWert(Lok, FktIdx));
         }
      }
   }
}

void LokStatusParseLokomotiveSr2(LokStruct *Data, char *Buf, int Len)
{  Cs2parser *LokStatusParser;
   int NumLoks, LineInfo, FktIndex;
   LokInfo NewLok;

   NumLoks = 0;
   FktIndex = 0;
   LokStatusParser = Cs2pCreate();
   Cs2pInit(LokStatusParser, PARSER_TYPE_LOK_CS2, Buf, Len);
   Cs2pSetVerbose(LokStatusParser, FALSE);
   memset(&NewLok, 0, sizeof(LokInfo));
   do {
      LineInfo = Cs2pParse(LokStatusParser);
      switch (LineInfo)
      {
         case PARSER_ERROR:
            break;
         case PARSER_EOF:
            LokStatusInsert(Data, &NewLok);
            break;
         case PARSER_PARAGRAPH:
            switch (Cs2pGetSubType(LokStatusParser))
            {
               default:
                  break;
            }
            break;
         case PARSER_VALUE:
            switch (Cs2pGetSubType(LokStatusParser))
            {
               case PARSER_VALUE_LOKOMOTIVE:
                  if (NumLoks != 0)
                     LokStatusInsert(Data, &NewLok);
                  memset(&NewLok, 0, sizeof(LokInfo));
                  NumLoks++;
                  FktIndex = -1;
                  break;
               case PARSER_VALUE_LOK:
                  break;
               case PARSER_VALUE_VERSION:
                  break;
               case PARSER_VALUE_NAME:
                  LokInfoSetName(&NewLok,
                                 Cs2pGetValue(LokStatusParser));
                  break;
               case PARSER_VALUE_VELOCITY:
                  LokInfoSetVelocity(&NewLok,
                                     strtoul(Cs2pGetValue(LokStatusParser),
                                             NULL, 0));
                  break;
               case PARSER_VALUE_RICHTUNG:
                  LokInfoSetRichtung(&NewLok,
                                     strtoul(Cs2pGetValue(LokStatusParser),
                                             NULL, 0));
                  break;
               case PARSER_VALUE_FUNKTIONEN:
                  FktIndex++;
                  break;
               case PARSER_VALUE_WERT:
                  LokInfoSetFktWert(&NewLok, FktIndex,
                                    strtoul(Cs2pGetValue(LokStatusParser),
                                            NULL, 0));
                  break;
               case PARSER_VALUE_MAJOR:
                  break;
               case PARSER_VALUE_MINOR:
                  break;
            }
            break;
      }
   } while (LineInfo != PARSER_EOF);
   Cs2pExit(LokStatusParser);
   Cs2pDestroy(LokStatusParser);
}

void LokStatusLoadLokomotiveSr2(LokStruct *Data)
{  FILE *LokStatusSr2Stream;
   struct stat attribut;
   char *LokStatusFileName, *LokStatusFileContent;

   if (LokGetLocFilePath(Data) != (char *)NULL)
   {
      LokStatusFileName = (char *)malloc(strlen(LokGetLocFilePath(Data)) + 
                                         strlen(CS2_FILE_STRING_STATUS_LOKOMOTIVE) +
                                         2);
      if (LokStatusFileName != (char *)NULL)
      {
         strcpy(LokStatusFileName, LokGetLocFilePath(Data));
         if (LokStatusFileName[strlen(LokStatusFileName) - 1] != '/')
            strcat(LokStatusFileName, "/");
         strcat(LokStatusFileName, CS2_FILE_STRING_STATUS_LOKOMOTIVE);
         if (stat(LokStatusFileName, &attribut) == 0)
         {
            if (attribut.st_size > 0)
            {
               LokStatusFileContent = (char *)malloc(attribut.st_size);
               if (LokStatusFileContent != (char *)NULL)
               {
                  LokStatusSr2Stream = fopen(LokStatusFileName, "r");
                  if (LokStatusSr2Stream != NULL)
                  {
                     fread(LokStatusFileContent, 1, attribut.st_size,
                           LokStatusSr2Stream);
                     LokParseLokomotiveCs2(Data, LokStatusFileContent,
                                           attribut.st_size);
                     Cs2Close(LokStatusSr2Stream);
                  }
                  free(LokStatusFileContent);
               }
            }
         }
         free(LokStatusFileName);
      }
   }
}

static void WriteLokOfLokomotiveSr2(void *PrivData,
                                    MapKeyType Key, MapDataType Daten)
{  int i;
   LokInfo *Lok;
   FILE *LokStatusSr2Stream;
   LokStruct *Data;

   Data = (LokStruct *)PrivData;
   Lok = (LokInfo *)Daten;
   LokStatusSr2Stream = (FILE *)PrivData;
   if (!LokInfoGetIsDeleted(Lok))
   {
      Cs2WriteTitleByName(LokStatusSr2Stream, "lokomotive", 0);
      Cs2WriteStringValueByName(LokStatusSr2Stream, "name", LokInfoGetName(Lok), 1);
      if (LokInfoGetVelocity(Lok) != 0)
      {
         Cs2WriteIntValueByName(LokStatusSr2Stream, "velocity",
                                LokInfoGetVelocity(Lok), 1);
      }
      if (LokInfoGetRichtung(Lok) != 0)
      {
         Cs2WriteIntValueByName(LokStatusSr2Stream, "richtung",
                                LokInfoGetRichtung(Lok), 1);
      }
      for (i = 0; i < LokGetNumLokFkts(Data); i++)
      {
         Cs2WriteTitleByName(LokStatusSr2Stream, "funktionen", 1);
         Cs2WriteIntValueByName(LokStatusSr2Stream, "nr", i, 2);
         if (LokInfoGetFktWert(Lok, i) != 0)
         {
            Cs2WriteIntValueByName(LokStatusSr2Stream, "wert",
                                   LokInfoGetFktWert(Lok, i), 2);
         }
      }
   }
}

void LokStatusSaveLokomotiveSr2(LokStruct *Data)
{  FILE *LokStatusSr2Stream;
   char *LokStatusFile;

   if (LokGetIsChanged(Data))
   {
      if (LokGetLocFilePath(Data) != (char *)NULL)
      {
         LokStatusFile = (char *)malloc(strlen(LokGetLocFilePath(Data)) + 
                                        strlen(CS2_FILE_STRING_STATUS_LOKOMOTIVE) +
                                        2);
         if (LokStatusFile != (char *)NULL)
         {
            strcpy(LokStatusFile, LokGetLocFilePath(Data));
            if (LokStatusFile[strlen(LokStatusFile) - 1] != '/')
               strcat(LokStatusFile, "/");
            strcat(LokStatusFile, CS2_FILE_STRING_STATUS_LOKOMOTIVE);
            LokStatusSr2Stream = Cs2OpenByName(LokStatusFile);
            if (LokStatusSr2Stream != NULL)
            {
               fchmod(fileno(LokStatusSr2Stream),
                      S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
               Cs2WriteParagraphByType(LokStatusSr2Stream,
                                       CS2_PARAGRAPH_TYPE_LOKOMOTIVE);
               Cs2WriteTitleByName(LokStatusSr2Stream, "version", 0);
               Cs2WriteIntValueByName(LokStatusSr2Stream, "major", 0, 1);
               Cs2WriteIntValueByName(LokStatusSr2Stream, "minor", 1, 1);
               MapWalkAscend(LokGetLokDb(Data),
                             (MapWalkCbFkt)WriteLokOfLokomotiveSr2,
                             (void *)LokStatusSr2Stream);
               Cs2Close(LokStatusSr2Stream);
            }
            free(LokStatusFile);
         }
      }
   }
}
