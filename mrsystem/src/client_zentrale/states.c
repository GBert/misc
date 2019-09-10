#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <boolean.h>
#include <bytestream.h>
#include <cs2parse.h>
#include <write_cs2.h>
#include <fsm.h>
#include <mr_ipc.h>
#include <mr_ipchl.h>
#include <cs2.h>
#include <config.h>
#include "zentrale.h"
#include "lok.h"
#include "lokstatus.h"
#include "magstat.h"
#include "gbsstat.h"
#include "fsstat.h"
#include "config.h"

#define PERIODIC_NAME_NULL      ""
#define PERIODIC_NAME_QUERY     "query_member"
#define PERIODIC_NAME_END_QUERY "end_query_member"
#define PERIODIC_NAME_POLLMS2   "poll_ms2"
#define PERIODIC_NAME_PING      "ping"
#define PERIODIC_NAME_TO_CS2    "timeout_cs2"
#define PERIODIC_NAME_TO_MS2    "timeout_ms2"
#define PERIODIC_NAME_LOKNAME   "query_lokname"
#define PERIODIC_NAME_LOKINFO   "query_lokinfo"
#define PERIODIC_NAME_TO_NORMAL "timeout_normal"

#define PERIODIC_INTERVALL_QUERY    10
#define PERIODIC_INTERVALL_POLLMS2  60
#define PERIODIC_INTERVALL_PING     10
#define PERIODIC_INTERVALL_LOKNAME   1
#define PERIODIC_INTERVALL_LOKINFO   1
#define PERIODIC_INTERVALL_TIMEOUT   5
#define PERIODIC_INTERVALL_QPERIOD   1
#define STATE_TIMEOUT 5

#define PARAGRAPH_UNDEFINED 0
#define PARAGRAPH_LOK       1
#define PARAGRAPH_NUMLOKS   2

#define NUM_STATES 36
#define STATE_WAIT_FOR_MS2                0
#define STATE_WAIT_LOKNAME_CFG_HDR        1
#define STATE_WAIT_LOKNAME_CFG_DATA       2
#define STATE_WAIT_LOKINFO_CFG_HDR        3
#define STATE_WAIT_LOKINFO_CFG_DATA       4
#define STATE_WAIT_CFG_DATA               5
#define STATE_WAIT_CS2                    6
#define STATE_NORMAL                      7
#define STATE_GET_LOK_CS2_CFG_DATA        8
#define STATE_WAIT_LOK_CS2_CFG_HDR        9
#define STATE_WAIT_LOK_CS2_CFG_DATA      10
#define STATE_WAIT_MAG_CS2_CFG_HDR       11
#define STATE_WAIT_MAG_CS2_CFG_DATA      12
#define STATE_WAIT_FS_CS2_CFG_HDR        13
#define STATE_WAIT_FS_CS2_CFG_DATA       14
#define STATE_WAIT_GBS_CS2_CFG_HDR       15
#define STATE_WAIT_GBS_CS2_CFG_DATA      16
#define STATE_WAIT_GPG_CS2_CFG_HDR       17
#define STATE_WAIT_GPG_CS2_CFG_DATA      18
#define STATE_WAIT_LOK_CVR_CFG_HDR       19
#define STATE_WAIT_LOK_CVR_CFG_DATA      20
#define STATE_WAIT_MAG_CVR_CFG_HDR       21
#define STATE_WAIT_MAG_CVR_CFG_DATA      22
#define STATE_WAIT_GBS_CVR_CFG_HDR       23
#define STATE_WAIT_GBS_CVR_CFG_DATA      24
#define STATE_WAIT_FS_CVR_CFG_HDR        25
#define STATE_WAIT_FS_CVR_CFG_DATA       26
#define STATE_WAIT_CFG_DATA_HDR          27
#define STATE_WAIT_CFG_DATA_DATA         28
#define STATE_WAIT_SYS_CFG_MESSWERT      29
#define STATE_WAIT_MS2_LOKNAME_CFG_HDR   30
#define STATE_WAIT_MS2_LOKNAME_CFG_DATA  31
#define STATE_WAIT_MS2_LOKINFO_CFG_HDR   32
#define STATE_WAIT_MS2_LOKINFO_CFG_DATA  33
#define STATE_WAIT_LOKLISTE_CFG_HDR      34
#define STATE_WAIT_LOKLISTE_CFG_DATA     35


#define NUM_SIGNALS 27


typedef struct {
   int Length;
   unsigned char Data[8];
} S88SystemConfig;


static S88SystemConfig S88Bootldr[3] = {
   { 5, { 0x53, 0x38, 0x38, 0x00, 0xE4, 0x00, 0x00, 0x00 } },
   { 5, { 0x53, 0x38, 0x38, 0x00, 0x11, 0x00, 0x00, 0x00 } },
   { 7, { 0x53, 0x38, 0x38, 0x00, 0x0C, 0x00, 0x00, 0x00 } } };

static void SendPing(ZentraleStruct *Data)
{  MrIpcCmdType Cmd;

   MrIpcHlMemberRequest(&Cmd, ZentraleGetUid(Data));
   MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
}

static int QueryMembers(ZentraleStruct *Data, BOOL GetConfig)
{  MrIpcCmdType Cmd;
   CanMemberInfo *CanMember;
   int NextState;

   NextState = STATE_NO_CHANGE;
   if (!ZentraleGetIsInPoll(Data))
   {
      CanMemberDelAllInvalid(ZentraleGetCanMember(Data));
      if (GetConfig)
      {
         CanMember = CanMemberSearchNotConfigured(ZentraleGetCanMember(Data));
         if (CanMember != (CanMemberInfo *)NULL)
         {
            if (ZentraleGetVerbose(Data))
               printf("FSM: get member %ld status\n",
                      CanMemberInfoGetUid(CanMember));
            CanMemberInfoSetConfigQuerried(CanMember, TRUE);
            CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
            CanMemberSetIsChanged(ZentraleGetCanMember(Data), TRUE);
            ZentraleSetActualIndex(Data, 0);
            ZentraleSetActualConfig(Data, CanMemberInfoGetUid(CanMember));
            MrIpcHlStatusRequest(&Cmd, ZentraleGetUid(Data),
                                 CanMemberInfoGetUid(CanMember), 0);
            MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
            NextState = STATE_WAIT_CFG_DATA_HDR;
         }
         else
         {
            CanMemberMarkAllInvalid(ZentraleGetCanMember(Data));
            ZentraleSetIsInPoll(Data, TRUE);
            CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_END_QUERY);
            if (ZentraleGetVerbose(Data))
               puts("FSM: query members");
            SendPing(Data);
         }
      }
      else
      {
         CanMemberMarkAllInvalid(ZentraleGetCanMember(Data));
         ZentraleSetIsInPoll(Data, TRUE);
         CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_END_QUERY);
         if (ZentraleGetVerbose(Data))
            puts("FSM: query members");
         SendPing(Data);
      }
   }
   else
   {
      if (ZentraleGetVerbose(Data))
         puts("FSM: query members");
      SendPing(Data);
   }
   return(NextState);
}

static int PeriodicEndQueryMembers(void *PrivData)
{  ZentraleStruct *Data;

   Data = (ZentraleStruct *)PrivData;
   ZentraleSetIsInPoll(Data, FALSE);
   CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_END_QUERY);
   return(STATE_NO_CHANGE);
}

static int HandleTimer(void *Priv, void *SignalData)
{  ZentraleStruct *Data;

   Data = (ZentraleStruct *)Priv;
   if (ZentraleGetVerbose(Data))
      puts("FSM: periodic task");
   return(CronDo(ZentraleGetCronJobs(Data)));
}

static int HandleLokSpeed(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;
   unsigned long Addr;
   unsigned Speed;
   LokInfo *LokData;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetLocomotiveSpeed(CmdFrame, &Addr, &Speed);
   if (ZentraleGetVerbose(Data))
      printf("FSM: LOK Speed addr %lu, speed %d (response %d)\n",
             Addr, Speed, MrIpcGetCanResponse(CmdFrame));
   if (MrIpcGetCanResponse(CmdFrame))
   {
      LokData = LokSearch(ZentraleGetLoks(Data), Addr);
      if (LokData != (LokInfo *)NULL)
         LokInfoSetVelocity(LokData, Speed);
   }
   if (ZentraleGetVerbose(Data))
      printf("FSM: new state %d\n",STATE_NO_CHANGE);
   return(STATE_NO_CHANGE);
}

static int HandleLokDirection(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;
   unsigned long Addr;
   DirectionType Direction;
   LokInfo *LokData;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetLocomotiveDir(CmdFrame, &Addr, &Direction);
   if (ZentraleGetVerbose(Data))
      printf("FSM: LOK Direction addr %lu, speed %d (response %d)\n",
             Addr, (int)Direction, MrIpcGetCanResponse(CmdFrame));
   if (MrIpcGetCanResponse(CmdFrame))
   {
      LokData = LokSearch(ZentraleGetLoks(Data), Addr);
      if (LokData != (LokInfo *)NULL)
         LokInfoSetRichtung(LokData, (int)Direction);
   }
   if (ZentraleGetVerbose(Data))
      printf("FSM: new state %d\n",STATE_NO_CHANGE);
   return(STATE_NO_CHANGE);
}

static int HandleLokFunction(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;
   unsigned long Addr;
   unsigned Function;
   SwitchType Switch;
   LokInfo *LokData;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetLocomotiveFkt(CmdFrame, &Addr, &Function, &Switch);
   if (ZentraleGetVerbose(Data))
      printf("FSM: LOK Function addr %lu, function %d, value %d (response %d)\n",
             Addr, Function, (int)Switch, MrIpcGetCanResponse(CmdFrame));
   if (MrIpcGetCanResponse(CmdFrame))
   {
      LokData = LokSearch(ZentraleGetLoks(Data), Addr);
      if (LokData != (LokInfo *)NULL)
         LokInfoSetFktWert(LokData, Function, (int)Switch);
   }
   if (ZentraleGetVerbose(Data))
      printf("FSM: new state %d\n",STATE_NO_CHANGE);
   return(STATE_NO_CHANGE);
}

static int HandleFileRequest(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame, Cmd;
   char Name[9], *Dateiname, FullPath[255], *LokomotiveDaten;
   int i, j, FileLength;
   unsigned Hash;
   struct stat attribut;
   FILE *LokomotiveDatei;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetQuery(CmdFrame, Name);
   Name[8] = '\0';
   if (ZentraleGetVerbose(Data))
      printf("FSM: request %s\n", Name);
   if (strncmp(CS2_CFG_LOCINFO, Name, strlen(CS2_CFG_LOCINFO)) == 0)
      Dateiname = (char *)NULL;
   else if (strncmp(CS2_CFG_LOCNAMES, Name, strlen(CS2_CFG_LOCNAMES)) == 0)
      Dateiname = (char *)NULL;
   else if (strncmp(CS2_CFG_MAGINFO, Name, strlen(CS2_CFG_MAGINFO)) == 0)
      Dateiname = (char *)NULL;
   else if (strncmp(CS2_CFG_LOCDB, Name, strlen(CS2_CFG_LOCDB)) == 0)
      Dateiname = (char *)NULL;
   else if (strncmp(CS2_CFG_LANGVER, Name, strlen(CS2_CFG_LANGVER)) == 0)
      Dateiname = (char *)NULL;
   else if (strncmp(CS2_CFG_LANG, Name, strlen(CS2_CFG_LANG)) == 0)
      Dateiname = (char *)NULL;
   else if (strncmp(CS2_CFG_LDBVER, Name, strlen(CS2_CFG_LDBVER)) == 0)
      Dateiname = (char *)NULL;
   else if (strncmp(CS2_CFG_LOK_STAT, Name, strlen(CS2_CFG_LOK_STAT)) == 0)
   {
      LokStatusSaveLokomotiveSr2(ZentraleGetLoks(Data));
      Dateiname = CS2_FILE_STRING_STATUS_LOKOMOTIVE;
   }
   else if (strncmp(CS2_CFG_LOKS, Name, strlen(CS2_CFG_LOKS)) == 0)
      Dateiname = CS2_FILE_STRING_LOKOMOTIVE;
   else if (strncmp(CS2_CFG_MAGS, Name, strlen(CS2_CFG_MAGS)) == 0)
      Dateiname = CS2_FILE_STRING_MAGNETARTIKEL;
   else if (strncmp(CS2_CFG_GBS_PAGE, Name, strlen(CS2_CFG_GBS_PAGE)) == 0)
   {
      if (atoi(Name + strlen(CS2_CFG_GBS_PAGE)) <
          GleisbildGetNumPages(ZentraleGetGleisbild(Data)))
      {
         if (ZentraleGetNrGleisPages(Data,
                                     atoi(Name + strlen(CS2_CFG_GBS_PAGE))) != (GleisbildPageStruct *)NULL)
         {
            Dateiname = GleisbildPageGetName(ZentraleGetNrGleisPages(Data,
                                                                     atoi(Name + strlen(CS2_CFG_GBS_PAGE))));
         }
         else
            Dateiname = (char *)NULL;
      }
      else
         Dateiname = (char *)NULL;
   }
   else if (strncmp(CS2_CFG_GBS_STAT, Name, strlen(CS2_CFG_GBS_STAT)) == 0)
   {
      GbsStatSaveGbsStatSr2(ZentraleGetGleisbild(Data));
      Dateiname = CS2_FILE_STRING_STATUS_GLEISBILD;
   }
   else if (strncmp(CS2_CFG_GBS, Name, strlen(CS2_CFG_GBS)) == 0)
      Dateiname = CS2_FILE_STRING_GLEISBILD;
   else if (strncmp(CS2_CFG_FS, Name, strlen(CS2_CFG_FS)) == 0)
      Dateiname = CS2_FILE_STRING_FAHRSTRASSE;
   else if (strncmp(CS2_CFG_MAG_STAT, Name, strlen(CS2_CFG_MAG_STAT)) == 0)
   {
      MagStatusSaveMagStatusSr2(ZentraleGetMagnetartikel(Data));
      Dateiname = CS2_FILE_STRING_STATUS_MAGNETARTIKEL;
   }
   else if (strncmp(CS2_CFG_FS_STAT, Name, strlen(CS2_CFG_FS_STAT)) == 0)
   {
      FsStatSaveFsStatSr2(ZentraleGetFahrstrasse(Data));
      Dateiname = CS2_FILE_STRING_STATUS_FAHRSTRASSE;
   }
   else
      Dateiname = (char *)NULL;
   if (Dateiname != (char *)NULL)
   {
      if (ZentraleGetVerbose(Data))
         printf("FSM: request file %s\n", Dateiname);
      strcpy(FullPath, ZentraleGetLocPath(Data));
      if (FullPath[strlen(FullPath)] != '/')
         strcat(FullPath, "/");
      strcat(FullPath, Dateiname);
      LokomotiveDatei = fopen(FullPath, "r");
      if (LokomotiveDatei != (FILE *)NULL)
      {
         if (stat(FullPath, &attribut) == 0)
         {
            LokomotiveDaten = (char *)malloc(attribut.st_size);
            if (LokomotiveDaten != (char *)NULL)
            {
               FileLength = fread(LokomotiveDaten, 1, attribut.st_size,
                                  LokomotiveDatei);
               if (FileLength > 0)
               {
                  ZFileInit(ZentraleGetPackedCs2File(Data), LokomotiveDaten,
                            FileLength);
                  if (ZFileCompress(ZentraleGetPackedCs2File(Data)))
                  {
                     MrIpcSetCanResponse(CmdFrame, 1);
                     Hash = Cs2CalcHash(ZentraleGetUid(Data));
                     MrIpcSetCanHash(CmdFrame, Cs2CalcHash(ZentraleGetUid(Data)));
                     MrIpcSetReceiverSocket(CmdFrame,
                                            MrIpcGetSenderSocket(CmdFrame));
                     MrIpcSetSenderSocket(CmdFrame, MR_IPC_SOCKET_ALL);
                     MrIpcSend(ZentraleGetClientSock(Data), CmdFrame);
                     MrIpcSetCanHash(CmdFrame, Hash);
                     MrIpcHlCfgHeaderRequest(&Cmd, ZentraleGetUid(Data),
                                             MrIpcGetReceiverSocket(CmdFrame),
                                             ZFileGetLength(ZentraleGetPackedCs2File(Data)),
                                             ZFileGetCrc(ZentraleGetPackedCs2File(Data)));
                     MrIpcSend(ZentraleGetClientSock(Data), &Cmd);

                     i = 0;
                     while (i < ZFileGetFrameLength(ZentraleGetPackedCs2File(Data)))
                     {
                        j = 0;
                        while (j < 8)
                        {
                           Name[j] = ZFileGetBuffer(ZentraleGetPackedCs2File(Data))[i + j];
                           j++;
                        }
                        MrIpcHlCfgDataRequest(&Cmd, ZentraleGetUid(Data),
                                              MrIpcGetReceiverSocket(CmdFrame),
                                              Name);
                        MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
                        i += 8;
                     }
                  }
                  else if (ZentraleGetVerbose(Data))
                     printf("FSM: error in compress file %s\n", Dateiname);
                  ZFileExit(ZentraleGetPackedCs2File(Data));
               }
               else if (ZentraleGetVerbose(Data))
                  printf("FSM: error in read file %s\n", Dateiname);
               free(LokomotiveDaten);
            }
            else if (ZentraleGetVerbose(Data))
               printf("FSM: error in allocate file buffer for %s\n", Dateiname);
            fclose(LokomotiveDatei);
         }
         else if (ZentraleGetVerbose(Data))
            printf("FSM: error in get file size %s\n", FullPath);
      }
      else if (ZentraleGetVerbose(Data))
         printf("FSM: error in open file %s\n", FullPath);
   }
   if (ZentraleGetVerbose(Data))
      printf("FSM: new state %d\n",STATE_NO_CHANGE);
   return(STATE_NO_CHANGE);
}

static BOOL DoCfgHeader(ZentraleStruct *Data, MrIpcCmdType *CmdFrame,
                        BOOL IsCompressed)
{  unsigned long Length;
   unsigned int Crc;

   MrIpcCmdGetCfgHeader(CmdFrame, &Length, &Crc);
   if (ZentraleGetVerbose(Data))
      printf("DoCfgHeader: CFG data length %lu, crc %d\n", Length, Crc);
   if (!Cs2CfgDataStart(ZentraleGetCs2CfgDaten(Data), MrIpcGetCanHash(CmdFrame),
                        Length, IsCompressed))
   {
      if (ZentraleGetVerbose(Data))
         puts("DoCfgHeader: error");
      return(FALSE);
   }
   else
   {
      return(TRUE);
   }
}

static BOOL HandleCfgLoknamen(ZentraleStruct *Data, MrIpcCmdType *CmdFrame)
{  Cs2parser *LokParser;
   int LineInfo, Paragraph;
   BOOL CfgIsCorrupt;

   Paragraph = PARAGRAPH_UNDEFINED;
   if (ZentraleGetVerbose(Data))
      printf("FSM: evaluate lokname cfg\n%s",
             Cs2CfgDataGetBuf(ZentraleGetCs2CfgDaten(Data), MrIpcGetCanHash(CmdFrame)));
   Cs2CfgDataGetBuf(ZentraleGetCs2CfgDaten(Data), MrIpcGetCanHash(CmdFrame))[Cs2CfgDataGetLength(ZentraleGetCs2CfgDaten(Data), MrIpcGetCanHash(CmdFrame))] = '\0';
   LokParser = Cs2pCreate();
   Cs2pInit(LokParser, PARSER_TYPE_LOKNAMEN,
            Cs2CfgDataGetBuf(ZentraleGetCs2CfgDaten(Data), MrIpcGetCanHash(CmdFrame)),
            Cs2CfgDataGetHaveRead(ZentraleGetCs2CfgDaten(Data), MrIpcGetCanHash(CmdFrame)));
   Cs2pSetVerbose(LokParser, FALSE);
   CfgIsCorrupt = FALSE;
   do {
      LineInfo = Cs2pParse(LokParser);
      switch (LineInfo)
      {
         case PARSER_ERROR:
            if (ZentraleGetVerbose(Data))
               puts("ERROR in lok cfg");
            CfgIsCorrupt = TRUE;
            break;
         case PARSER_EOF:
            if (ZentraleGetVerbose(Data))
               puts("end of lok cfg");
            break;
         case PARSER_PARAGRAPH:
            if (ZentraleGetVerbose(Data))
               printf("new paragraph %s in lok cfg\n",
                      Cs2pGetName(LokParser));
            switch (Cs2pGetSubType(LokParser))
            {
               case PARSER_PARAGRAPH_LOK:
                  if (ZentraleGetVerbose(Data))
                     puts("lok paragraph in lok cfg");
                  Paragraph = PARAGRAPH_LOK;
                  break;
               case PARSER_PARAGRAPH_NUMLOKS:
                  if (ZentraleGetVerbose(Data))
                     puts("numloks paragraph in lok cfg");
                  Paragraph = PARAGRAPH_NUMLOKS;
                  break;
            }
            break;
         case PARSER_VALUE:
            if (ZentraleGetVerbose(Data))
               printf("new value %s=%s in lok cfg\n",
                      Cs2pGetName(LokParser), Cs2pGetValue(LokParser));
            switch (Cs2pGetSubType(LokParser))
            {
               case PARSER_VALUE_NAME:
                  if (ZentraleGetVerbose(Data))
                     printf("lok name %d in lok cfg\n", ZentraleGetActualIndex(Data));
                  if (Paragraph == PARAGRAPH_LOK &&
                      ZentraleGetActualIndex(Data) < ZentraleGetNumLoks(Data))
                  {
                     ZentraleSetLokNamenNr(Data,
                                           ZentraleGetActualIndex(Data),
                                           Cs2pGetValue(LokParser));
                     ZentraleSetActualIndex(Data, ZentraleGetActualIndex(Data) + 1);
                  }
                  break;
               case PARSER_VALUE_WERT:
                  if (ZentraleGetVerbose(Data))
                     puts("number of loks in lok cfg");
                  if (Paragraph == PARAGRAPH_NUMLOKS)
                  {
                     ZentraleSetNumLoks(Data, atoi(Cs2pGetValue(LokParser)));
                     if (ZentraleGetVerbose(Data))
                        printf("number of loks in lok cfg is %d\n",
                               ZentraleGetNumLoks(Data));
                     if (ZentraleGetMaxLoks(Data) < ZentraleGetNumLoks(Data))
                     {
                        ZentraleSetMaxLoks(Data, ZentraleGetNumLoks(Data));
                        ZentraleSetLokNamen(Data, realloc(ZentraleGetLokNamen(Data),
                                                          ZentraleGetNumLoks(Data) * sizeof(ZentraleLokName)));
                        if (ZentraleGetVerbose(Data))
                           printf("new number of loks in lok cfg is %d\n",
                                  ZentraleGetNumLoks(Data));
                     }
                  }
                  break;
            }
            break;
      }
   } while ((LineInfo != PARSER_EOF) && !CfgIsCorrupt);
   Cs2pDestroy(LokParser);
   if (ZentraleGetActualIndex(Data) < ZentraleGetNumLoks(Data))
   {
      return(FALSE);
   }
   else
   {
      LokMarkAllDeleted(ZentraleGetLoks(Data));
      return(TRUE);
   }
}

static void HandleCfgLokliste(ZentraleStruct *Data, MrIpcCmdType *CmdFrame)
{  Cs2parser *LokParser;
   int LineInfo;
   BOOL CfgIsCorrupt;

   if (ZentraleGetVerbose(Data))
      printf("FSM: evaluate lokliste cfg\n%s",
             Cs2CfgDataGetBuf(ZentraleGetCs2CfgDaten(Data), MrIpcGetCanHash(CmdFrame)));
   Cs2CfgDataGetBuf(ZentraleGetCs2CfgDaten(Data), MrIpcGetCanHash(CmdFrame))[Cs2CfgDataGetLength(ZentraleGetCs2CfgDaten(Data), MrIpcGetCanHash(CmdFrame))] = '\0';
   LokParser = Cs2pCreate();
   Cs2pInit(LokParser, PARSER_TYPE_LOKLISTE,
            Cs2CfgDataGetBuf(ZentraleGetCs2CfgDaten(Data), MrIpcGetCanHash(CmdFrame)),
            Cs2CfgDataGetHaveRead(ZentraleGetCs2CfgDaten(Data), MrIpcGetCanHash(CmdFrame)));
   Cs2pSetVerbose(LokParser, FALSE);
   ZentraleSetActualIndex(Data, 0);
   CfgIsCorrupt = FALSE;
   do {
      LineInfo = Cs2pParse(LokParser);
      switch (LineInfo)
      {
         case PARSER_ERROR:
            if (ZentraleGetVerbose(Data))
               puts("ERROR in lokliste");
            CfgIsCorrupt = TRUE;
            break;
         case PARSER_EOF:
            if (ZentraleGetVerbose(Data))
               puts("end of lokliste");
            break;
         case PARSER_PARAGRAPH:
            if (ZentraleGetVerbose(Data))
               printf("new paragraph %s in lokliste\n",
                      Cs2pGetName(LokParser));
            switch (Cs2pGetSubType(LokParser))
            {
               case PARSER_PARAGRAPH_LOKLISTE:
                  if (ZentraleGetVerbose(Data))
                     puts("lokliste paragraph in lokliste");
                  break;
            }
            break;
         case PARSER_VALUE:
            if (ZentraleGetVerbose(Data))
               printf("new value %s=%s in lokliste\n",
                      Cs2pGetName(LokParser), Cs2pGetValue(LokParser));
            switch (Cs2pGetSubType(LokParser))
            {
               case PARSER_VALUE_DV:
                  if (ZentraleGetVerbose(Data))
                     printf("dv %d in lokliste\n", atoi(Cs2pGetValue(LokParser)));
                  break;
               case PARSER_VALUE_IDX:
                  if (ZentraleGetVerbose(Data))
                     printf("idx %d in lokliste\n", atoi(Cs2pGetValue(LokParser)));
                  if (strlen(ZentraleGetLokNamenNr(Data, ZentraleGetActualIndex(Data))) != 0)
                  {
                     ZentraleSetActualIndex(Data,
                                            ZentraleGetActualIndex(Data) + 1);
                  }
                  break;
               case PARSER_VALUE_LLINDEX:
                  if (ZentraleGetVerbose(Data))
                     printf("llindex %d in lokliste\n", atoi(Cs2pGetValue(LokParser)));
                  break;
               case PARSER_VALUE_NAME:
                  if (ZentraleGetVerbose(Data))
                     printf("lok name >%s< in lokliste\n", Cs2pGetValue(LokParser));
                  ZentraleSetLokNamenNr(Data,
                                        ZentraleGetActualIndex(Data),
                                        Cs2pGetValue(LokParser));
                  break;
               case PARSER_VALUE_CRC:
                  if (ZentraleGetVerbose(Data))
                     printf("crc %d in lokliste\n", atoi(Cs2pGetValue(LokParser)));
                  break;
            }
            break;
      }
   } while ((LineInfo != PARSER_EOF) && !CfgIsCorrupt);
   Cs2pDestroy(LokParser);
   ZentraleSetNumLoks(Data, ZentraleGetActualIndex(Data) + 1);
   LokMarkAllDeleted(ZentraleGetLoks(Data));
}

static BOOL HandleCfgLokinfo(ZentraleStruct *Data, MrIpcCmdType *CmdFrame)
{  Cs2parser *LokParser;
   int LineInfo, FktIndex;
   BOOL CfgIsCorrupt;

   if (ZentraleGetVerbose(Data))
      printf("FSM: LOKINFO CFG data, aktuell %lu von %lu\n",
             Cs2CfgDataGetHaveRead(ZentraleGetCs2CfgDaten(Data), MrIpcGetCanHash(CmdFrame)),
             Cs2CfgDataGetLength(ZentraleGetCs2CfgDaten(Data), MrIpcGetCanHash(CmdFrame)));
   LokInfoInit(ZentraleGetActualLok(Data));
   FktIndex = -1;
   Cs2CfgDataGetBuf(ZentraleGetCs2CfgDaten(Data), MrIpcGetCanHash(CmdFrame))[Cs2CfgDataGetHaveRead(ZentraleGetCs2CfgDaten(Data), MrIpcGetCanHash(CmdFrame))] = '\0';
   LokParser = Cs2pCreate();
   Cs2pInit(LokParser, PARSER_TYPE_LOKINFO,
            Cs2CfgDataGetBuf(ZentraleGetCs2CfgDaten(Data), MrIpcGetCanHash(CmdFrame)),
            Cs2CfgDataGetHaveRead(ZentraleGetCs2CfgDaten(Data), MrIpcGetCanHash(CmdFrame)));
   Cs2pSetVerbose(LokParser, FALSE);
   CfgIsCorrupt = FALSE;
   do {
      LineInfo = Cs2pParse(LokParser);
      switch (LineInfo)
      {
         case PARSER_ERROR:
            if (ZentraleGetVerbose(Data))
               puts("ERROR in lok info cfg");
            CfgIsCorrupt = TRUE;
            break;
         case PARSER_EOF:
            if (ZentraleGetVerbose(Data))
               puts("end of lok info cfg");
            break;
         case PARSER_PARAGRAPH:
            if (ZentraleGetVerbose(Data))
               printf("new paragraph %s in lok info cfg\n",
                      Cs2pGetName(LokParser));
            switch (Cs2pGetSubType(LokParser))
            {
               case PARSER_PARAGRAPH_LOKOMOTIVE:
                  if (ZentraleGetVerbose(Data))
                     puts("lokomotive paragraph in lok cfg");
                  break;
            }
            break;
         case PARSER_VALUE:
            if (ZentraleGetVerbose(Data))
               printf("new value %s=%s in lok cfg\n",
                      Cs2pGetName(LokParser), Cs2pGetValue(LokParser));
            switch (Cs2pGetSubType(LokParser))
            {
               case PARSER_VALUE_LOK:
                  if (ZentraleGetVerbose(Data))
                     puts("neuer lok Eintrag");
                  break;
               case PARSER_VALUE_DV:
                  if (ZentraleGetVerbose(Data))
                     puts("lok dv");
                 break;
               case PARSER_VALUE_UID:
                  if (ZentraleGetVerbose(Data))
                     puts("lok uid");
                  LokInfoSetUid(ZentraleGetActualLok(Data),
                                strtoul(Cs2pGetValue(LokParser),
                                        NULL, 0));
                  break;
               case PARSER_VALUE_NAME:
                  if (ZentraleGetVerbose(Data))
                     puts("lok name");
                  LokInfoSetName(ZentraleGetActualLok(Data),
                                 Cs2pGetValue(LokParser));
                  break;
               case PARSER_VALUE_ADRESSE:
                  if (ZentraleGetVerbose(Data))
                     puts("lok adresse");
                  LokInfoSetAdresse(ZentraleGetActualLok(Data),
                                    strtoul(Cs2pGetValue(LokParser),
                                            NULL, 0));
                  break;
               case PARSER_VALUE_TYP:
                  if (Cs2pGetLevel(LokParser) == 1)
                  {
                     if (ZentraleGetVerbose(Data))
                        puts("lok typ");
                     LokInfoSetTyp(ZentraleGetActualLok(Data),
                                   Cs2pGetValue(LokParser));
                  }
                  else if (Cs2pGetLevel(LokParser) == 2)
                  {
                     if (ZentraleGetVerbose(Data))
                        puts("lok function typ");
                     LokInfoSetFktTyp(ZentraleGetActualLok(Data), FktIndex,
                                      strtoul(Cs2pGetValue(LokParser),
                                              NULL, 0));
                  }
                  break;
               case PARSER_VALUE_MFXUID:
                  if (ZentraleGetVerbose(Data))
                     puts("lok mfxuid");
                  LokInfoSetMfxUid(ZentraleGetActualLok(Data),
                                   strtoul(Cs2pGetValue(LokParser),
                                           NULL, 0));
                  break;
               case PARSER_VALUE_SYMBOL:
                  if (ZentraleGetVerbose(Data))
                     puts("lok symbol");
                  LokInfoSetSymbol(ZentraleGetActualLok(Data),
                                   strtoul(Cs2pGetValue(LokParser),
                                           NULL, 0));
                  break;
               case PARSER_VALUE_AV:
                  if (ZentraleGetVerbose(Data))
                     puts("lok av");
                  LokInfoSetAv(ZentraleGetActualLok(Data),
                               strtoul(Cs2pGetValue(LokParser),
                                       NULL, 0));
                  break;
               case PARSER_VALUE_BV:
                  if (ZentraleGetVerbose(Data))
                     puts("lok bv");
                  LokInfoSetBv(ZentraleGetActualLok(Data),
                               strtoul(Cs2pGetValue(LokParser),
                                       NULL, 0));
                  break;
               case PARSER_VALUE_VOLUME:
                  if (ZentraleGetVerbose(Data))
                     puts("lok volume");
                  LokInfoSetVolume(ZentraleGetActualLok(Data),
                                   strtoul(Cs2pGetValue(LokParser),
                                           NULL, 0));
                  break;
               case PARSER_VALUE_VELOCITY:
                  if (ZentraleGetVerbose(Data))
                     puts("lok velocity");
                  LokInfoSetVelocity(ZentraleGetActualLok(Data),
                                     strtoul(Cs2pGetValue(LokParser),
                                             NULL, 0));
                  break;
               case PARSER_VALUE_RICHTUNG:
                  if (ZentraleGetVerbose(Data))
                     puts("lok richtung");
                  LokInfoSetRichtung(ZentraleGetActualLok(Data),
                                     strtoul(Cs2pGetValue(LokParser),
                                             NULL, 0));
                  break;
               case PARSER_VALUE_VMAX:
                  if (ZentraleGetVerbose(Data))
                     puts("lok vmax");
                  LokInfoSetVmax(ZentraleGetActualLok(Data),
                                 strtoul(Cs2pGetValue(LokParser),
                                         NULL, 0));
                  break;
               case PARSER_VALUE_VMIN:
                  if (ZentraleGetVerbose(Data))
                     puts("lok vmin");
                  LokInfoSetVmin(ZentraleGetActualLok(Data),
                                 strtoul(Cs2pGetValue(LokParser),
                                         NULL, 0));
                  break;
               case PARSER_VALUE_FKT:
                  if (ZentraleGetVerbose(Data))
                     puts("lok fkt");
                  FktIndex++;
                  break;
               case PARSER_VALUE_DAUER:
                  if (ZentraleGetVerbose(Data))
                     puts("lok function dauer");
                  LokInfoSetFktDauer(ZentraleGetActualLok(Data), FktIndex,
                                     strtoul(Cs2pGetValue(LokParser),
                                             NULL, 0));
                  break;
               case PARSER_VALUE_WERT:
                  if (ZentraleGetVerbose(Data))
                     puts("lok function wert");
                  LokInfoSetFktWert(ZentraleGetActualLok(Data), FktIndex,
                                    strtoul(Cs2pGetValue(LokParser),
                                            NULL, 0));
                  break;
            }
            break;
      }
   } while ((LineInfo != PARSER_EOF) && !CfgIsCorrupt);
   if (!CfgIsCorrupt)
   {
      Cs2pDestroy(LokParser);
      LokInsert(ZentraleGetLoks(Data), ZentraleGetActualLok(Data));
      ZentraleSetActualIndex(Data, ZentraleGetActualIndex(Data) + 1);
   }
   if (ZentraleGetActualIndex(Data) < ZentraleGetNumLoks(Data))
   {
      if (ZentraleGetVerbose(Data))
         printf("%d loks from %d loks\n",
                ZentraleGetActualIndex(Data),
                ZentraleGetNumLoks(Data));
      return(FALSE);
   }
   else
   {
      if (ZentraleGetVerbose(Data))
         printf("save lokomotive.cs2\n");
      LokSaveLokomotiveCs2(ZentraleGetLoks(Data));
      return(TRUE);
   }
}

static void HandleCfgUnCompressedData(ZentraleStruct *Data, MrIpcCmdType *CmdFrame)
{  Cs2parser *Parser;

   if (ZentraleGetVerbose(Data))
      puts("handle uncomptressed cfg data");
   Parser = Cs2pCreate();
   Cs2pInit(Parser, PARSER_TYPE_HEADER_CS2,
            Cs2CfgDataGetBuf(ZentraleGetCs2CfgDaten(Data), MrIpcGetCanHash(CmdFrame)),
            Cs2CfgDataGetHaveRead(ZentraleGetCs2CfgDaten(Data), MrIpcGetCanHash(CmdFrame)));
   Cs2pSetVerbose(Parser, FALSE);
   if (Cs2pParse(Parser) == PARSER_PARAGRAPH)
   {
      switch (Cs2pGetSubType(Parser))
      {
         case PARSER_PARAGRAPH_LOK:
         case PARSER_PARAGRAPH_NUMLOKS:
            HandleCfgLoknamen(Data, CmdFrame);
            CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
            CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_LOKNAME);
            break;
         case PARSER_PARAGRAPH_LOKOMOTIVE:
            HandleCfgLokinfo(Data, CmdFrame);
            CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
            CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_LOKINFO);
            break;
      }
      Cs2pDestroy(Parser);
   }
}

static void HandleCfgCompressedData(ZentraleStruct *Data, MrIpcCmdType *CmdFrame)
{  Cs2parser *Parser;

   if (ZentraleGetVerbose(Data))
      printf("FSM: inflate compressed cfg\n");
   ZFileInit(ZentraleGetPackedCs2File(Data),
             Cs2CfgDataGetBuf(ZentraleGetCs2CfgDaten(Data), MrIpcGetCanHash(CmdFrame)),
             Cs2CfgDataGetHaveRead(ZentraleGetCs2CfgDaten(Data), MrIpcGetCanHash(CmdFrame)));
   if (ZFileUnCompress(ZentraleGetPackedCs2File(Data)))
   {
      Parser = Cs2pCreate();
      Cs2pInit(Parser, PARSER_TYPE_HEADER_CS2,
               (char *)ZFileGetBuffer(ZentraleGetPackedCs2File(Data)),
               ZFileGetLength(ZentraleGetPackedCs2File(Data)));
      Cs2pSetVerbose(Parser, FALSE);
      if (Cs2pParse(Parser) == PARSER_PARAGRAPH)
      {
         switch (Cs2pGetSubType(Parser))
         {
            case PARSER_PARAGRAPH_LOKOMOTIVE:
               if (LokParseLokomotiveCs2(ZentraleGetLoks(Data),
                                         (char *)ZFileGetBuffer(ZentraleGetPackedCs2File(Data)),
                                                 ZFileGetLength(ZentraleGetPackedCs2File(Data))))
               {
                  LokSaveLokomotiveCs2(ZentraleGetLoks(Data));
               }
               else
               {
                  LokClear(ZentraleGetLoks(Data));
               }
               break;
            case PARSER_PARAGRAPH_GLEISBILD:
               {  DIR *d;
                  struct dirent *dir;
                  char GleisbildPageDir[256], GleisbildPageFullName[256];

                  GleisbildClear(ZentraleGetGleisbild(Data));
                  strcpy(GleisbildPageDir, ZentraleGetLocPath(Data));
                  if (GleisbildPageDir[strlen(GleisbildPageDir) - 1] != '/')
                     strcat(GleisbildPageDir, "/");
                  strcat(GleisbildPageDir, CS2_GLEISBILD_PAGE_SUBDIR);
                  d = opendir(GleisbildPageDir);
                  if (d)
                  {
                     while ((dir = readdir(d)) != NULL)
                     {
                        if ((strcmp(dir->d_name, ".") != 0) &&
                            (strcmp(dir->d_name, "..") != 0))
                        {
                           strcpy(GleisbildPageFullName, GleisbildPageDir);
                           if (GleisbildPageFullName[strlen(GleisbildPageFullName) - 1] != '/')
                              strcat(GleisbildPageFullName, "/");
                           strcat(GleisbildPageFullName, dir->d_name);
                           remove(GleisbildPageFullName);
                        }
                     }
                     closedir(d);
                  }
                  if (GleisbildGetNumPages(ZentraleGetGleisbild(Data)) > 0)
                  {  int i;

                     for (i=0; i<GleisbildGetNumPages(ZentraleGetGleisbild(Data)); i++)
                     {
                        GleisbildPageExit(ZentraleGetNrGleisPages(Data, i));
                        GleisbildPageDestroy(ZentraleGetNrGleisPages(Data, i));
                     }
                     free(ZentraleGetGleisPages(Data));
                  }
                  if (GleisbildParseGleisbildCs2(ZentraleGetGleisbild(Data),
                                                 (char *)ZFileGetBuffer(ZentraleGetPackedCs2File(Data)),
                                                 ZFileGetLength(ZentraleGetPackedCs2File(Data))))
                  {
                     GleisbildSaveGleisbildCs2(ZentraleGetGleisbild(Data));
                     if (GleisbildGetNumPages(ZentraleGetGleisbild(Data)) > 0)
                     {  int i;

                        ZentraleSetGleisPages(Data,
                                              (GleisbildPageStruct **)malloc(sizeof(GleisbildPageStruct *) *
                                                                    GleisbildGetNumPages(ZentraleGetGleisbild(Data))));
                        for (i=0; i<GleisbildGetNumPages(ZentraleGetGleisbild(Data)); i++)
                        {
                           ZentraleSetNrGleisPages(Data, i,
                                                   (GleisbildPageStruct *)NULL);
                        }
                     }
                  }
                  else
                  {
                     GleisbildClear(ZentraleGetGleisbild(Data));
                  }
               }
               break;
            case PARSER_PARAGRAPH_MAGNETARTIKEL:
               if (MagnetartikelParseMagnetartikelCs2(ZentraleGetMagnetartikel(Data),
                                                      (char *)ZFileGetBuffer(ZentraleGetPackedCs2File(Data)),
                                                      ZFileGetLength(ZentraleGetPackedCs2File(Data))))
               {
                  MagnetartikelSaveMagnetartikelCs2(ZentraleGetMagnetartikel(Data));
               }
               else
               {
                  MagnetartikelClear(ZentraleGetMagnetartikel(Data));
               }
               break;
            case PARSER_PARAGRAPH_FAHRSTRASSEN:
               if (FahrstrasseParseFahrstrasseCs2(ZentraleGetFahrstrasse(Data),
                                                  (char *)ZFileGetBuffer(ZentraleGetPackedCs2File(Data)),
                                                  ZFileGetLength(ZentraleGetPackedCs2File(Data))))
               {
                  FahrstrasseSaveFahrstrasseCs2(ZentraleGetFahrstrasse(Data));
               }
               else
               {
                  FahrstrasseClear(ZentraleGetFahrstrasse(Data));
               }
               break;
            case PARSER_PARAGRAPH_GLEISBILDSEITE:
               {
                  GleisbildPageStruct *NewPage;
                  char *GleisbildName;

                  NewPage = GleisbildPageCreate();
                  if (NewPage != (GleisbildPageStruct *)NULL)
                  {
                     GleisbildPageInit(NewPage, ZentraleGetLocPath(Data), "", 0);
                     if (GleisbildPageParseGleisbildPageCs2(NewPage,
                                                            (char *)ZFileGetBuffer(ZentraleGetPackedCs2File(Data)),
                                                            ZFileGetLength(ZentraleGetPackedCs2File(Data))))
                     {
                        GleisbildPageSetGleisbildPageFilePath(NewPage,
                                                              ZentraleGetLocPath(Data));
                        GleisbildName = malloc(strlen(GleisbildInfoGetName(GleisbildSearch(ZentraleGetGleisbild(Data),
                                                                           GleisbildPageStructGetPage(NewPage)))) +
                                               strlen(CS2_FILE_EXTENSION) + 1);
                        if (GleisbildName != (char *)NULL)
                        {
                           strcpy(GleisbildName,
                                  GleisbildInfoGetName(GleisbildSearch(ZentraleGetGleisbild(Data),
                                                       GleisbildPageStructGetPage(NewPage))));
                           strcat(GleisbildName, CS2_FILE_EXTENSION);
                           GleisbildPageSetGleisbildName(NewPage,
                                                         GleisbildName);
                           GleisbildPageSaveGleisbildPageCs2(NewPage);
                           ZentraleSetNrGleisPages(Data,
                                                   GleisbildPageStructGetPage(NewPage),
                                                   NewPage);
                        }
                     }
                     else
                     {
                        GleisbildPageClear(NewPage);
                        GleisbildPageExit(NewPage);
                        GleisbildPageDestroy(NewPage);
                     }
                  }
               }
               break;
         }
      }
      Cs2pDestroy(Parser);
   }
   ZFileExit(ZentraleGetPackedCs2File(Data));
}

static BOOL DoCfgData(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;
   char Buf[8];

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetCfgData(CmdFrame, Buf);
   if (Cs2CfgDataNextBuf(ZentraleGetCs2CfgDaten(Data),
                         MrIpcGetCanHash(CmdFrame), Buf))
   {
      if (Cs2CfgDataGetIsCompressed(ZentraleGetCs2CfgDaten(Data),
                                    MrIpcGetCanHash(CmdFrame)))
      {
         if (ZentraleGetVerbose(Data))
            printf("FSM: handle compressed cfg\n");
         HandleCfgCompressedData(Data, CmdFrame);
      }
      else
      {
         if (ZentraleGetVerbose(Data))
            printf("FSM: handle uncompressed cfg\n");
         HandleCfgUnCompressedData(Data, CmdFrame);
      }
      Cs2CfgDataEnd(ZentraleGetCs2CfgDaten(Data), MrIpcGetCanHash(CmdFrame));
      if (ZentraleGetVerbose(Data))
         puts("FSM: cfg finished");
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
      return(TRUE);
   }
   else
   {
      if (ZentraleGetVerbose(Data))
         puts("FSM: cfg wait for next packet");
      return(FALSE);
   }
}

static int HandleCfgData(void *Priv, void *SignalData)
{
   DoCfgData(Priv, SignalData);
   return(STATE_NORMAL);
}

static int HandleCvrData(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;
   char Buf[8];
   Cs2parser *Parser;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetCfgData(CmdFrame, Buf);
   if (Cs2CfgDataNextBuf(ZentraleGetCs2CfgDaten(Data),
                         MrIpcGetCanHash(CmdFrame), Buf))
   {
      if (ZentraleGetVerbose(Data))
         printf("FSM: inflate compressed cfg\n");
      ZFileInit(ZentraleGetPackedCs2File(Data),
                Cs2CfgDataGetBuf(ZentraleGetCs2CfgDaten(Data), MrIpcGetCanHash(CmdFrame)),
                Cs2CfgDataGetHaveRead(ZentraleGetCs2CfgDaten(Data), MrIpcGetCanHash(CmdFrame)));
      if (ZFileUnCompress(ZentraleGetPackedCs2File(Data)))
      {
         Parser = Cs2pCreate();
         Cs2pInit(Parser, PARSER_TYPE_HEADER_CS2,
                  (char *)ZFileGetBuffer(ZentraleGetPackedCs2File(Data)),
                  ZFileGetLength(ZentraleGetPackedCs2File(Data)));
         Cs2pSetVerbose(Parser, FALSE);
         if (Cs2pParse(Parser) == PARSER_PARAGRAPH)
         {
            switch (Cs2pGetSubType(Parser))
            {
               case PARSER_PARAGRAPH_LOKSTATUS:
                  LokStatusParseLokomotiveSr2(ZentraleGetLoks(Data),
                                              (char *)ZFileGetBuffer(ZentraleGetPackedCs2File(Data)),
                                              ZFileGetLength(ZentraleGetPackedCs2File(Data)));
                  LokStatusSaveLokomotiveSr2(ZentraleGetLoks(Data));
                  break;
               case PARSER_PARAGRAPH_GLEISBILD:
                  GbsStatParseGbsStatSr2(ZentraleGetGleisbild(Data),
                                         (char *)ZFileGetBuffer(ZentraleGetPackedCs2File(Data)),
                                         ZFileGetLength(ZentraleGetPackedCs2File(Data)));
                  GbsStatSaveGbsStatSr2(ZentraleGetGleisbild(Data));
                  break;
               case PARSER_PARAGRAPH_MAGNETARTIKEL:
                  MagStatusParseMagStatusSr2(ZentraleGetMagnetartikel(Data),
                                             (char *)ZFileGetBuffer(ZentraleGetPackedCs2File(Data)),
                                             ZFileGetLength(ZentraleGetPackedCs2File(Data)));
                  MagStatusSaveMagStatusSr2(ZentraleGetMagnetartikel(Data));
                  break;
               case PARSER_PARAGRAPH_FAHRSTRASSEN:
                  FsStatParseFsStatSr2(ZentraleGetFahrstrasse(Data),
                                       (char *)ZFileGetBuffer(ZentraleGetPackedCs2File(Data)),
                                       ZFileGetLength(ZentraleGetPackedCs2File(Data)));
                  FsStatSaveFsStatSr2(ZentraleGetFahrstrasse(Data));
                  break;
            }
         }
         Cs2pDestroy(Parser);
      }
      ZFileExit(ZentraleGetPackedCs2File(Data));
      Cs2CfgDataEnd(ZentraleGetCs2CfgDaten(Data), MrIpcGetCanHash(CmdFrame));
      if (ZentraleGetVerbose(Data))
         printf("FSM: new state %d\n",STATE_NORMAL);
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
      return(STATE_NORMAL);
   }
   else
   {
      if (ZentraleGetVerbose(Data))
         printf("FSM: new state %d\n",STATE_WAIT_CFG_DATA);
      return(STATE_WAIT_CFG_DATA);
   }
}

static int HandleSCfgDataProxy(void *Priv, void *SignalData,
                               int SignalContinue, int SignalStay,
                               char *CfgFile)
{  ZentraleStruct *Data;
   MrIpcCmdType Cmd;
   int Ret;

   Data = (ZentraleStruct *)Priv;
   Ret = HandleCvrData(Priv, SignalData);
   if (Ret == STATE_WAIT_CFG_DATA)
   {
      if (ZentraleGetVerbose(Data))
         printf("FSM: rewrite new state %d\n",SignalStay);
      return(SignalStay);
   }
   else
   {
      if (CfgFile != (char *)NULL)
      {
         MrIpcHlConfigResponse(&Cmd, ZentraleGetUid(Data), CfgFile);
         MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
      }
      if (ZentraleGetVerbose(Data))
         printf("FSM: rewrite new state %d\n",SignalContinue);
      return(SignalContinue);
   }
}

static int HandleLokCvrDataProxy(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   int NewState;

   Data = (ZentraleStruct *)Priv;
   if ((ZentraleGetSyncMask(Data) & MRSYSTEM_CFG_SYNC_KEYBD) != 0)
      NewState = HandleSCfgDataProxy(Priv, SignalData, STATE_WAIT_MAG_CVR_CFG_HDR,
                                     STATE_WAIT_LOK_CVR_CFG_DATA, CS2_CFG_MAG_STAT);
   else if ((ZentraleGetSyncMask(Data) & MRSYSTEM_CFG_SYNC_LAYOUT) != 0)
      NewState = HandleSCfgDataProxy(Priv, SignalData, STATE_WAIT_GBS_CVR_CFG_HDR,
                                     STATE_WAIT_LOK_CVR_CFG_DATA, CS2_CFG_GBS_STAT);
   else if ((ZentraleGetSyncMask(Data) & MRSYSTEM_CFG_SYNC_MEM) != 0)
      NewState = HandleSCfgDataProxy(Priv, SignalData, STATE_WAIT_FS_CVR_CFG_HDR,
                                     STATE_WAIT_LOK_CVR_CFG_DATA, CS2_CFG_FS_STAT);
   else if ((ZentraleGetSyncMask(Data) & MRSYSTEM_CFG_SYNC_CONTR) != 0)
   {
      NewState = HandleSCfgDataProxy(Priv, SignalData, STATE_NORMAL,
                                     STATE_WAIT_LOK_CVR_CFG_DATA, NULL);
      if (NewState == STATE_NORMAL)
      {
         CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
      }
   }
   else
   {
      NewState = HandleSCfgDataProxy(Priv, SignalData, STATE_NORMAL,
                                     STATE_WAIT_LOK_CVR_CFG_DATA, NULL);
      if (NewState == STATE_NORMAL)
      {
         CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
      }
   }
   return(NewState);
}

static int HandleMagCvrDataProxy(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   int NewState;

   Data = (ZentraleStruct *)Priv;
   if ((ZentraleGetSyncMask(Data) & MRSYSTEM_CFG_SYNC_LAYOUT) != 0)
      NewState = HandleSCfgDataProxy(Priv, SignalData, STATE_WAIT_GBS_CVR_CFG_HDR,
                                     STATE_WAIT_MAG_CVR_CFG_DATA, CS2_CFG_GBS_STAT);
   else if ((ZentraleGetSyncMask(Data) & MRSYSTEM_CFG_SYNC_MEM) != 0)
      NewState = HandleSCfgDataProxy(Priv, SignalData, STATE_WAIT_FS_CVR_CFG_HDR,
                                     STATE_WAIT_MAG_CVR_CFG_DATA, CS2_CFG_FS_STAT);
   else if ((ZentraleGetSyncMask(Data) & MRSYSTEM_CFG_SYNC_CONTR) != 0)
   {
      NewState = HandleSCfgDataProxy(Priv, SignalData, STATE_NORMAL,
                                     STATE_WAIT_MAG_CVR_CFG_DATA, NULL);
      if (NewState == STATE_NORMAL)
      {
         CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
      }
   }
   else
   {
      NewState = HandleSCfgDataProxy(Priv, SignalData, STATE_NORMAL,
                                     STATE_WAIT_MAG_CVR_CFG_DATA, NULL);
      if (NewState == STATE_NORMAL)
      {
         CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
      }
   }
   return(NewState);
}

static int HandleGbsCvrDataProxy(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   int NewState;

   Data = (ZentraleStruct *)Priv;
   if ((ZentraleGetSyncMask(Data) & MRSYSTEM_CFG_SYNC_MEM) != 0)
      NewState = HandleSCfgDataProxy(Priv, SignalData, STATE_WAIT_FS_CVR_CFG_HDR,
                                     STATE_WAIT_GBS_CVR_CFG_DATA, CS2_CFG_FS_STAT);
   else if ((ZentraleGetSyncMask(Data) & MRSYSTEM_CFG_SYNC_CONTR) != 0)
   {
      NewState = HandleSCfgDataProxy(Priv, SignalData, STATE_NORMAL,
                                     STATE_WAIT_GBS_CVR_CFG_DATA, NULL);
      if (NewState == STATE_NORMAL)
      {
         CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
      }
   }
   else
   {
      NewState = HandleSCfgDataProxy(Priv, SignalData, STATE_NORMAL,
                                     STATE_WAIT_GBS_CVR_CFG_DATA, NULL);
      if (NewState == STATE_NORMAL)
      {
         CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
      }
   }
   return(NewState);
}

static int HandleFsCvrDataProxy(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   int NewState;

   Data = (ZentraleStruct *)Priv;
   NewState = HandleSCfgDataProxy(Priv, SignalData, STATE_NORMAL,
                                  STATE_WAIT_FS_CVR_CFG_DATA, NULL);
   if (NewState == STATE_NORMAL)
   {
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
   }
   return(NewState);
}

static void DoPingMember(ZentraleStruct *Data, unsigned int DeviceId)
{  MrIpcCmdType Cmd;

   MrIpcHlMemberResponse(&Cmd, ZentraleGetUid(Data), DeviceId);
   MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
}

static int HandlePing(void *Priv, void *SignalData)
{  ZentraleStruct *Data;

   Data = (ZentraleStruct *)Priv;
   if (ZentraleGetVerbose(Data))
      puts("FSM: answer ping");
   DoPingMember(Data, CS2_DEVID_CS2);
   DoPingMember(Data, CS2_DEVID_CS2GUI);
   if (ZentraleGetVerbose(Data))
      printf("FSM: new state %d\n",STATE_NO_CHANGE);
   return(STATE_NO_CHANGE);
}

static void PingAnswerToS88(ZentraleStruct *Data, CanMemberInfo *CanMember)
{  MrIpcCmdType Cmd;
   unsigned int i;

   if (ZentraleGetShouldWakeUpS88(Data) &&
       ((CanMemberInfoGetUid(CanMember) & 0xffff0000) == S88_UID_PREFIX))
   {
      for (i = 0; i < 3; i++)
      {
         if (ZentraleGetS88BusIdxLength(Data, i) != 0)
         {
            MrIpcHlConfigvalue(&Cmd, ZentraleGetUid(Data),
                               CanMemberInfoGetUid(CanMember), i + 2,
                                ZentraleGetS88BusIdxLength(Data, i));
            MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
         }
      }
      for (i = 0; i < 3; i++)
      {
         if (ZentraleGetS88BusIdxTCycle(Data, i) != 0)
         {
            MrIpcHlConfigvalue(&Cmd, ZentraleGetUid(Data),
                               CanMemberInfoGetUid(CanMember), i + 5,
                               ZentraleGetS88BusIdxTCycle(Data, i));
            MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
         }
      }
   }
}

static int HandleMemberProxy(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;
   unsigned long Uid;
   unsigned int Version, Type;
   CanMemberInfo NewCanMember, *OldCanMember;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetMember(CmdFrame, &Uid, &Version, &Type);
   if (ZentraleGetVerbose(Data))
      printf("FSM: CAN member %lu, version %d, type 0x%x\n", Uid, Version, Type);
   OldCanMember = CanMemberSearch(ZentraleGetCanMember(Data), Uid);
   if (OldCanMember == (CanMemberInfo *)NULL)
   {
      CanMemberInfoSetUid(&NewCanMember, Uid);
      CanMemberInfoSetVersion(&NewCanMember, Version);
      CanMemberInfoSetType(&NewCanMember, Type);
      CanMemberInfoSetConfigQuerried(&NewCanMember, FALSE);
      CanMemberInsert(ZentraleGetCanMember(Data), &NewCanMember);
      PingAnswerToS88(Data, &NewCanMember);
   }
   else
   {
      CanMemberInfoSetIsInvalid(OldCanMember, FALSE);
   }
   if (ZentraleGetVerbose(Data))
      printf("FSM: new state %d\n",STATE_NO_CHANGE);
   return(STATE_NO_CHANGE);
}

static int HandleCanBootldr(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame, Cmd;
   unsigned long Uid;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   if (ZentraleGetVerbose(Data))
      printf("FSM: CAN Bootloader Gebunden %d\n", MrIpcGetCommand(CmdFrame));
   Uid = GetLongFromByteArray(MrIpcGetRawData(CmdFrame));
   if (ZentraleGetShouldWakeUpS88(Data) &&
       (MrIpcGetRawDlc(CmdFrame) == 8) &&
       ((Uid & 0xffff0000) == S88_UID_PREFIX) &&
       (CanMemberSearch(ZentraleGetCanMember(Data), Uid) != (CanMemberInfo *)NULL))
   {
      MrIpcHlCanBootldr(&Cmd, ZentraleGetUid(Data), S88Bootldr[0].Length,
                        S88Bootldr[0].Data);
      MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
      MrIpcHlCanBootldr(&Cmd, ZentraleGetUid(Data), S88Bootldr[1].Length,
                        S88Bootldr[1].Data);
      MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
      MrIpcHlCanBootldr(&Cmd, ZentraleGetUid(Data), S88Bootldr[2].Length,
                        S88Bootldr[2].Data);
      MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
      QueryMembers(Data, FALSE);
   }
   if (ZentraleGetVerbose(Data))
      printf("FSM: new state %d\n",STATE_NO_CHANGE);
   return(STATE_NO_CHANGE);
}

static int HandleCfgDataHeader(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   unsigned long Uid;
   MrIpcCmdType *CmdFrame;
   unsigned int Index, PaketZahl;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetStatusPos(CmdFrame, &Uid, &Index, &PaketZahl);
   if (ZentraleGetVerbose(Data))
      printf("DoStatusHeader: status data uid %lu, index %d num pkts %d\n",
             Uid, Index, PaketZahl);
   if (!Cs2CfgDataStart(ZentraleGetCs2CfgDaten(Data), Uid,
                        PaketZahl * 8, FALSE))
   {
      if (ZentraleGetVerbose(Data))
         puts("HandleCfgDataHeader: error");
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
      return(STATE_NORMAL);
   }
   else
   {
      ZentraleSetActualIndex(Data, Index);
      ZentraleSetActualConfig(Data, Uid);
      return(STATE_WAIT_CFG_DATA_DATA);
   }
}

static int HandleCfgDataFirstData(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;
   char Buf[8];
   unsigned int PaketZahl;
   int NextState;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetCfgData(CmdFrame, Buf);
   if (ZentraleGetVerbose(Data))
      printf("HandleCfgDataFirstData: index %d, uid %ld\n",
             ZentraleGetActualIndex(Data),
             ZentraleGetActualConfig(Data));
   PaketZahl = 30; /* enough? Cs2CfgData needs dyamically alloc */
   if (!Cs2CfgDataStart(ZentraleGetCs2CfgDaten(Data),
                        ZentraleGetActualConfig(Data),
                        PaketZahl * 8, FALSE))
   {
      if (ZentraleGetVerbose(Data))
         puts("HandleCfgDataFirstData: error");
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
      NextState = STATE_NORMAL;
   }
   else
   {
      Cs2CfgDataNextBuf(ZentraleGetCs2CfgDaten(Data),
                        ZentraleGetActualConfig(Data), Buf);
      if (ZentraleGetVerbose(Data))
         puts("FSM: cfg wait for next packet");
      NextState = STATE_WAIT_CFG_DATA_DATA;
   }
   return(NextState);
}

static void ReadStatusEntryFromBuf(char *Buf, unsigned long Len,
                                   CanMemberInfo *CanMember,
                                   unsigned int Index, BOOL Verbose)
{  ConfigStatusInfo *ConfigStatus;
   ConfigMesswertTyp *ConfigMesswert;
   ConfigTyp *Config;

   ConfigStatus = CanMemberInfoGetConfigStatus(CanMember);
   if (Index == 0)
   {
      if (Verbose)
         printf("FSM: handle status index 0\n");
      ConfigStatusGeraetebeschreibung(ConfigStatusInfoGetGeraeteTyp(ConfigStatus),
                                      Buf, Len);
   }
   else if ((Index > 0) &&
            (Index <= ConfigGeraetTypGetNumMesswerte(ConfigStatusInfoGetGeraeteTyp(ConfigStatus))))
   {
      if (Verbose)
         printf("FSM: handle status messwerte index %d\n", Index);
      ConfigMesswert = ConfigMesswertTypCreate();
      if (ConfigMesswert != (ConfigMesswertTyp *)NULL)
      {
         ConfigMesswertTypInit(ConfigMesswert);
         ConfigMesswertTypSetIndex(ConfigMesswert, Index - 1);
         ConfigStatusMesswerte(ConfigMesswert, Buf, Len);
         ConfigStatusInfoAddMesswerte(ConfigStatus, ConfigMesswert);
      }
   }
   else
   {
      if (Verbose)
         printf("FSM: handle status konfigwerte index %d\n",
                Index - ConfigGeraetTypGetNumMesswerte(ConfigStatusInfoGetGeraeteTyp(ConfigStatus)));
      Config = ConfigTypCreate();
      if (Config != (ConfigTyp *)NULL)
      {
         ConfigTypInit(Config);
         ConfigTypSetIndex(Config,
                           Index - ConfigGeraetTypGetNumMesswerte(ConfigStatusInfoGetGeraeteTyp(ConfigStatus)));
         ConfigStatusKonfigwerte(Config, Buf, Len);
         ConfigStatusInfoAddTypen(ConfigStatus, Config);
      }
   }
}

static int HandleCfgDataLateHeader(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   unsigned long Uid;
   MrIpcCmdType *CmdFrame, Cmd;
   unsigned int Index, PaketZahl;
   CanMemberInfo *CanMember;
   int NextState;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetStatusPos(CmdFrame, &Uid, &Index, &PaketZahl);
   if (ZentraleGetVerbose(Data))
      printf("DoStatusLateHeader: status data uid %lu, index %d num pkts %d\n",
             Uid, Index, PaketZahl);
   Cs2CfgDataSetLen(ZentraleGetCs2CfgDaten(Data), Uid, PaketZahl * 8);
   if (Cs2CfgDataAllRead(ZentraleGetCs2CfgDaten(Data), Uid))
   {
      Index = ZentraleGetActualIndex(Data);
      CanMember = CanMemberSearch(ZentraleGetCanMember(Data),
                                  ZentraleGetActualConfig(Data));
      ReadStatusEntryFromBuf(Cs2CfgDataGetBuf(ZentraleGetCs2CfgDaten(Data),
                                              ZentraleGetActualConfig(Data)),
                             Cs2CfgDataGetLen(ZentraleGetCs2CfgDaten(Data),
                                              ZentraleGetActualConfig(Data)),
                             CanMember, Index, ZentraleGetVerbose(Data));
      Cs2CfgDataEnd(ZentraleGetCs2CfgDaten(Data),
                    ZentraleGetActualConfig(Data));
      if (ZentraleGetVerbose(Data))
         printf("FSM: cfg finished index %d\n", Index);
      if ((CanMemberInfoGetConfigStatus(CanMember)!= (ConfigStatusInfo *)NULL) &&
          (Index < (CanMemberGetNumMesswerte(CanMember) +
                    CanMemberGetNumKonfigwerte(CanMember))))
      {
         CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
         ZentraleSetActualIndex(Data, Index + 1);
         MrIpcHlStatusRequest(&Cmd, ZentraleGetUid(Data),
                              ZentraleGetActualConfig(Data),
                              ZentraleGetActualIndex(Data));
         MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
         NextState = STATE_WAIT_CFG_DATA_HDR;
      }
      else
      {
         CanMember = CanMemberSearchNotConfigured(ZentraleGetCanMember(Data));
         if (CanMember != (CanMemberInfo *)NULL)
         {
            CanMemberInfoSetConfigQuerried(CanMember, TRUE);
            CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
            ZentraleSetActualIndex(Data, 0);
            MrIpcHlStatusRequest(&Cmd, ZentraleGetUid(Data),
                                 CanMemberInfoGetUid(CanMember),
                                 ZentraleGetActualIndex(Data));
            MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
            NextState = STATE_WAIT_CFG_DATA_HDR;
         }
         else
         {
            if (ZentraleGetWriteWeb(Data))
            {
               CanMemberSaveMesswerteCs2(ZentraleGetCanMember(Data),
                                         ZentraleGetLocPath(Data));
               CanMemberSaveMesswerteWeb(ZentraleGetCanMember(Data),
                                         ZentraleGetLocPath(Data),
                                         ZentraleGetUid(Data),
                                         ZentraleGetMajorVersion(Data),
                                         ZentraleGetMinorVersion(Data));
            }
            CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
            CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_END_QUERY);
            CanMemberSetIsChanged(ZentraleGetCanMember(Data), FALSE);
            NextState = STATE_NORMAL;
         }
      }
   }
   else
   {
      NextState = STATE_NO_CHANGE;
   }
   return(NextState);
}

static int HandleCfgDataData(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;
   char Buf[8];
   int NextState;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetCfgData(CmdFrame, Buf);
   if (ZentraleGetVerbose(Data))
      printf("HandleCfgDataData: index %d, uid %ld\n",
             ZentraleGetActualIndex(Data),
             ZentraleGetActualConfig(Data));
   if (Cs2CfgDataNextBuf(ZentraleGetCs2CfgDaten(Data),
                         ZentraleGetActualConfig(Data), Buf))
   {
      if (ZentraleGetVerbose(Data))
         puts("FSM: status komplett");
      NextState = STATE_NO_CHANGE;
   }
   else
   {
      if (ZentraleGetVerbose(Data))
         puts("FSM: cfg wait for next packet");
      NextState = STATE_NO_CHANGE;
   }
   return(NextState);
}

static void RequestNextKanal(ZentraleStruct *Data, unsigned long Uid,
                             unsigned int Kanal)
{  MrIpcCmdType Cmd;

   MrIpcHlMesswertRequest(&Cmd, ZentraleGetUid(Data), Uid, Kanal);
   MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
}

static int HandleStatusResponse(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame, Cmd;
   unsigned long Addr;
   unsigned int Index;
   CanMemberInfo *CanMember;
   ConfigMesswertTyp *ConfigMesswert;
   ConfigTyp *Config;
   int NextState;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   if (ZentraleGetVerbose(Data))
      printf("FSM: StatusResponse %d\n", MrIpcGetCommand(CmdFrame));
   MrIpcCmdGetStatusResponse(CmdFrame, &Addr, &Index);
   if (Addr == ZentraleGetActualConfig(Data))
   {
      if (ZentraleGetVerbose(Data))
         puts("end of status data");
      Index = ZentraleGetActualIndex(Data);
      CanMember = CanMemberSearch(ZentraleGetCanMember(Data),
                                  ZentraleGetActualConfig(Data));
      ReadStatusEntryFromBuf(Cs2CfgDataGetBuf(ZentraleGetCs2CfgDaten(Data),
                                              ZentraleGetActualConfig(Data)),
                             Cs2CfgDataGetLen(ZentraleGetCs2CfgDaten(Data),
                                              ZentraleGetActualConfig(Data)),
                             CanMember, Index, ZentraleGetVerbose(Data));
      Cs2CfgDataEnd(ZentraleGetCs2CfgDaten(Data),
                    ZentraleGetActualConfig(Data));
      if (ZentraleGetVerbose(Data))
         printf("FSM: cfg finished index %d\n", Index);
      if ((CanMemberInfoGetConfigStatus(CanMember)!= (ConfigStatusInfo *)NULL) &&
          (Index < (CanMemberGetNumMesswerte(CanMember) +
                    CanMemberGetNumKonfigwerte(CanMember))))
      {
         CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
         ZentraleSetActualIndex(Data, Index + 1);
         MrIpcHlStatusRequest(&Cmd, ZentraleGetUid(Data),
                              ZentraleGetActualConfig(Data),
                              ZentraleGetActualIndex(Data));
         MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
         NextState = STATE_WAIT_CFG_DATA_HDR;
      }
      else if ((CanMemberGetNumMesswerte(CanMember) > 0) ||
               (CanMemberGetNumKonfigwerte(CanMember) > 0))
      {
         if (CanMemberGetNumMesswerte(CanMember) > 0)
         {
            MengeInitIterator(&ZentraleGetActualKonfigWert(Data),
                              ConfigStatusInfoGetMesswerte(CanMemberInfoGetConfigStatus(CanMember)));
            ConfigMesswert = (ConfigMesswertTyp *)MengeFirst(&ZentraleGetActualKonfigWert(Data));
            ZentraleSetActualMesswert(Data, ConfigMesswert);
            ZentraleSetActualKonfig(Data, (ConfigTyp *)NULL);
            RequestNextKanal(Data, CanMemberInfoGetUid(CanMember),
                             ConfigMesswertTypGetAbfragekanalnummer(ConfigMesswert));
            CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
            NextState = STATE_WAIT_SYS_CFG_MESSWERT;
         }
         else if (CanMemberGetNumKonfigwerte(CanMember) > 0)
         {
            MengeInitIterator(&ZentraleGetActualKonfigWert(Data),
                              ConfigStatusInfoGetTypen(CanMemberInfoGetConfigStatus(CanMember)));
            Config = (ConfigTyp *)MengeFirst(&ZentraleGetActualKonfigWert(Data));
            ZentraleSetActualMesswert(Data, (ConfigMesswertTyp *)NULL);
            ZentraleSetActualKonfig(Data, Config);
            switch (ConfigTypGetTyp(Config))
            {
               case CONFIG_TYP_AUSWAHL:
                  RequestNextKanal(Data, CanMemberInfoGetUid(CanMember),
                                   ConfigAuswahlGetKonfigkanalnummer(ConfigTypGetTypAuswahl(Config)));
                  break;
               case CONFIG_TYP_WERT:
                  RequestNextKanal(Data, CanMemberInfoGetUid(CanMember),
                                   ConfigWertGetKonfigurationskanalnummer(ConfigTypGetTypWert(Config)));
                  break;
            }
            CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
            NextState = STATE_WAIT_SYS_CFG_MESSWERT;
         }
      }
      else
      {
         CanMember = CanMemberSearchNotConfigured(ZentraleGetCanMember(Data));
         if (CanMember != (CanMemberInfo *)NULL)
         {
            CanMemberInfoSetConfigQuerried(CanMember, TRUE);
            CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
            ZentraleSetActualConfig(Data, CanMemberInfoGetUid(CanMember));
            ZentraleSetActualIndex(Data, 0);
            MrIpcHlStatusRequest(&Cmd, ZentraleGetUid(Data),
                                 CanMemberInfoGetUid(CanMember), 0);
            MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
            NextState = STATE_WAIT_CFG_DATA_HDR;
         }
         else
         {
            if (ZentraleGetWriteWeb(Data))
            {
               CanMemberSaveMesswerteCs2(ZentraleGetCanMember(Data),
                                         ZentraleGetLocPath(Data));
               CanMemberSaveMesswerteWeb(ZentraleGetCanMember(Data),
                                         ZentraleGetLocPath(Data),
                                         ZentraleGetUid(Data),
                                         ZentraleGetMajorVersion(Data),
                                         ZentraleGetMinorVersion(Data));
            }
            CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
            CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_END_QUERY);
            CanMemberSetIsChanged(ZentraleGetCanMember(Data), FALSE);
            NextState = STATE_NORMAL;
         }
      }
   }
   else
   {
      if (ZentraleGetVerbose(Data))
         puts("FSM: cfg wait for next packet");
      NextState = STATE_NO_CHANGE;
   }
   return(NextState);
}

static int HandleMesswertResponse(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame, Cmd;
   ConfigMesswertTyp *ConfigMesswert;
   ConfigStatusInfo *ConfigStatus;
   ConfigTyp *Config;
   CanMemberInfo *CanMember;
   int NextState;
   unsigned int Kanalnummer, Value;
   unsigned long Uid;
   BOOL RequestNextIndex;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetMesswertResponse(CmdFrame, &Uid, &Kanalnummer, &Value);
   CanMember = CanMemberSearch(ZentraleGetCanMember(Data),
                               ZentraleGetActualConfig(Data));
   ConfigStatus = CanMemberInfoGetConfigStatus(CanMember);
   if (ZentraleGetActualMesswert(Data) != (ConfigMesswertTyp *)NULL)
   {
      ConfigMesswertTypSetAktuellerMesswert(ZentraleGetActualMesswert(Data), Value);
      ConfigMesswert = (ConfigMesswertTyp *)MengeNext(&ZentraleGetActualKonfigWert(Data));
      if (ConfigMesswert != (ConfigMesswertTyp *)NULL)
      {
         ZentraleSetActualKonfig(Data, (ConfigTyp *)NULL);
         ZentraleSetActualMesswert(Data, ConfigMesswert);
         RequestNextKanal(Data, CanMemberInfoGetUid(CanMember),
                          ConfigMesswertTypGetAbfragekanalnummer(ConfigMesswert));
         CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
         RequestNextIndex = FALSE;
         NextState = STATE_NO_CHANGE;
      }
      else if (ConfigGeraetTypGetNumKonfigwerte(ConfigStatusInfoGetGeraeteTyp(ConfigStatus)) > 0)
      {
         ZentraleSetActualMesswert(Data, (ConfigMesswertTyp *)NULL);
         MengeInitIterator(&ZentraleGetActualKonfigWert(Data),
                           ConfigStatusInfoGetTypen(CanMemberInfoGetConfigStatus(CanMember)));
         Config = (ConfigTyp *)MengeFirst(&ZentraleGetActualKonfigWert(Data));
         ZentraleSetActualKonfig(Data, Config);
         switch (ConfigTypGetTyp(Config))
         {
            case CONFIG_TYP_AUSWAHL:
               RequestNextKanal(Data, CanMemberInfoGetUid(CanMember),
                                ConfigAuswahlGetKonfigkanalnummer(ConfigTypGetTypAuswahl(Config)));
              break;
            case CONFIG_TYP_WERT:
               RequestNextKanal(Data, CanMemberInfoGetUid(CanMember),
                                ConfigWertGetKonfigurationskanalnummer(ConfigTypGetTypWert(Config)));
               break;
         }
         CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
         RequestNextIndex = FALSE;
         NextState = STATE_NO_CHANGE;
      }
      else
      {
         RequestNextIndex = TRUE;
      }
   }
   else if (ZentraleGetActualKonfig(Data) != (ConfigTyp *)NULL)
   {
      switch (ConfigTypGetTyp(ZentraleGetActualKonfig(Data)))
      {
         case CONFIG_TYP_AUSWAHL:
            ConfigAuswahlSetJetzigeEinstellung(ConfigTypGetTypAuswahl(ZentraleGetActualKonfig(Data)),
                                               Value);
            break;
         case CONFIG_TYP_WERT:
            ConfigWertSetAktuelleEinstellung(ConfigTypGetTypWert(ZentraleGetActualKonfig(Data)),
                                             Value);
            break;
      }
      ZentraleSetActualMesswert(Data, (ConfigMesswertTyp *)NULL);
      Config = (ConfigTyp *)MengeNext(&ZentraleGetActualKonfigWert(Data));
      if (Config != (ConfigTyp *)NULL)
      {
         ZentraleSetActualKonfig(Data, Config);
         switch (ConfigTypGetTyp(Config))
         {
            case CONFIG_TYP_AUSWAHL:
               RequestNextKanal(Data, CanMemberInfoGetUid(CanMember),
                                ConfigAuswahlGetKonfigkanalnummer(ConfigTypGetTypAuswahl(Config)));
               break;
            case CONFIG_TYP_WERT:
               RequestNextKanal(Data, CanMemberInfoGetUid(CanMember),
                                ConfigWertGetKonfigurationskanalnummer(ConfigTypGetTypWert(Config)));
               break;
         }
         CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
         RequestNextIndex = FALSE;
         NextState = STATE_WAIT_SYS_CFG_MESSWERT;
      }
      else
      {
         RequestNextIndex = TRUE;
      }
   }
   if (RequestNextIndex)
   {
      CanMember = CanMemberSearchNotConfigured(ZentraleGetCanMember(Data));
      if (CanMember != (CanMemberInfo *)NULL)
      {
         CanMemberInfoSetConfigQuerried(CanMember, TRUE);
         CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
         ZentraleSetActualConfig(Data, CanMemberInfoGetUid(CanMember));
         ZentraleSetActualIndex(Data, 0);
         MrIpcHlStatusRequest(&Cmd, ZentraleGetUid(Data),
                              CanMemberInfoGetUid(CanMember), 0);
         MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
         NextState = STATE_WAIT_CFG_DATA_HDR;
      }
      else
      {
         if (ZentraleGetWriteWeb(Data))
         {
            CanMemberSaveMesswerteCs2(ZentraleGetCanMember(Data),
                                      ZentraleGetLocPath(Data));
            CanMemberSaveMesswerteWeb(ZentraleGetCanMember(Data),
                                      ZentraleGetLocPath(Data),
                                      ZentraleGetUid(Data),
                                      ZentraleGetMajorVersion(Data),
                                      ZentraleGetMinorVersion(Data));
         }
         CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
         CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_END_QUERY);
         CanMemberSetIsChanged(ZentraleGetCanMember(Data), FALSE);
         NextState = STATE_NORMAL;
      }
   }
   return(NextState);
}

static int HandleStatusRequest(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame, Cmd;
   unsigned long Addr, NumBytes;
   unsigned int Index, i, j, Hash;
   char Buf[128];
   unsigned char Name[8];

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   if (ZentraleGetVerbose(Data))
      printf("FSM: StatusRequest %d\n", MrIpcGetCommand(CmdFrame));
   MrIpcCmdGetStatusRequest(CmdFrame, &Addr, &Index);
   if (Addr == ZentraleGetUid(Data))
   {
      if (Index == 0)
      {
         if (ZentraleGetVerbose(Data))
            puts("send device info");
         NumBytes = ConfigStatusMrsystemGeraetebeschreibung(Buf,
                                                            ZentraleGetMajorVersion(Data),
                                                            ZentraleGetMinorVersion(Data));
         Hash = Cs2CalcHash(ZentraleGetUid(Data));
         MrIpcHlStatusPosResponse(&Cmd, ZentraleGetUid(Data),
                                  MrIpcGetReceiverSocket(CmdFrame), Addr,
                                  Index, (NumBytes + 7)/ 8);
         MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
         i = 0;
         while (i < NumBytes)
         {
            j = 0;
            while (j < 8)
            {
               Name[j] = Buf[i + j];
               j++;
            }
            MrIpcHlStatusNameResponse(&Cmd, ZentraleGetUid(Data),
                                      MrIpcGetReceiverSocket(CmdFrame), Name);
            MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
            i += 8;
          }
          Hash = Cs2CalcHash(ZentraleGetUid(Data));
          MrIpcHlStatusResponse(CmdFrame, ZentraleGetUid(Data),
                                MrIpcGetSenderSocket(CmdFrame));
          MrIpcSend(ZentraleGetClientSock(Data), CmdFrame);
          MrIpcSetCanHash(CmdFrame, Hash);
      }
      else
      {
         if (ZentraleGetVerbose(Data))
            printf("we should provide status %d\n", Index);
      }


      if (ZentraleGetVerbose(Data))
         puts("we should provide status values/konfiguration");
   }
   if (ZentraleGetVerbose(Data))
      printf("FSM: new state %d\n",STATE_NO_CHANGE);
   return(STATE_NO_CHANGE);
}

static int HandleOther(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   if (ZentraleGetVerbose(Data))
      printf("FSM: unhandled %d\n", MrIpcGetCommand(CmdFrame));
   if (ZentraleGetVerbose(Data))
      printf("FSM: new state %d\n",STATE_NO_CHANGE);
   return(STATE_NO_CHANGE);
}

#include "fsmfkt_ms2master.h"
#include "fsmfkt_proxy.h"

#include "fsmtab_ms2master.h"
#include "fsmtab_proxy.h"

static SignalFunctionsType *StateMachines[] = {
   StateMachineFunctionsProxy,
   StateMachineFunctionsMs2Master,
};
static int StartStates[] = {
   STATE_WAIT_CS2,
   STATE_WAIT_FOR_MS2,
};

void ZentraleInitFsm(ZentraleStruct *Data, int MasterMode)
{
   if (MasterMode == 0)
   {
      CronAdd(ZentraleGetCronJobs(Data), PERIODIC_NAME_PING,
              PERIODIC_INTERVALL_PING, PeriodicPing,
              (void *)Data);
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_PING);
      CronAdd(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_CS2,
              PERIODIC_INTERVALL_TIMEOUT, PeriodicTimeoutCs2,
              (void *)Data);
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_CS2);
      CronAdd(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL,
              PERIODIC_INTERVALL_TIMEOUT, PeriodicTimeoutWaitNormal,
              (void *)Data);
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
      CronAdd(ZentraleGetCronJobs(Data), PERIODIC_NAME_END_QUERY,
              PERIODIC_INTERVALL_QPERIOD, PeriodicEndQueryMembers,
              (void *)Data);
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_END_QUERY);
   }
   else if (MasterMode == 1)
   {
      CronAdd(ZentraleGetCronJobs(Data), PERIODIC_NAME_QUERY,
              PERIODIC_INTERVALL_QUERY, PeriodicQueryMembers,
              (void *)Data);
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_QUERY);
      CronAdd(ZentraleGetCronJobs(Data), PERIODIC_NAME_POLLMS2,
              PERIODIC_INTERVALL_POLLMS2, PeriodicPollMs2,
              (void *)Data);
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_POLLMS2);
      CronAdd(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_MS2,
              PERIODIC_INTERVALL_TIMEOUT, PeriodicTimeoutWaitMS2,
              (void *)Data);
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_MS2);
      CronAdd(ZentraleGetCronJobs(Data), PERIODIC_NAME_LOKNAME,
              PERIODIC_INTERVALL_LOKNAME, PeriodicQueryLokname,
              (void *)Data);
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_LOKNAME);
      CronAdd(ZentraleGetCronJobs(Data), PERIODIC_NAME_LOKINFO,
              PERIODIC_INTERVALL_LOKINFO, PeriodicQueryLokinfo,
              (void *)Data);
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_LOKINFO);
      CronAdd(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL,
              PERIODIC_INTERVALL_TIMEOUT, PeriodicQueryTimeoutWaitNormal,
              (void *)Data);
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
      CronAdd(ZentraleGetCronJobs(Data), PERIODIC_NAME_END_QUERY,
              PERIODIC_INTERVALL_QPERIOD, PeriodicEndQueryMembersMs2Master,
              (void *)Data);
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_END_QUERY);
   }
   FsmInit(ZentraleGetStateMachine(Data), (void *)Data,
           StartStates[MasterMode], NUM_SIGNALS, NUM_STATES,
           StateMachines[MasterMode]);
}
