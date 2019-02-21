#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <boolean.h>
#include <map.h>
#include <cs2parse.h>
#include <write_cs2.h>
#include "magnetartikel.h"
#include <stdio.h>

MagnetartikelStruct *MagnetartikelCreate(void)
{  MagnetartikelStruct *NewData;

   NewData = (MagnetartikelStruct *)malloc(sizeof(MagnetartikelStruct));
   if (NewData != (MagnetartikelStruct *)NULL)
   {
      MagnetartikelSetMagnetartikelFilePath(NewData, "/var/www/config/");
      MagnetartikelSetNumArtikel(NewData, 0);
      MagnetartikelSetMagnetartikelDb(NewData, MapCreate());
      if (MagnetartikelGetMagnetartikelDb(NewData) == (Map *)NULL)
      {
         free(NewData);
         NewData = (MagnetartikelStruct *)NULL;
      }
   }
   return(NewData);
}

void MagnetartikelDestroy(MagnetartikelStruct *Data)
{
   if (MagnetartikelGetMagnetartikelDb(Data) != (Map *)NULL)
      MapDestroy(MagnetartikelGetMagnetartikelDb(Data));
   free(Data);
}

static int MagnetartikelIdCmp(void *d1, void *d2)
{
   return((int)d1 - (int)d2);
}

void MagnetartikelInit(MagnetartikelStruct *Data, char *MagnetartikelPath)
{
   MagnetartikelSetMagnetartikelFilePath(Data, MagnetartikelPath);
   MagnetartikelSetNumArtikel(Data, 0);
   MapInit(MagnetartikelGetMagnetartikelDb(Data), (CmpFkt)MagnetartikelIdCmp,
           (MapKeyDelCbFkt)NULL, (MapDataDelCbFkt)free);
}

void MagnetartikelExit(MagnetartikelStruct *Data)
{
}

void MagnetartikelClear(MagnetartikelStruct *Data)
{
   if (MagnetartikelGetMagnetartikelDb(Data) != (Map *)NULL)
      MapDestroy(MagnetartikelGetMagnetartikelDb(Data));
      MagnetartikelSetMagnetartikelDb(Data, MapCreate());
   MagnetartikelSetNumArtikel(Data, 0);
   if (MagnetartikelGetMagnetartikelDb(Data) != (Map *)NULL)
   {
      MapInit(MagnetartikelGetMagnetartikelDb(Data), (CmpFkt)MagnetartikelIdCmp,
              (MapKeyDelCbFkt)NULL, (MapDataDelCbFkt)free);
   }
}

void MagnetartikelInsert(MagnetartikelStruct *Data,
                         MagnetartikelInfo *Magnetartikel)
{  int Id;
   MagnetartikelInfo *OldMagnetartikel;

   Id = MagnetartikelInfoGetId(Magnetartikel);
   OldMagnetartikel = (MagnetartikelInfo *)MapGet(MagnetartikelGetMagnetartikelDb(Data),
                                          (MapKeyType)Id);
   if (OldMagnetartikel != (MagnetartikelInfo *)NULL)
   {
      MagnetartikelInfoSetId(OldMagnetartikel, MagnetartikelInfoGetId(Magnetartikel));
      MagnetartikelInfoSetName(OldMagnetartikel, MagnetartikelInfoGetName(Magnetartikel));
      MagnetartikelInfoSetTyp(OldMagnetartikel, MagnetartikelInfoGetTyp(Magnetartikel));
      MagnetartikelInfoSetStellung(OldMagnetartikel, MagnetartikelInfoGetStellung(Magnetartikel));
      MagnetartikelInfoSetSchaltzeit(OldMagnetartikel, MagnetartikelInfoGetSchaltzeit(Magnetartikel));
      MagnetartikelInfoSetUngerade(OldMagnetartikel, MagnetartikelInfoGetUngerade(Magnetartikel));
      MagnetartikelInfoSetDecoder(OldMagnetartikel, MagnetartikelInfoGetDecoder(Magnetartikel));
      MagnetartikelInfoSetDectyp(OldMagnetartikel, MagnetartikelInfoGetDectyp(Magnetartikel));
   }
   else
   {
      OldMagnetartikel = (MagnetartikelInfo *)malloc(sizeof(MagnetartikelInfo));
      if (OldMagnetartikel != (MagnetartikelInfo *)NULL)
      {
         MagnetartikelInfoSetId(OldMagnetartikel, MagnetartikelInfoGetId(Magnetartikel));
         MagnetartikelInfoSetName(OldMagnetartikel, MagnetartikelInfoGetName(Magnetartikel));
         MagnetartikelInfoSetTyp(OldMagnetartikel, MagnetartikelInfoGetTyp(Magnetartikel));
         MagnetartikelInfoSetStellung(OldMagnetartikel, MagnetartikelInfoGetStellung(Magnetartikel));
         MagnetartikelInfoSetSchaltzeit(OldMagnetartikel, MagnetartikelInfoGetSchaltzeit(Magnetartikel));
         MagnetartikelInfoSetUngerade(OldMagnetartikel, MagnetartikelInfoGetUngerade(Magnetartikel));
         MagnetartikelInfoSetDecoder(OldMagnetartikel, MagnetartikelInfoGetDecoder(Magnetartikel));
         MagnetartikelInfoSetDectyp(OldMagnetartikel, MagnetartikelInfoGetDectyp(Magnetartikel));
         MapSet(MagnetartikelGetMagnetartikelDb(Data),
                (MapKeyType)Id, (MapDataType)OldMagnetartikel);
      }
   }
}

void MagnetartikelDelete(MagnetartikelStruct *Data,
                         MagnetartikelInfo *Magnetartikel)
{  int Id;

   Id = MagnetartikelInfoGetId(Magnetartikel);
   MapDel(MagnetartikelGetMagnetartikelDb(Data), (MapKeyType)Id);
}

MagnetartikelInfo *MagnetartikelSearch(MagnetartikelStruct *Data, int Id)
{
   return((MagnetartikelInfo *)MapGet(MagnetartikelGetMagnetartikelDb(Data),
                                      (MapKeyType)Id));
}

BOOL MagnetartikelParseMagnetartikelCs2(MagnetartikelStruct *Data, char *Buf, int Len)
{  Cs2parser *MagnetartikelParser;
   int NumArtikel, LineInfo;
   MagnetartikelInfo NewMagnetartikel;

   MagnetartikelParser = Cs2pCreate();
   Cs2pInit(MagnetartikelParser, PARSER_TYPE_MAGNETARTIKEL_CS2, Buf, Len);
   Cs2pSetVerbose(MagnetartikelParser, FALSE);
   MagnetartikelInfoSetUngerade(&NewMagnetartikel, -1);
   NumArtikel = 0;
   do {
      LineInfo = Cs2pParse(MagnetartikelParser);
      switch (LineInfo)
      {
         case PARSER_ERROR:
            break;
         case PARSER_EOF:
            if (strcmp(MagnetartikelInfoGetTyp(&NewMagnetartikel),
                       CS_MAGNETARTIKEL_TYP_STRING_STD_ROT_GRUEN) == 0)
               MagnetartikelDelete(Data, &NewMagnetartikel);
            else
               MagnetartikelInsert(Data, &NewMagnetartikel);
            break;
         case PARSER_PARAGRAPH:
            switch (Cs2pGetSubType(MagnetartikelParser))
            {
               default:
                  break;
            }
            break;
         case PARSER_VALUE:
            switch (Cs2pGetSubType(MagnetartikelParser))
            {
               case PARSER_VALUE_ARTIKEL:
                  if (NumArtikel != 0)
                  {
                     if (strcmp(MagnetartikelInfoGetTyp(&NewMagnetartikel),
                                CS_MAGNETARTIKEL_TYP_STRING_STD_ROT_GRUEN) == 0)
                        MagnetartikelDelete(Data, &NewMagnetartikel);
                     else
                        MagnetartikelInsert(Data, &NewMagnetartikel);
                  }
                  MagnetartikelInfoSetUngerade(&NewMagnetartikel, -1);
                  NumArtikel++;
                  break;
               case PARSER_VALUE_VERSION:
                  break;
               case PARSER_VALUE_MAJOR:
                  break;
               case PARSER_VALUE_MINOR:
                  break;
               case PARSER_VALUE_ID:
                  MagnetartikelInfoSetId(&NewMagnetartikel,
                                         strtoul(Cs2pGetValue(MagnetartikelParser),
                                                 NULL, 0));
                  break;
               case PARSER_VALUE_NAME:
                  MagnetartikelInfoSetName(&NewMagnetartikel,
                                           Cs2pGetValue(MagnetartikelParser));
                  break;
               case PARSER_VALUE_TYP:
                  MagnetartikelInfoSetTyp(&NewMagnetartikel,
                                          Cs2pGetValue(MagnetartikelParser));
                  break;
               case PARSER_VALUE_STELLUNG:
                  MagnetartikelInfoSetStellung(&NewMagnetartikel,
                                               strtoul(Cs2pGetValue(MagnetartikelParser),
                                                       NULL, 0));
                  break;
               case PARSER_VALUE_SCHALTZEIT:
                  MagnetartikelInfoSetSchaltzeit(&NewMagnetartikel,
                                                 strtoul(Cs2pGetValue(MagnetartikelParser),
                                                         NULL, 0));
                  break;
               case PARSER_VALUE_UNGERADE:
                  MagnetartikelInfoSetUngerade(&NewMagnetartikel,
                                               strtoul(Cs2pGetValue(MagnetartikelParser),
                                                       NULL, 0));
                  break;
               case PARSER_VALUE_DECODER:
                  MagnetartikelInfoSetDecoder(&NewMagnetartikel,
                                              Cs2pGetValue(MagnetartikelParser));
                  break;
               case PARSER_VALUE_DECTYP:
                  MagnetartikelInfoSetDectyp(&NewMagnetartikel,
                                             Cs2pGetValue(MagnetartikelParser));
                  break;
            }
            break;
      }
   } while ((LineInfo != PARSER_EOF) && (LineInfo != PARSER_ERROR));
   Cs2pExit(MagnetartikelParser);
   Cs2pDestroy(MagnetartikelParser);
   return(LineInfo == PARSER_EOF);
}

void MagnetartikelLoadMagnetartikelCs2(MagnetartikelStruct *Data)
{  FILE *MagnetartikelCs2Stream;
   struct stat attribut;
   char *MagnetartikelFileName, *MagnetartikelFileContent;

   if (MagnetartikelGetMagnetartikelFilePath(Data) != (char *)NULL)
   {
      MagnetartikelFileName = (char *)malloc(strlen(MagnetartikelGetMagnetartikelFilePath(Data)) + 
                                             strlen(CS2_FILE_STRING_MAGNETARTIKEL) +
                                             2);
      if (MagnetartikelFileName != (char *)NULL)
      {
         strcpy(MagnetartikelFileName, MagnetartikelGetMagnetartikelFilePath(Data));
         if (MagnetartikelFileName[strlen(MagnetartikelFileName) - 1] != '/')
            strcat(MagnetartikelFileName, "/");
         strcat(MagnetartikelFileName, CS2_FILE_STRING_MAGNETARTIKEL);
         if (stat(MagnetartikelFileName, &attribut) == 0)
         {
            MagnetartikelFileContent = (char *)malloc(attribut.st_size);
            if (MagnetartikelFileContent != (char *)NULL)
            {
               MagnetartikelCs2Stream = fopen(MagnetartikelFileName, "r");
               if (MagnetartikelCs2Stream != NULL)
               {
                  fread(MagnetartikelFileContent, 1, attribut.st_size,
                        MagnetartikelCs2Stream);
                  if (!MagnetartikelParseMagnetartikelCs2(Data,
                                                          MagnetartikelFileContent,
                                                          attribut.st_size))
                  {
                     MagnetartikelClear(Data);
                  }
                  Cs2Close(MagnetartikelCs2Stream);
               }
               free(MagnetartikelFileContent);
            }
         }
         free(MagnetartikelFileName);
      }
   }
}

static void WriteMagnetartikelOfMagnetartikelCs2(void *PrivData,
                                                 MapKeyType Key,
                                                 MapDataType Daten)
{  MagnetartikelInfo *MagnetartikelPage;
   FILE *MagnetartikelCs2Stream;

   MagnetartikelPage = (MagnetartikelInfo *)Daten;
   MagnetartikelCs2Stream = (FILE *)PrivData;
   Cs2WriteTitleByName(MagnetartikelCs2Stream, "artikel", 0);
   Cs2WriteIntValueByName(MagnetartikelCs2Stream, "id",
                          MagnetartikelInfoGetId(MagnetartikelPage), 1);
   Cs2WriteStringValueByName(MagnetartikelCs2Stream, "name",
                             MagnetartikelInfoGetName(MagnetartikelPage), 1);
   Cs2WriteStringValueByName(MagnetartikelCs2Stream, "typ",
                             MagnetartikelInfoGetTyp(MagnetartikelPage), 1);
   Cs2WriteIntValueByName(MagnetartikelCs2Stream, "stellung",
                          MagnetartikelInfoGetStellung(MagnetartikelPage), 1);
   Cs2WriteIntValueByName(MagnetartikelCs2Stream, "schaltzeit",
                          MagnetartikelInfoGetSchaltzeit(MagnetartikelPage), 1);
   if (MagnetartikelInfoGetUngerade(MagnetartikelPage) != -1)
      Cs2WriteIntValueByName(MagnetartikelCs2Stream, "ungerade",
                             MagnetartikelInfoGetUngerade(MagnetartikelPage), 1);
   Cs2WriteStringValueByName(MagnetartikelCs2Stream, "decoder",
                             MagnetartikelInfoGetDecoder(MagnetartikelPage), 1);
   Cs2WriteStringValueByName(MagnetartikelCs2Stream, "dectyp",
                             MagnetartikelInfoGetDectyp(MagnetartikelPage), 1);
}

void MagnetartikelSaveMagnetartikelCs2(MagnetartikelStruct *Data)
{  FILE *MagnetartikelCs2Stream;
   char *MagnetartikelFile;

   if (MagnetartikelGetMagnetartikelFilePath(Data) != (char *)NULL)
   {
      MagnetartikelFile = (char *)malloc(strlen(MagnetartikelGetMagnetartikelFilePath(Data)) +
                                         strlen(CS2_FILE_STRING_MAGNETARTIKEL) +
                                         2);
      if (MagnetartikelFile != (char *)NULL)
      {
         strcpy(MagnetartikelFile, MagnetartikelGetMagnetartikelFilePath(Data));
         if (MagnetartikelFile[strlen(MagnetartikelFile) - 1] != '/')
            strcat(MagnetartikelFile, "/");
         strcat(MagnetartikelFile, CS2_FILE_STRING_MAGNETARTIKEL);
         MagnetartikelCs2Stream = Cs2OpenByName(MagnetartikelFile);
         if (MagnetartikelCs2Stream != NULL)
         {
            fchmod(fileno(MagnetartikelCs2Stream),
                   S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
            Cs2WriteParagraphByType(MagnetartikelCs2Stream,
                                    CS2_PARAGRAPH_TYPE_MAGNETARTIKEL);
            Cs2WriteTitleByName(MagnetartikelCs2Stream, "version", 0);
            Cs2WriteIntValueByName(MagnetartikelCs2Stream, "major", 0, 1);
            Cs2WriteIntValueByName(MagnetartikelCs2Stream, "minor", 1, 1);
            MapWalkAscend(MagnetartikelGetMagnetartikelDb(Data),
                          (MapWalkCbFkt)WriteMagnetartikelOfMagnetartikelCs2,
                          (void *)MagnetartikelCs2Stream);
            Cs2Close(MagnetartikelCs2Stream);
         }
         free(MagnetartikelFile);
      }
   }
}
