#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <boolean.h>
#include <map.h>
#include <cs2parse.h>
#include <write_cs2.h>
#include "fsstat.h"

void FsStatInsert(FahrstrasseStruct *Data, FahrstrasseInfo *Fahrstrasse)
{  int Id;
   FahrstrasseInfo *OldFahrstrasse;

   Id = FahrstrasseInfoGetId(Fahrstrasse);
   OldFahrstrasse = (FahrstrasseInfo *)MapGet(FahrstrasseGetFahrstrasseDb(Data),
                                              (MapKeyType)Id);
   if (OldFahrstrasse != (FahrstrasseInfo *)NULL)
   {
      FahrstrasseInfoSetOn(OldFahrstrasse,
                           FahrstrasseInfoGetOn(Fahrstrasse));
   }
}

void FsStatParseFsStatSr2(FahrstrasseStruct *Data, char *Buf, int Len)
{  Cs2parser *FsStatParser;
   int NumFahrstrassen, LineInfo;
   FahrstrasseInfo NewFahrstrasse;

   FsStatParser = Cs2pCreate();
   Cs2pInit(FsStatParser, PARSER_TYPE_FAHRSTRASSEN_CS2, Buf, Len);
   Cs2pSetVerbose(FsStatParser, FALSE);
   NumFahrstrassen = 0;
   do {
      LineInfo = Cs2pParse(FsStatParser);
      switch (LineInfo)
      {
         case PARSER_ERROR:
            break;
         case PARSER_EOF:
            FahrstrasseInsert(Data, &NewFahrstrasse);
            break;
         case PARSER_PARAGRAPH:
            switch (Cs2pGetSubType(FsStatParser))
            {
               default:
                  break;
            }
            break;
         case PARSER_VALUE:
            switch (Cs2pGetSubType(FsStatParser))
            {
               case PARSER_VALUE_FAHRSTRASSE:
                  if (NumFahrstrassen != 0)
                  {
                     FahrstrasseInsert(Data, &NewFahrstrasse);
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
                                       strtoul(Cs2pGetValue(FsStatParser),
                                               NULL, 0));
                  break;
               case PARSER_VALUE_ON:
                  FahrstrasseInfoSetOn(&NewFahrstrasse,
                                       strtoul(Cs2pGetValue(FsStatParser),
                                               NULL, 0));
                  break;
            }
            break;
      }
   } while (LineInfo != PARSER_EOF);
   Cs2pExit(FsStatParser);
   Cs2pDestroy(FsStatParser);
}

void FsStatLoadFsStatSr2(FahrstrasseStruct *Data)
{  FILE *FsStatSr2Stream;
   struct stat attribut;
   char *FsStatFileName, *FsStatFileContent;

   if (FahrstrasseGetFahrstrasseFilePath(Data) != (char *)NULL)
   {
      FsStatFileName = (char *)malloc(strlen(FahrstrasseGetFahrstrasseFilePath(Data)) + 
                                      strlen(CS2_FILE_STRING_STATUS_FAHRSTRASSE) +
                                      2);
      if (FsStatFileName != (char *)NULL)
      {
         strcpy(FsStatFileName, FahrstrasseGetFahrstrasseFilePath(Data));
         if (FsStatFileName[strlen(FsStatFileName) - 1] != '/')
            strcat(FsStatFileName, "/");
         strcat(FsStatFileName, CS2_FILE_STRING_STATUS_FAHRSTRASSE);
         if (stat(FsStatFileName, &attribut) == 0)
         {
            FsStatFileContent = (char *)malloc(attribut.st_size);
            if (FsStatFileContent != (char *)NULL)
            {
               FsStatSr2Stream = fopen(FsStatFileName, "r");
               if (FsStatSr2Stream != NULL)
               {
                  fread(FsStatFileContent, 1, attribut.st_size,
                        FsStatSr2Stream);
                  FahrstrasseParseFahrstrasseCs2(Data, FsStatFileContent,
                                                 attribut.st_size);
                  Cs2Close(FsStatSr2Stream);
               }
               free(FsStatFileContent);
            }
         }
         free(FsStatFileName);
      }
   }
}

static void WriteFsStatOfFsStatSr2(void *PrivData, MapKeyType Key,
                                   MapDataType Daten)
{  FahrstrasseInfo *FahrstrassePage;
   FILE *FsStatSr2Stream;

   FahrstrassePage = (FahrstrasseInfo *)Daten;
   FsStatSr2Stream = (FILE *)PrivData;
   Cs2WriteTitleByName(FsStatSr2Stream, "fahrstrasse", 0);
   Cs2WriteIntValueByName(FsStatSr2Stream, "id",
                          FahrstrasseInfoGetId(FahrstrassePage), 1);
   if (FahrstrasseInfoGetOn(FahrstrassePage) != 0)
   {
      Cs2WriteIntValueByName(FsStatSr2Stream, "on",
                             FahrstrasseInfoGetOn(FahrstrassePage), 1);
   }
}

void FsStatSaveFsStatSr2(FahrstrasseStruct *Data)
{  FILE *FsStatSr2Stream;
   char *FsStatFile;

   if (FahrstrasseGetFahrstrasseFilePath(Data) != (char *)NULL)
   {
      FsStatFile = (char *)malloc(strlen(FahrstrasseGetFahrstrasseFilePath(Data)) +
                                  strlen(CS2_FILE_STRING_STATUS_FAHRSTRASSE) +
                                  2);
      if (FsStatFile != (char *)NULL)
      {
         strcpy(FsStatFile, FahrstrasseGetFahrstrasseFilePath(Data));
         if (FsStatFile[strlen(FsStatFile) - 1] != '/')
            strcat(FsStatFile, "/");
         strcat(FsStatFile, CS2_FILE_STRING_STATUS_FAHRSTRASSE);
         FsStatSr2Stream = Cs2OpenByName(FsStatFile);
         if (FsStatSr2Stream != NULL)
         {
            fchmod(fileno(FsStatSr2Stream),
                   S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
            Cs2WriteParagraphByType(FsStatSr2Stream,
                                    CS2_PARAGRAPH_TYPE_FAHRSTRASSEN);
            Cs2WriteTitleByName(FsStatSr2Stream, "version", 0);
            Cs2WriteIntValueByName(FsStatSr2Stream, "major", 0, 1);
            Cs2WriteIntValueByName(FsStatSr2Stream, "minor", 1, 1);
            MapWalkAscend(FahrstrasseGetFahrstrasseDb(Data),
                          (MapWalkCbFkt)WriteFsStatOfFsStatSr2,
                          (void *)FsStatSr2Stream);
            Cs2Close(FsStatSr2Stream);
         }
         free(FsStatFile);
      }
   }
}
