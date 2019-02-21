#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <boolean.h>
#include <cs2parse.h>
#include <write_cs2.h>
#include "zentrale.h"
#include "lok.h"

LokStruct *LokCreate(void)
{  LokStruct *NewData;

   NewData = (LokStruct *)malloc(sizeof(LokStruct));
   if (NewData != (LokStruct *)NULL)
   {
      LokSetLocFilePath(NewData, "/var/www/config/");
      LokSetNumLoks(NewData, 0);
      LokSetLokDb(NewData, MapCreate());
      if (LokGetLokDb(NewData) == (Map *)NULL)
      {
         free(NewData);
         NewData = (LokStruct *)NULL;
      }
   }
   return(NewData);
}

void LokDestroy(LokStruct *Data)
{
   if (LokGetLokDb(Data) != (Map *)NULL)
      MapDestroy(LokGetLokDb(Data));
   free(Data);
}

void LokInit(LokStruct *Data, char *LocPath, int NumLokFkts)
{
   LokSetLocFilePath(Data, LocPath);
   LokSetNumLoks(Data, 0);
   LokSetNumLokFkts(Data, NumLokFkts);
   LokSetIsChanged(Data, FALSE);
   MapInit(LokGetLokDb(Data), (CmpFkt)strcmp,
           (MapKeyDelCbFkt)NULL, (MapDataDelCbFkt)free);
}

void LokExit(LokStruct *Data)
{
}

void LokClear(LokStruct *Data)
{
   if (LokGetLokDb(Data) != (Map *)NULL)
      MapDestroy(LokGetLokDb(Data));
   LokSetNumLoks(Data, 0);
   LokSetIsChanged(Data, FALSE);
   LokSetLokDb(Data, MapCreate());
   if (LokGetLokDb(Data) != (Map *)NULL)
   {
      MapInit(LokGetLokDb(Data), (CmpFkt)strcmp,
              (MapKeyDelCbFkt)NULL, (MapDataDelCbFkt)free);
   }
}

void LokInfoInit(LokInfo *Data)
{
   memset(Data, 0, sizeof(LokInfo));
}

static BOOL LokIsEqual(LokInfo *Lok1, LokInfo *Lok2)
{  BOOL IsEqual;

   IsEqual = (LokInfoGetUid(Lok1) == LokInfoGetUid(Lok2)) &&
             (strcmp(LokInfoGetName(Lok1), LokInfoGetName(Lok2)) == 0) &&
             (LokInfoGetAdresse(Lok1) == LokInfoGetAdresse(Lok2)) &&
             (strcmp(LokInfoGetTyp(Lok1), LokInfoGetTyp(Lok2)) == 0) &&
             (LokInfoGetMfxUid(Lok1) == LokInfoGetMfxUid(Lok2)) &&
             (LokInfoGetSymbol(Lok1) == LokInfoGetSymbol(Lok2)) &&
             (LokInfoGetAv(Lok1) == LokInfoGetAv(Lok2)) &&
             (LokInfoGetBv(Lok1) == LokInfoGetBv(Lok2)) &&
             (LokInfoGetVolume(Lok1) == LokInfoGetVolume(Lok2)) &&
             (LokInfoGetVelocity(Lok1) == LokInfoGetVelocity(Lok2)) &&
             (LokInfoGetRichtung(Lok1) == LokInfoGetRichtung(Lok2)) &&
             (LokInfoGetVmax(Lok1) == LokInfoGetVmax(Lok2)) &&
             (LokInfoGetVmin(Lok1) == LokInfoGetVmin(Lok2));
   return(IsEqual);
}

void LokInsert(LokStruct *Data, LokInfo *Lok)
{  char *Name;
   LokInfo *OldLok;

   Name = LokInfoGetName(Lok);
   if (strlen(Name) > 0)
   {
      OldLok = (LokInfo *)MapGet(LokGetLokDb(Data), (MapKeyType)Name);
      if (OldLok != (LokInfo *)NULL)
      {
         if (!LokIsEqual(OldLok, Lok))
         {
            LokSetIsChanged(Data, TRUE);
            memcpy(OldLok, Lok, sizeof(LokInfo));
         }
         LokInfoSetIsDeleted(OldLok, FALSE);
      }
      else
      {
         OldLok = (LokInfo *)malloc(sizeof(LokInfo));
         if (OldLok != (LokInfo *)NULL)
         {
            LokSetIsChanged(Data, TRUE);
            memcpy(OldLok, Lok, sizeof(LokInfo));
            LokInfoSetIsDeleted(OldLok, FALSE);
            MapSet(LokGetLokDb(Data),
                   (MapKeyType)LokInfoGetName(OldLok),
                   (MapDataType)OldLok);
            LokSetNumLoks(Data, LokGetNumLoks(Data) + 1);
         }
      }
   }
}

typedef struct {
   unsigned long Addr;
   LokInfo *Result;
} AddrSearchType;

static void SearchLokomotiveByAddress(void *PrivData,
                                      MapKeyType Key, MapDataType Daten)
{  AddrSearchType *Search;

   Search = (AddrSearchType *)PrivData;
   if (Search->Addr == LokInfoGetUid((LokInfo *)Daten))
      Search->Result = (LokInfo *)Daten;
}

LokInfo *LokSearch(LokStruct *Data, unsigned long Addr)
{  AddrSearchType Search;

   Search.Addr = Addr;
   Search.Result = (LokInfo *)NULL;
   MapWalkAscend(LokGetLokDb(Data),
                 (MapWalkCbFkt)SearchLokomotiveByAddress,
                 (void *)&Search);
   return(Search.Result);
}

BOOL LokParseLokomotiveCs2(LokStruct *Data, char *Buf, int Len)
{  Cs2parser *LokParser;
   int NumLoks, LineInfo, FktIndex;
   LokInfo NewLok;

   NumLoks = 0;
   FktIndex = 0;
   LokParser = Cs2pCreate();
   Cs2pInit(LokParser, PARSER_TYPE_LOK_CS2, Buf, Len);
   Cs2pSetVerbose(LokParser, FALSE);
   LokInfoSetIsDeleted(&NewLok, FALSE);
   LokInfoInit(&NewLok);
   do {
      LineInfo = Cs2pParse(LokParser);
      switch (LineInfo)
      {
         case PARSER_ERROR:
            break;
         case PARSER_EOF:
            LokInsert(Data, &NewLok);
            break;
         case PARSER_PARAGRAPH:
            switch (Cs2pGetSubType(LokParser))
            {
               default:
                  break;
            }
            break;
         case PARSER_VALUE:
            switch (Cs2pGetSubType(LokParser))
            {
               case PARSER_VALUE_LOKOMOTIVE:
                  if (NumLoks != 0)
                     LokInsert(Data, &NewLok);
                  NumLoks++;
                  FktIndex = -1;
                  LokInfoInit(&NewLok);
                  LokInfoSetIsDeleted(&NewLok, FALSE);
                  break;
               case PARSER_VALUE_LOK:
                  break;
               case PARSER_VALUE_VERSION:
                  break;
               case PARSER_VALUE_UID:
                  LokInfoSetUid(&NewLok,
                                strtoul(Cs2pGetValue(LokParser), NULL, 0));
                  break;
               case PARSER_VALUE_NAME:
                  LokInfoSetName(&NewLok,
                                 Cs2pGetValue(LokParser));
                  break;
               case PARSER_VALUE_ADRESSE:
                  LokInfoSetAdresse(&NewLok,
                                    strtoul(Cs2pGetValue(LokParser), NULL, 0));
                  break;
               case PARSER_VALUE_TYP:
                  if (Cs2pGetLevel(LokParser) == 1)
                  {
                     LokInfoSetTyp(&NewLok, Cs2pGetValue(LokParser));
                  }
                  else if (Cs2pGetLevel(LokParser) == 2)
                  {
                     if (FktIndex < LOK_NUM_FUNCTIONS)
                     {
                        LokInfoSetFktTyp(&NewLok, FktIndex,
                                         strtoul(Cs2pGetValue(LokParser),
                                                 NULL, 0));
                     }
                  }
                  break;
               case PARSER_VALUE_MFXUID:
                  LokInfoSetMfxUid(&NewLok,
                                   strtoul(Cs2pGetValue(LokParser),
                                           NULL, 0));
                  break;
               case PARSER_VALUE_SYMBOL:
                  LokInfoSetSymbol(&NewLok,
                                   strtoul(Cs2pGetValue(LokParser),
                                           NULL, 0));
                  break;
               case PARSER_VALUE_AV:
                  LokInfoSetAv(&NewLok,
                               strtoul(Cs2pGetValue(LokParser),
                                       NULL, 0));
                  break;
               case PARSER_VALUE_BV:
                  LokInfoSetBv(&NewLok,
                               strtoul(Cs2pGetValue(LokParser),
                                       NULL, 0));
                  break;
               case PARSER_VALUE_VOLUME:
                  LokInfoSetVolume(&NewLok,
                                   strtoul(Cs2pGetValue(LokParser),
                                           NULL, 0));
                  break;
               case PARSER_VALUE_VELOCITY:
                  LokInfoSetVelocity(&NewLok,
                                     strtoul(Cs2pGetValue(LokParser),
                                             NULL, 0));
                  break;
               case PARSER_VALUE_RICHTUNG:
                  LokInfoSetRichtung(&NewLok,
                                     strtoul(Cs2pGetValue(LokParser),
                                             NULL, 0));
                  break;
               case PARSER_VALUE_VMAX:
                  LokInfoSetVmax(&NewLok,
                                 strtoul(Cs2pGetValue(LokParser),
                                         NULL, 0));
                  break;
               case PARSER_VALUE_VMIN:
                  LokInfoSetVmin(&NewLok,
                                 strtoul(Cs2pGetValue(LokParser),
                                         NULL, 0));
                  break;
               case PARSER_VALUE_FUNKTIONEN:
                  FktIndex++;
                  break;
               case PARSER_VALUE_DAUER:
                  if (FktIndex < LOK_NUM_FUNCTIONS)
                  {
                     LokInfoSetFktDauer(&NewLok, FktIndex,
                                        strtoul(Cs2pGetValue(LokParser),
                                                NULL, 0));
                  }
                  break;
               case PARSER_VALUE_WERT:
                  if (FktIndex < LOK_NUM_FUNCTIONS)
                  {
                     LokInfoSetFktWert(&NewLok, FktIndex,
                                       strtoul(Cs2pGetValue(LokParser),
                                               NULL, 0));
                  }
                  break;
               case PARSER_VALUE_MAJOR:
                  break;
               case PARSER_VALUE_MINOR:
                  break;
               case PARSER_VALUE_SESSION:
                  break;
               case PARSER_VALUE_ID:
                  break;
               case PARSER_VALUE_SID:
                  break;
               case PARSER_VALUE_ICON:
                  break;
               case PARSER_VALUE_TACHOMAX:
                  break;
               case PARSER_VALUE_XPROTOKOLL:
                  break;
               case PARSER_VALUE_MFXTYP:
                  break;
               case PARSER_VALUE_STAND:
                  break;
               case PARSER_VALUE_FAHRT:
                  break;
               case PARSER_VALUE_NR:
                  break;
               case PARSER_VALUE_VORWAERTS:
                  break;
               case PARSER_VALUE_RUECKWAERTS:
                  break;
               case PARSER_VALUE_INTRAKTION:
                  break;
            }
            break;
      }
   } while ((LineInfo != PARSER_EOF) && (LineInfo != PARSER_ERROR));
   Cs2pExit(LokParser);
   Cs2pDestroy(LokParser);
   return(LineInfo == PARSER_EOF);
}

void LokLoadLokomotiveCs2(LokStruct *Data)
{  FILE *LokCs2Stream;
   struct stat attribut;
   char *LokFileName, *LokFileContent;

   if (LokGetLocFilePath(Data) != (char *)NULL)
   {
      LokFileName = (char *)malloc(strlen(LokGetLocFilePath(Data)) + 
                                   strlen(CS2_FILE_STRING_LOKOMOTIVE) + 2);
      if (LokFileName != (char *)NULL)
      {
         strcpy(LokFileName, LokGetLocFilePath(Data));
         if (LokFileName[strlen(LokFileName) - 1] != '/')
            strcat(LokFileName, "/");
         strcat(LokFileName, CS2_FILE_STRING_LOKOMOTIVE);
         if (stat(LokFileName, &attribut) == 0)
         {
            if (attribut.st_size > 0)
            {
               LokFileContent = (char *)malloc(attribut.st_size);
               if (LokFileContent != (char *)NULL)
               {
                  LokCs2Stream = fopen(LokFileName, "r");
                  if (LokCs2Stream != NULL)
                  {
                     fread(LokFileContent, 1, attribut.st_size, LokCs2Stream);
                     if (!LokParseLokomotiveCs2(Data, LokFileContent, 
                                                attribut.st_size))
                     {
                        LokClear(Data);
                     }
                     Cs2Close(LokCs2Stream);
                     LokSetIsChanged(Data, FALSE);
                  }
                  free(LokFileContent);
               }
            }
         }
         free(LokFileName);
      }
   }
}

static void SearchDeleted(void *PrivData, MapKeyType Key, MapDataType Daten)
{  LokInfo *Lok;
   AddrSearchType *SearchData;

   Lok = (LokInfo *)Daten;
   SearchData = (AddrSearchType *)PrivData;
   if (LokInfoGetIsDeleted(Lok))
   {
      SearchData->Result = Lok;
   }
}

BOOL LokHaveDeleted(LokStruct *Data)
{  AddrSearchType Search;

   Search.Addr = 0l;
   Search.Result = (LokInfo *)NULL;
   MapWalkAscend(LokGetLokDb(Data), (MapWalkCbFkt)SearchDeleted,
                 (void *)&Search);
   return(Search.Result != (LokInfo *)NULL);
}

static void PurgeDeletedLoks(LokStruct *Data)
{  AddrSearchType Search;

   do {
      Search.Addr = 0l;
      Search.Result = (LokInfo *)NULL;
      MapWalkAscend(LokGetLokDb(Data), (MapWalkCbFkt)SearchDeleted,
                    (void *)&Search);
      if (Search.Result != (LokInfo *)NULL)
      {
         MapDel(LokGetLokDb(Data), LokInfoGetName(Search.Result));
      }
   } while (Search.Result != (LokInfo *)NULL);
}

typedef struct {
   LokStruct *Data;
   FILE *LokCs2Stream;
} LokWalkType;

static void WriteLokOfLokomotiveCs2(void *PrivData,
                                    MapKeyType Key, MapDataType Daten)
{  int i;
   LokStruct *Data;
   LokInfo *Lok;
   FILE *LokCs2Stream;

   Lok = (LokInfo *)Daten;
   Data = ((LokWalkType *)PrivData)->Data;
   LokCs2Stream = ((LokWalkType *)PrivData)->LokCs2Stream;
   if (!LokInfoGetIsDeleted(Lok))
   {
      Cs2WriteTitleByName(LokCs2Stream, "lokomotive", 0);
      Cs2WriteStringValueByName(LokCs2Stream, "name", LokInfoGetName(Lok), 1);
      Cs2WriteULongValueByName(LokCs2Stream, "uid", LokInfoGetUid(Lok), 1);
      Cs2WriteHexValueByName(LokCs2Stream, "adresse", LokInfoGetAdresse(Lok), 1);
      Cs2WriteStringValueByName(LokCs2Stream, "typ", LokInfoGetTyp(Lok), 1);
      Cs2WriteHexValueByName(LokCs2Stream, "sid", 1, 1);
      Cs2WriteULongValueByName(LokCs2Stream, "mfxuid", LokInfoGetMfxUid(Lok), 1);
      Cs2WriteStringValueByName(LokCs2Stream, "icon", LokInfoGetName(Lok), 1);
      Cs2WriteIntValueByName(LokCs2Stream, "symbol", LokInfoGetSymbol(Lok), 1);
      Cs2WriteIntValueByName(LokCs2Stream, "av", LokInfoGetAv(Lok), 1);
      Cs2WriteIntValueByName(LokCs2Stream, "bv", LokInfoGetBv(Lok), 1);
      Cs2WriteIntValueByName(LokCs2Stream, "volume", LokInfoGetVolume(Lok), 1);
      Cs2WriteIntValueByName(LokCs2Stream, "velocity", LokInfoGetVelocity(Lok), 1);
      Cs2WriteIntValueByName(LokCs2Stream, "richtung", LokInfoGetRichtung(Lok), 1);
      Cs2WriteIntValueByName(LokCs2Stream, "tachomax", 320, 1);
      Cs2WriteIntValueByName(LokCs2Stream, "vmax", LokInfoGetVmax(Lok), 1);
      Cs2WriteIntValueByName(LokCs2Stream, "vmin", LokInfoGetVmin(Lok), 1);
      Cs2WriteIntValueByName(LokCs2Stream, "xprotokoll", 0, 1);
      Cs2WriteIntValueByName(LokCs2Stream, "mfxtyp", 0, 1);
      Cs2WriteHexValueByName(LokCs2Stream, "stand", 0, 1);
      Cs2WriteHexValueByName(LokCs2Stream, "fahrt", 0, 1);
      for (i = 0; i < LokGetNumLokFkts(Data); i++)
      {
         Cs2WriteTitleByName(LokCs2Stream, "funktionen", 1);
         Cs2WriteIntValueByName(LokCs2Stream, "nr", i, 2);
         if (LokInfoGetFktTyp(Lok, i) != 0)
            Cs2WriteIntValueByName(LokCs2Stream, "typ", LokInfoGetFktTyp(Lok, i), 2);
         if (LokInfoGetFktDauer(Lok, i) != 0)
            Cs2WriteIntValueByName(LokCs2Stream, "dauer", LokInfoGetFktDauer(Lok, i), 2);
         if (LokInfoGetFktWert(Lok, i) != 0)
            Cs2WriteIntValueByName(LokCs2Stream, "wert", LokInfoGetFktWert(Lok, i), 2);
         if ((LokInfoGetFktTyp(Lok, i) != 0) &&
             (LokInfoGetFktDauer(Lok, i) != 0) &&
             (LokInfoGetFktWert(Lok, i) != 0))
         {
            Cs2WriteHexValueByName(LokCs2Stream, "vorwaerts", 0, 2);
            Cs2WriteHexValueByName(LokCs2Stream, "rueckwaerts", 0, 2);
         }
      }
      Cs2WriteHexLongValueByName(LokCs2Stream, "inTraktion", 0xffffffff, 1);
   }
}

void LokSaveLokomotiveCs2(LokStruct *Data)
{  FILE *LokCs2Stream;
   char *LokFile;
   LokWalkType LokWalk;

   if (LokGetIsChanged(Data) || LokHaveDeleted(Data))
   {
      if (LokGetLocFilePath(Data) != (char *)NULL)
      {
         LokFile = (char *)malloc(strlen(LokGetLocFilePath(Data)) + 
                                  strlen(CS2_FILE_STRING_LOKOMOTIVE) + 2);
         if (LokFile != (char *)NULL)
         {
            strcpy(LokFile, LokGetLocFilePath(Data));
            if (LokFile[strlen(LokFile) - 1] != '/')
               strcat(LokFile, "/");
            strcat(LokFile, CS2_FILE_STRING_LOKOMOTIVE);
            LokCs2Stream = Cs2OpenByName(LokFile);
            if (LokCs2Stream != NULL)
            {
               fchmod(fileno(LokCs2Stream),
                      S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
               Cs2WriteParagraphByType(LokCs2Stream, CS2_PARAGRAPH_TYPE_LOKOMOTIVE);
               Cs2WriteTitleByName(LokCs2Stream, "version", 0);
               Cs2WriteIntValueByName(LokCs2Stream, "major", 0, 1);
               Cs2WriteIntValueByName(LokCs2Stream, "minor", 1, 1);
               Cs2WriteTitleByName(LokCs2Stream, "session", 0);
               Cs2WriteIntValueByName(LokCs2Stream, "id", 1, 1);
               LokWalk.Data = Data;
               LokWalk.LokCs2Stream = LokCs2Stream;
               MapWalkAscend(LokGetLokDb(Data),
                             (MapWalkCbFkt)WriteLokOfLokomotiveCs2,
                             (void *)&LokWalk);
               Cs2Close(LokCs2Stream);
               PurgeDeletedLoks(Data);
               LokSetIsChanged(Data, FALSE);
            }
            free(LokFile);
         }
      }
   }
}

static void MarkDelete(void *PrivData, MapKeyType Key, MapDataType Daten)
{  LokInfo *Lok;

   Lok = (LokInfo *)Daten;
   LokInfoSetIsDeleted(Lok, TRUE);
}

void LokMarkAllDeleted(LokStruct *Data)
{
   MapWalkAscend(LokGetLokDb(Data), (MapWalkCbFkt)MarkDelete, (void *)NULL);
   LokSetNumLoks(Data, 0);
}

void LokMarkDeleted(LokStruct *Data, char *Name)
{  LokInfo *Lok;

   Lok = (LokInfo *)MapGet(LokGetLokDb(Data), (MapKeyType)Name);
   if (Lok != (LokInfo *)NULL)
   {
      LokInfoSetIsDeleted(Lok, TRUE);
      LokSetNumLoks(Data, LokGetNumLoks(Data) - 1);
   }
}

static void MarkUndelete(void *PrivData, MapKeyType Key, MapDataType Daten)
{  LokInfo *Lok;
   AddrSearchType *SearchData;

   Lok = (LokInfo *)Daten;
   SearchData = (AddrSearchType *)PrivData;
   LokInfoSetIsDeleted(Lok, FALSE);
   SearchData->Addr++;
}

void LokMarkAllUndeleted(LokStruct *Data)
{  AddrSearchType Search;

   LokSetNumLoks(Data, 0);
   Search.Addr = 0;
   MapWalkAscend(LokGetLokDb(Data), (MapWalkCbFkt)MarkUndelete,
                 (void *)&Search);
   LokSetNumLoks(Data, Search.Addr);
}
