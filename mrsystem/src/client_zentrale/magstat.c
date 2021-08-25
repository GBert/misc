#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <boolean.h>
#include <map.h>
#include <cs2parse.h>
#include <write_cs2.h>
#include "magstat.h"

void MagStatusInsert(MagnetartikelStruct *Data,
                     MagnetartikelInfo *Magnetartikel)
{  int Id;
   MagnetartikelInfo *OldMagnetartikel;

   Id = MagnetartikelInfoGetId(Magnetartikel);
   OldMagnetartikel = (MagnetartikelInfo *)MapGet(MagnetartikelGetMagnetartikelDb(Data),
                                          (MapKeyType)Id);
   if (OldMagnetartikel != (MagnetartikelInfo *)NULL)
   {
      MagnetartikelInfoSetStellung(OldMagnetartikel, MagnetartikelInfoGetStellung(Magnetartikel));
   }
}

void MagStatusParseMagStatusSr2(MagnetartikelStruct *Data, char *Buf, int Len)
{  Cs2parser *MagStatParser;
   int NumArtikel, LineInfo;
   MagnetartikelInfo NewMagStat;

   MagStatParser = Cs2pCreate();
   Cs2pInit(MagStatParser, PARSER_TYPE_MAGNETARTIKEL_CS2, Buf, Len);
   Cs2pSetVerbose(MagStatParser, FALSE);
   NumArtikel = 0;
   MagnetartikelInfoSetStellung(&NewMagStat, 0);
   do {
      LineInfo = Cs2pParse(MagStatParser);
      switch (LineInfo)
      {
         case PARSER_ERROR:
            break;
         case PARSER_EOF:
            MagnetartikelInsert(Data, &NewMagStat);
            break;
         case PARSER_PARAGRAPH:
            switch (Cs2pGetSubType(MagStatParser))
            {
               default:
                  break;
            }
            break;
         case PARSER_VALUE:
            switch (Cs2pGetSubType(MagStatParser))
            {
               case PARSER_VALUE_ARTIKEL:
                  if (NumArtikel != 0)
                  {
                     MagnetartikelInsert(Data, &NewMagStat);
                  }
                  MagnetartikelInfoSetStellung(&NewMagStat, 0);
                  NumArtikel++;
                  break;
               case PARSER_VALUE_VERSION:
                  break;
               case PARSER_VALUE_MAJOR:
                  break;
               case PARSER_VALUE_MINOR:
                  break;
               case PARSER_VALUE_ID:
                  MagnetartikelInfoSetId(&NewMagStat,
                                         strtoul(Cs2pGetValue(MagStatParser),
                                                 NULL, 0));
                  break;
               case PARSER_VALUE_STELLUNG:
                  MagnetartikelInfoSetStellung(&NewMagStat,
                                               strtoul(Cs2pGetValue(MagStatParser),
                                                       NULL, 0));
                  break;
            }
            break;
      }
   } while (LineInfo != PARSER_EOF);
   Cs2pExit(MagStatParser);
   Cs2pDestroy(MagStatParser);
}

void MagStatusLoadMagStatusSr2(MagnetartikelStruct *Data)
{  FILE *MagStatSr2Stream;
   struct stat attribut;
   char *MagStatusFileName, *MagStatusFileContent;

   if (MagnetartikelGetMagnetartikelFilePath(Data) != (char *)NULL)
   {
      MagStatusFileName = (char *)malloc(strlen(MagnetartikelGetMagnetartikelFilePath(Data)) + 
                                         strlen(CS2_FILE_STRING_STATUS_MAGNETARTIKEL) +
                                         2);
      if (MagStatusFileName != (char *)NULL)
      {
         strcpy(MagStatusFileName, MagnetartikelGetMagnetartikelFilePath(Data));
         if (MagStatusFileName[strlen(MagStatusFileName) - 1] != '/')
            strcat(MagStatusFileName, "/");
         strcat(MagStatusFileName, CS2_FILE_STRING_STATUS_MAGNETARTIKEL);
         if (stat(MagStatusFileName, &attribut) == 0)
         {
            MagStatusFileContent = (char *)malloc(attribut.st_size);
            if (MagStatusFileContent != (char *)NULL)
            {
               MagStatSr2Stream = fopen(MagStatusFileName, "r");
               if (MagStatSr2Stream != NULL)
               {
                  fread(MagStatusFileContent, 1, attribut.st_size,
                        MagStatSr2Stream);
                  MagnetartikelParseMagnetartikelCs2(Data,
                                                     MagStatusFileContent,
                                                     attribut.st_size);
                  Cs2Close(MagStatSr2Stream);
               }
               free(MagStatusFileContent);
            }
         }
         free(MagStatusFileName);
      }
   }
}

static void WriteMagStatusOfMagStatusSr2(void *PrivData, MapKeyType Key,
                                         MapDataType Daten)
{  MagnetartikelInfo *MagnetartikelPage;
   FILE *MagStatusSr2Stream;

   MagnetartikelPage = (MagnetartikelInfo *)Daten;
   MagStatusSr2Stream = (FILE *)PrivData;
   Cs2WriteTitleByName(MagStatusSr2Stream, "artikel", 0);
   Cs2WriteIntValueByName(MagStatusSr2Stream, "id",
                          MagnetartikelInfoGetId(MagnetartikelPage), 1);
   if (MagnetartikelInfoGetStellung(MagnetartikelPage) != 0)
   {
      Cs2WriteIntValueByName(MagStatusSr2Stream, "stellung",
                             MagnetartikelInfoGetStellung(MagnetartikelPage), 1);
   }
}

void MagStatusSaveMagStatusSr2(MagnetartikelStruct *Data)
{  FILE *MagStatusSr2Stream;
   char *MagStatusFile;

   if (MagnetartikelGetMagnetartikelFilePath(Data) != (char *)NULL)
   {
      MagStatusFile = (char *)malloc(strlen(MagnetartikelGetMagnetartikelFilePath(Data)) +
                                     strlen(CS2_FILE_STRING_STATUS_MAGNETARTIKEL) +
                                     2);
      if (MagStatusFile != (char *)NULL)
      {
         strcpy(MagStatusFile, MagnetartikelGetMagnetartikelFilePath(Data));
         if (MagStatusFile[strlen(MagStatusFile) - 1] != '/')
            strcat(MagStatusFile, "/");
         strcat(MagStatusFile, CS2_FILE_STRING_STATUS_MAGNETARTIKEL);
         MagStatusSr2Stream = Cs2OpenByName(MagStatusFile);
         if (MagStatusSr2Stream != NULL)
         {
            fchmod(fileno(MagStatusSr2Stream),
                   S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
            Cs2WriteParagraphByType(MagStatusSr2Stream,
                                    CS2_PARAGRAPH_TYPE_MAGNETARTIKEL);
            Cs2WriteTitleByName(MagStatusSr2Stream, "version", 0);
            Cs2WriteIntValueByName(MagStatusSr2Stream, "major", 0, 1);
            Cs2WriteIntValueByName(MagStatusSr2Stream, "minor", 1, 1);
            MapWalkAscend(MagnetartikelGetMagnetartikelDb(Data),
                          (MapWalkCbFkt)WriteMagStatusOfMagStatusSr2,
                          (void *)MagStatusSr2Stream);
            Cs2Close(MagStatusSr2Stream);
         }
         free(MagStatusFile);
      }
   }
}
