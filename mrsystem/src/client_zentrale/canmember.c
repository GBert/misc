#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <boolean.h>
#include <map.h>
#include <menge.h>
#include <write_cs2.h>
#include <mr_cs2ms2.h>
#include <cs2.h>
#include <uci.h>
#include "canmember.h"


#define WRITE_ALL_INFORMATIONS
#define CS2_FILE_STRING_CONFIG "config.cs2"
#define WEB_DIR_MEMBER   "member"
#define WEB_DIR_MESSWERT "messwert"
#define WEB_DIR_KONFIG   "konfig"
#define WEB_FILE_BASE_MEMBER   "member"
#define WEB_FILE_BASE_MESSWERT "messwert"
#define WEB_FILE_BASE_KONFIG   "konfig"


CanMemberInfo *CanMemberInfoCreate(void)
{  CanMemberInfo *NewData;

   NewData = (CanMemberInfo *)malloc(sizeof(CanMemberInfo));
   if (NewData != (CanMemberInfo *)NULL)
   {
       CanMemberInfoSetConfigStatus(NewData, ConfigStatusInfoCreate());
       if (CanMemberInfoGetConfigStatus(NewData) == (ConfigStatusInfo *)NULL)
       {
          free(NewData);
          NewData = (CanMemberInfo *)NULL;
       }
   }
   return(NewData);
}

void CanMemberInfoDestroy(CanMemberInfo *Data)
{
   if (CanMemberInfoGetConfigStatus(Data) != (ConfigStatusInfo *)NULL)
      ConfigStatusInfoDestroy(CanMemberInfoGetConfigStatus(Data));
   free(Data);
}

void CanMemberInfoInit(CanMemberInfo *Data)
{
   CanMemberInfoSetIsInvalid(Data, FALSE);
   CanMemberInfoSetUid(Data, 0l);
   CanMemberInfoSetVersion(Data, 0);
   CanMemberInfoSetType(Data, 0);
   CanMemberInfoSetConfigQuerried(Data, FALSE);
   ConfigStatusInfoInit(CanMemberInfoGetConfigStatus(Data));
}

void CanMemberInfoExit(CanMemberInfo *Data)
{
   if (CanMemberInfoGetConfigStatus(Data) != (ConfigStatusInfo *)NULL)
      ConfigStatusInfoExit(CanMemberInfoGetConfigStatus(Data));
}

CanMemberStruct *CanMemberCreate(void)
{  CanMemberStruct *NewData;

   NewData = (CanMemberStruct *)malloc(sizeof(CanMemberStruct));
   if (NewData != (CanMemberStruct *)NULL)
   {
      CanMemberSetNumMembers(NewData, 0);
      CanMemberSetCanMemberDb(NewData, MapCreate());
      if (CanMemberGetCanMemberDb(NewData) == (Map *)NULL)
      {
         free(NewData);
         NewData = (CanMemberStruct *)NULL;
      }
   }
   return(NewData);
}

void CanMemberDestroy(CanMemberStruct *Data)
{
   if (CanMemberGetCanMemberDb(Data) != (Map *)NULL)
      MapDestroy(CanMemberGetCanMemberDb(Data));
   free(Data);
}

static int CanMemberUidCmp(void *d1, void *d2)
{
   return((unsigned long)d1 - (unsigned long)d2);
}

void CanMemberInit(CanMemberStruct *Data)
{
   CanMemberSetNumMembers(Data, 0);
   CanMemberSetIsChanged(Data, TRUE);
   MapInit(CanMemberGetCanMemberDb(Data), (CmpFkt)CanMemberUidCmp,
           (MapKeyDelCbFkt)NULL, (MapDataDelCbFkt)CanMemberInfoDestroy);
}

void CanMemberExit(CanMemberStruct *Data)
{
}

void CanMemberClear(CanMemberStruct *Data)
{
   if (CanMemberGetCanMemberDb(Data) != (Map *)NULL)
      MapDestroy(CanMemberGetCanMemberDb(Data));
   CanMemberSetCanMemberDb(Data, MapCreate());
   CanMemberSetNumMembers(Data, 0);
   if (CanMemberGetCanMemberDb(Data) != (Map *)NULL)
   {
      MapInit(CanMemberGetCanMemberDb(Data), (CmpFkt)CanMemberUidCmp,
              (MapKeyDelCbFkt)NULL, (MapDataDelCbFkt)CanMemberInfoDestroy);
   }
}

void CanMemberInsert(CanMemberStruct *Data, CanMemberInfo *CanMember)
{  unsigned long Uid;
   CanMemberInfo *OldCanMember;

   Uid = CanMemberInfoGetUid(CanMember);
   OldCanMember = (CanMemberInfo *)MapGet(CanMemberGetCanMemberDb(Data),
                                          (MapKeyType)Uid);
   if (OldCanMember != (CanMemberInfo *)NULL)
   {
      CanMemberInfoSetIsInvalid(OldCanMember, FALSE);
      CanMemberInfoSetUid(OldCanMember, CanMemberInfoGetUid(CanMember));
      CanMemberInfoSetVersion(OldCanMember, CanMemberInfoGetVersion(CanMember));
      CanMemberInfoSetType(OldCanMember, CanMemberInfoGetType(CanMember));
   }
   else
   {
      OldCanMember = CanMemberInfoCreate();
      if (OldCanMember != (CanMemberInfo *)NULL)
      {
         CanMemberInfoInit(OldCanMember);
         CanMemberInfoSetIsInvalid(OldCanMember,  FALSE);
         CanMemberInfoSetUid(OldCanMember, CanMemberInfoGetUid(CanMember));
         CanMemberInfoSetVersion(OldCanMember, CanMemberInfoGetVersion(CanMember));
         CanMemberInfoSetType(OldCanMember, CanMemberInfoGetType(CanMember));
         CanMemberInfoSetConfigQuerried(OldCanMember, CanMemberInfoGetConfigQuerried(CanMember));
         MapSet(CanMemberGetCanMemberDb(Data),
                (MapKeyType)Uid, (MapDataType)OldCanMember);
         CanMemberSetNumMembers(Data, CanMemberGetNumMembers(Data) + 1);
      }
   }
}

void CanMemberDelete(CanMemberStruct *Data, CanMemberInfo *CanMember)
{  unsigned long Uid;

   Uid = CanMemberInfoGetUid(CanMember);
   MapDel(CanMemberGetCanMemberDb(Data), (MapKeyType)Uid);
   CanMemberSetNumMembers(Data, CanMemberGetNumMembers(Data) - 1);
}

CanMemberInfo *CanMemberSearch(CanMemberStruct *Data, unsigned long Uid)
{
   return((CanMemberInfo *)MapGet(CanMemberGetCanMemberDb(Data),
                                  (MapKeyType)Uid));
}

typedef struct {
   CanMemberInfo *CanMember;
} SearchConfiguredStruct;

static void SearchNotConfigured(void *PrivData, MapKeyType Key, MapDataType Daten)
{  CanMemberInfo *Data;
   SearchConfiguredStruct *ConfSearch;

   Data = (CanMemberInfo *)Daten;
   ConfSearch = (SearchConfiguredStruct *)PrivData;
   if ((ConfSearch->CanMember == (CanMemberInfo *)NULL) &&
       !CanMemberInfoGetConfigQuerried(Data))
   {
      ConfSearch->CanMember = Data;
   }
}

CanMemberInfo *CanMemberSearchNotConfigured(CanMemberStruct *Data)
{  SearchConfiguredStruct ConfSearch;

   ConfSearch.CanMember = (CanMemberInfo *)NULL;
   MapWalkAscend(CanMemberGetCanMemberDb(Data),
                 (MapWalkCbFkt)SearchNotConfigured, (void *)&ConfSearch);
   return(ConfSearch.CanMember);
}

static void SearchMs2(void *PrivData, MapKeyType Key, MapDataType Daten)
{  CanMemberInfo *Data;
   SearchConfiguredStruct *ConfSearch;

   Data = (CanMemberInfo *)Daten;
   ConfSearch = (SearchConfiguredStruct *)PrivData;
   if ((ConfSearch->CanMember == (CanMemberInfo *)NULL) &&
       (CanMemberInfoGetType(Data) == CS2_DEVID_MS2_2))
   {
      ConfSearch->CanMember = Data;
   }
}

CanMemberInfo *CanMemberSearchMs2(CanMemberStruct *Data)
{  SearchConfiguredStruct ConfSearch;

   ConfSearch.CanMember = (CanMemberInfo *)NULL;
   MapWalkAscend(CanMemberGetCanMemberDb(Data),
                 (MapWalkCbFkt)SearchMs2, (void *)&ConfSearch);
   return(ConfSearch.CanMember);
}

static void MarkInvalid(void *PrivData, MapKeyType Key, MapDataType Daten)
{  CanMemberInfo *Data;

   Data = (CanMemberInfo *)Daten;
   CanMemberInfoSetIsInvalid(Data, TRUE);
}

void CanMemberMarkAllInvalid(CanMemberStruct *Data)
{
   MapWalkAscend(CanMemberGetCanMemberDb(Data), (MapWalkCbFkt)MarkInvalid,
                 (void *)NULL);
}

typedef struct {
   Map *CanMemberDb;
   Menge *MemberToDel;
} CollectStruct;

static void CollectInvalid(void *PrivData, MapKeyType Key, MapDataType Daten)
{  CanMemberInfo *Data;
   CollectStruct *Collector;
   unsigned long *UidToDel;

   Data = (CanMemberInfo *)Daten;
   Collector = (CollectStruct *)PrivData;
   if (CanMemberInfoGetIsInvalid(Data))
   {
      UidToDel = (unsigned long *)malloc(sizeof(unsigned long));
      if (UidToDel != (unsigned long *)NULL)
      {
         *UidToDel = CanMemberInfoGetUid(Data);
         MengeAdd(Collector->MemberToDel, (MengeDataType)UidToDel);
      }
   }
}

int MemberDelCmp(void *d1, void *d2)
{
   return((int)((unsigned long)d1 - (unsigned long)d2));
}

void CanMemberDelAllInvalid(CanMemberStruct *Data)
{  Menge *MemberToDel;
   MengeIterator IterToDel;
   CollectStruct Collector;
   unsigned long *UidToDel;

   MemberToDel = MengeCreate();
   MengeInit(MemberToDel, MemberDelCmp, (MengeDelCbFkt)free);
   Collector.CanMemberDb = CanMemberGetCanMemberDb(Data);
   Collector.MemberToDel = MemberToDel;
   MapWalkAscend(CanMemberGetCanMemberDb(Data), (MapWalkCbFkt)CollectInvalid,
                 (void *)&Collector);
   MengeInitIterator(&IterToDel, MemberToDel);
   UidToDel = (unsigned long *)MengeFirst(&IterToDel);
   while (UidToDel != 0l)
   {
      MapDel(CanMemberGetCanMemberDb(Data), (MapKeyType)*UidToDel);
      UidToDel = (unsigned long *)MengeNext(&IterToDel);
      CanMemberSetIsChanged(Data, TRUE);
   }
   MengeDestroy(MemberToDel);
}

static void WriteMesswertOfMesswerteCs2(void *PrivData,
                                        MapKeyType Key, MapDataType Daten)
{  CanMemberInfo *CanMember;
   FILE *MesswertCs2Stream;

   CanMember = (CanMemberInfo *)Daten;
   MesswertCs2Stream = (FILE *)PrivData;
   Cs2WriteTitleByName(MesswertCs2Stream, "member", 0);
   Cs2WriteULongValueByName(MesswertCs2Stream, "uid", CanMemberInfoGetUid(CanMember), 1);
   Cs2WriteIntValueByName(MesswertCs2Stream, "version", CanMemberInfoGetVersion(CanMember), 1);
   Cs2WriteHexValueByName(MesswertCs2Stream, "type", CanMemberInfoGetType(CanMember), 1);
   Cs2WriteStringValueByName(MesswertCs2Stream, "artikelnr", ConfigGeraetTypGetArtikelNr(ConfigStatusInfoGetGeraeteTyp(CanMemberInfoGetConfigStatus(CanMember))), 1);
   Cs2WriteStringValueByName(MesswertCs2Stream, "bezeichnung", ConfigGeraetTypGetGeraeteBezeichnung(ConfigStatusInfoGetGeraeteTyp(CanMemberInfoGetConfigStatus(CanMember))), 1);
   Cs2WriteIntValueByName(MesswertCs2Stream, "nummesswerte", ConfigGeraetTypGetNumMesswerte(ConfigStatusInfoGetGeraeteTyp(CanMemberInfoGetConfigStatus(CanMember))), 1);
   Cs2WriteIntValueByName(MesswertCs2Stream, "numkonfigwerte", ConfigGeraetTypGetNumKonfigwerte(ConfigStatusInfoGetGeraeteTyp(CanMemberInfoGetConfigStatus(CanMember))), 1);
   Cs2WriteULongValueByName(MesswertCs2Stream, "serial", ConfigGeraetTypGetSerialNum(ConfigStatusInfoGetGeraeteTyp(CanMemberInfoGetConfigStatus(CanMember))), 1);
   ConfigStatusSaveMesswerte(CanMemberInfoGetConfigStatus(CanMember),
                             MesswertCs2Stream);
   ConfigStatusSaveKonfigwerte(CanMemberInfoGetConfigStatus(CanMember),
                               MesswertCs2Stream);
}

void CanMemberSaveMesswerteCs2(CanMemberStruct *Data, char *LocFilePath)
{  FILE *MesswertCs2Stream;
   char *MesswertFile;

   if (CanMemberGetIsChanged(Data))
   {
      if (LocFilePath != (char *)NULL)
      {
         MesswertFile = (char *)alloca(strlen(LocFilePath) + 
                                       strlen(CS2_FILE_STRING_CONFIG) + 2);
         if (MesswertFile != (char *)NULL)
         {
            strcpy(MesswertFile, LocFilePath);
            if (MesswertFile[strlen(MesswertFile) - 1] != '/')
               strcat(MesswertFile, "/");
            strcat(MesswertFile, CS2_FILE_STRING_CONFIG);
            MesswertCs2Stream = Cs2OpenByName(MesswertFile);
            if (MesswertCs2Stream != NULL)
            {
               fchmod(fileno(MesswertCs2Stream),
                      S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
               Cs2WriteParagraphByName(MesswertCs2Stream, "messwerte");
               Cs2WriteTitleByName(MesswertCs2Stream, "version", 0);
               Cs2WriteIntValueByName(MesswertCs2Stream, "major", 0, 1);
               Cs2WriteIntValueByName(MesswertCs2Stream, "minor", 1, 1);
               MapWalkAscend(CanMemberGetCanMemberDb(Data),
                             (MapWalkCbFkt)WriteMesswertOfMesswerteCs2,
                             (void *)MesswertCs2Stream);
               Cs2Close(MesswertCs2Stream);
            }
         }
      }
   }
}

typedef struct {
   FILE *UciStream;
   char *UciFilePath;
} IteratorParamStruct;

#define IterParamSetStream(Data,Val)   (Data)->UciStream=Val
#define IterParamSetFilePath(Data,Val) (Data)->UciFilePath=Val

#define IterParamGetStream(Data)   (Data)->UciStream
#define IterParamGetFilePath(Data) (Data)->UciFilePath

static void ClearMember(void *PrivData, MapKeyType Key, MapDataType Daten)
{  CanMemberInfo *CanMember;

   CanMember = (CanMemberInfo *)Daten;
   ConfigStatusInfoClear(CanMemberInfoGetConfigStatus(CanMember));
   CanMemberInfoSetConfigQuerried(CanMember, FALSE);
}

void CanMemberClearConfig(CanMemberStruct *Data)
{
   MapWalkAscend(CanMemberGetCanMemberDb(Data),
                 (MapWalkCbFkt)ClearMember,
                 (void *)NULL);
}

static void WriteMember(void *PrivData, MapKeyType Key, MapDataType Daten)
{  CanMemberInfo *CanMember;
   FILE *MemberUciStream, *ValueUciStream;
   char IntStr[10];
   IteratorParamStruct *IteratorParam;

   CanMember = (CanMemberInfo *)Daten;
   IteratorParam = (IteratorParamStruct *)PrivData;
   sprintf(IntStr, "%ld", CanMemberInfoGetUid(CanMember));
   MemberUciStream = UciOpenDefWr(IterParamGetFilePath(IteratorParam), IntStr);
   if (MemberUciStream != (FILE *)NULL)
   {
      ValueUciStream = UciOpenCfgWr(IterParamGetFilePath(IteratorParam), IntStr);
      if (ValueUciStream != (FILE *)NULL)
      {
         fchmod(fileno(MemberUciStream),
                S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
         fchmod(fileno(ValueUciStream),
                S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
         /* add uci tab */
         UciWrUciTab(IterParamGetStream(IteratorParam), IntStr, WEB_DIR_MEMBER);
         /* add uci tab config */
         UciWrTxt(MemberUciStream, ValueUciStream, "uid", 3, 3, IntStr);
         sprintf(IntStr, "%d", CanMemberInfoGetVersion(CanMember));
         UciWrTxt(MemberUciStream, ValueUciStream, "version", 1, 10, IntStr);
         sprintf(IntStr, "%d", CanMemberInfoGetType(CanMember));
         UciWrTxt(MemberUciStream, ValueUciStream, "type", 1, 5, IntStr);
         UciWrTxt(MemberUciStream, ValueUciStream, "artikelnr", 1, 10,
                  ConfigGeraetTypGetArtikelNr(ConfigStatusInfoGetGeraeteTyp(CanMemberInfoGetConfigStatus(CanMember))));
         UciWrTxt(MemberUciStream, ValueUciStream, "bezeichnung", 1, 50,
                  ConfigGeraetTypGetGeraeteBezeichnung(ConfigStatusInfoGetGeraeteTyp(CanMemberInfoGetConfigStatus(CanMember))));
         UciCloseCfg(ValueUciStream);
      }
      UciCloseDef(MemberUciStream);
   }
}

static void WriteMemberMrsystem(FILE *MemberStream, char *WebPath,
                                unsigned long LocalUid,
                                unsigned Major, unsigned Minor)
{  FILE *MemberUciStream, *ValueUciStream;
   ConfigStatusInfo *MrsystemInfo;
   char IntStr[10];

   MrsystemInfo = ConfigStatusInfoCreate();
   if (MrsystemInfo != (ConfigStatusInfo *)NULL)
   {
      ConfigStatusInfoInit(MrsystemInfo);
      ConfigStatusInfoGetMrsystem(MrsystemInfo, Major, Minor);
      sprintf(IntStr, "%ld", LocalUid);
      MemberUciStream = UciOpenDefWr(WebPath, IntStr);
      if (MemberUciStream != (FILE *)NULL)
      {
         ValueUciStream = UciOpenCfgWr(WebPath, IntStr);
         if (ValueUciStream != (FILE *)NULL)
         {
            fchmod(fileno(MemberUciStream),
                   S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
            fchmod(fileno(ValueUciStream),
                   S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
            /* add uci tab */
            UciWrUciTab(MemberStream, IntStr, WEB_DIR_MEMBER);
            /* add uci tab config */
            UciWrDefHdr(MemberUciStream, IntStr);
            UciWrTxt(MemberUciStream, ValueUciStream, "uid", 3, 3, IntStr);
            sprintf(IntStr, "%d.%02d", Major, Minor);
            UciWrTxt(MemberUciStream, ValueUciStream, "version", 1, 10, IntStr);
            sprintf(IntStr, "%d", CS2_DEVID_WIRED);
            UciWrTxt(MemberUciStream, ValueUciStream, "type", 1, 5, IntStr);
            UciWrTxt(MemberUciStream, ValueUciStream, "artikelnr", 1, 10,
                     ConfigGeraetTypGetArtikelNr(ConfigStatusInfoGetGeraeteTyp(MrsystemInfo)));
            UciWrTxt(MemberUciStream, ValueUciStream, "bezeichnung", 1, 50,
                     ConfigGeraetTypGetGeraeteBezeichnung(ConfigStatusInfoGetGeraeteTyp(MrsystemInfo)));
            UciCloseCfg(ValueUciStream);
         }
         UciCloseDef(MemberUciStream);
      }
      ConfigStatusInfoExit(MrsystemInfo);
      ConfigStatusInfoDestroy(MrsystemInfo);
   }
}

static void WriteWebCanMember(CanMemberStruct *Data, char *WebPath,
                              unsigned long LocalUid,
                              unsigned Major, unsigned Minor)
{  FILE *MemberStream;
   char *MemberFile;
   IteratorParamStruct IteratorParam;

   MemberFile = (char *)alloca(strlen(WebPath) + 
                               strlen(CS2_FILE_STRING_CONFIG) + 2);
   if (MemberFile != (char *)NULL)
   {
      strcpy(MemberFile, WebPath);
      strcat(MemberFile, WEB_DIR_MEMBER);
      MemberStream = UciOpenUciWr(MemberFile, WEB_FILE_BASE_MEMBER);
      if (MemberStream != (FILE *)NULL)
      {
         fchmod(fileno(MemberStream),
                S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
         UciWrUciHdr(MemberStream, WEB_FILE_BASE_MEMBER, UciSelTypeTab);
         WriteMemberMrsystem(MemberStream, MemberFile, LocalUid, Major, Minor);
         IterParamSetStream(&IteratorParam, MemberStream);
         IterParamSetFilePath(&IteratorParam, MemberFile);
         MapWalkAscend(CanMemberGetCanMemberDb(Data),
                       (MapWalkCbFkt)WriteMember,
                       (void *)&IteratorParam);
         UciCloseUci(MemberStream);
      }
   }
}

static void WriteMesswert(void *PrivData, MapKeyType Key, MapDataType Daten)
{  CanMemberInfo *CanMember;
   MengeIterator IterToWrite;
   ConfigMesswertTyp *MesswertToWrite;
   char IntStr[10];
   IteratorParamStruct *IteratorParam;

   CanMember = (CanMemberInfo *)Daten;
   IteratorParam = (IteratorParamStruct *)PrivData;
   /* loop over all messwerte */
   MengeInitIterator(&IterToWrite, ConfigStatusInfoGetMesswerte(CanMemberInfoGetConfigStatus(CanMember)));
   MesswertToWrite = (ConfigMesswertTyp *)MengeFirst(&IterToWrite);
   if (MesswertToWrite != (ConfigMesswertTyp *)NULL)
   {
      /* add uci tab */
      sprintf(IntStr, "%ld", CanMemberInfoGetUid(CanMember));
      UciWrUciTab(IterParamGetStream(IteratorParam), IntStr, WEB_DIR_MESSWERT);
      /* add uci tab config */
      ConfigStatusWebDeviceMesswerte(&IterToWrite,
                                     IterParamGetFilePath(IteratorParam),
                                     CanMemberInfoGetUid(CanMember));
   }
}

static void WriteWebCanMesswerte(CanMemberStruct *Data, char *WebPath)
{  FILE *MesswertStream;
   char *MesswertFile;
   IteratorParamStruct IteratorParam;

   MesswertFile = (char *)alloca(strlen(WebPath) + 
                                 strlen(WEB_DIR_MESSWERT) + 2);
   if (MesswertFile != (char *)NULL)
   {
      strcpy(MesswertFile, WebPath);
      strcat(MesswertFile, WEB_DIR_MESSWERT);
      MesswertStream = UciOpenUciWr(MesswertFile, WEB_FILE_BASE_MESSWERT);
      if (MesswertStream != (FILE *)NULL)
      {
         fchmod(fileno(MesswertStream),
                S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
         UciWrUciHdr(MesswertStream, WEB_FILE_BASE_MESSWERT, UciSelTypeTab);
         IterParamSetStream(&IteratorParam, MesswertStream);
         IterParamSetFilePath(&IteratorParam, MesswertFile);
         MapWalkAscend(CanMemberGetCanMemberDb(Data),
                       (MapWalkCbFkt)WriteMesswert,
                       (void *)&IteratorParam);
         UciCloseUci(MesswertStream);
      }
   }
}

static void WriteKonfig(void *PrivData, MapKeyType Key, MapDataType Daten)
{  CanMemberInfo *CanMember;
   MengeIterator IterToWrite;
   char IntStr[10];
   ConfigTyp *KonfigToWrite;
   IteratorParamStruct *IteratorParam;

   CanMember = (CanMemberInfo *)Daten;
   if (ConfigStatusInfoGetTypen(CanMemberInfoGetConfigStatus(CanMember)) != (Menge *)NULL)
   {
      IteratorParam = (IteratorParamStruct *)PrivData;
      /* loop over all konfigwerte */
      MengeInitIterator(&IterToWrite, ConfigStatusInfoGetTypen(CanMemberInfoGetConfigStatus(CanMember)));
      KonfigToWrite = (ConfigTyp *)MengeFirst(&IterToWrite);
      while (KonfigToWrite != (ConfigTyp *)NULL)
      {
         /* add uci tab */
         sprintf(IntStr, "%ld", CanMemberInfoGetUid(CanMember));
         UciWrUciTab(IterParamGetStream(IteratorParam),
                     IntStr, WEB_DIR_KONFIG);
         /* add uci tab config */
         ConfigStatusWebDeviceKonfig(&IterToWrite,
                                    IterParamGetFilePath(IteratorParam),
                                     CanMemberInfoGetUid(CanMember));
         KonfigToWrite = (ConfigTyp *)MengeNext(&IterToWrite);
      }
   }
}

static void WriteWebCanKonfig(CanMemberStruct *Data, char *WebPath,
                              unsigned long LocalUid)
{  FILE *KonfigStream;
   char *KonfigFile;
   IteratorParamStruct IteratorParam;

   KonfigFile = (char *)alloca(strlen(WebPath) + 
                               strlen(WEB_DIR_KONFIG) + 2);
   if (KonfigFile != (char *)NULL)
   {
      strcpy(KonfigFile, WebPath);
      strcat(KonfigFile, WEB_DIR_KONFIG);
      KonfigStream = UciOpenUciWr(KonfigFile, WEB_FILE_BASE_KONFIG);
      if (KonfigStream != (FILE *)NULL)
      {
         fchmod(fileno(KonfigStream),
                S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
         UciWrUciHdr(KonfigStream, WEB_FILE_BASE_KONFIG, UciSelTypeTab);
         IterParamSetStream(&IteratorParam, KonfigStream);
         IterParamSetFilePath(&IteratorParam, KonfigFile);
         MapWalkAscend(CanMemberGetCanMemberDb(Data),
                       (MapWalkCbFkt)WriteKonfig,
                       (void *)&IteratorParam);
         UciCloseUci(KonfigStream);
      }
   }
}

void CanMemberSaveMesswerteWeb(CanMemberStruct *Data, char *LocFilePath,
                               unsigned long LocalUid,
                               unsigned Major, unsigned Minor)

{  char WebBasePath[80];

   if (CanMemberGetIsChanged(Data))
   {
      strcpy(WebBasePath, LocFilePath);
      if (WebBasePath[strlen(WebBasePath) - 1] != '/')
         WebBasePath[strlen(WebBasePath) - 1] = '\0';
      *(strrchr(WebBasePath, '/')) = '\0';
      strcat(WebBasePath, "/");
      WriteWebCanMember(Data, WebBasePath, LocalUid, Major, Minor);
      WriteWebCanMesswerte(Data, WebBasePath);
      WriteWebCanKonfig(Data, WebBasePath, LocalUid);
   }
}
