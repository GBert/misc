#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <boolean.h>
#include <mr_ipc.h>
#include <config.h>
#include <cs2parse.h>
#include <write_cs2.h>
#include <fsm.h>
#include "zentrale.h"
#include "cron.h"
#include "canmember.h"
#include "lok.h"
#include "lokstatus.h"
#include "magnetartikel.h"
#include "magstat.h"
#include "gleisbild.h"
#include "gleisbildpage.h"
#include "gbsstat.h"
#include "fahrstrasse.h"
#include "fsstat.h"
#include "cs2cfg.h"

#define SELECT_TIMEOUT  1
#define TIMER_INTERVALL  10
#define GERAET_VRS_FILE "geraet.vrs"
#define S88_WAKEUP_PARM_DELIMETER " .,;:!-"
#define TCYC_MAX 1000 /* max cycle time in ms */

static BOOL Loop;

ZentraleStruct *ZentraleCreate(void)
{  ZentraleStruct *Data;

   Data = (ZentraleStruct *)malloc(sizeof(ZentraleStruct));
   if (Data != (ZentraleStruct *)NULL)
   {
      ZentraleSetVerbose(Data, FALSE);
      ZentraleSetMasterMode(Data, MASTER_MODE_MS2_MASTER);
      ZentraleSetInterface(Data, (char *)NULL);
      ZentraleSetServerPort(Data, -1);
      ZentraleSetClientSock(Data, -1);
      ZentraleSetLocPath(Data, (char *)NULL);
      ZentraleSetMajorVersion(Data, 0);
      ZentraleSetMinorVersion(Data, 1);
      ZentraleSetUid(Data, 0l);
      ZentraleSetStateMachine(Data, FsmCreate());
      ZentraleSetGleisPages(Data, (GleisbildPageStruct **)NULL);
      if (ZentraleGetStateMachine(Data) != (FsmStruct *)NULL)
      {
         ZentraleSetPackedCs2File(Data, ZFileCreate());
         if (ZentraleGetPackedCs2File(Data) != (ZlibFile *)NULL)
         {
            ZentraleSetCronJobs(Data, CronCreate());
            if (ZentraleGetCronJobs(Data) != (CronStruct *)NULL)
            {
               ZentraleSetLoks(Data, LokCreate());
               if (ZentraleGetLoks(Data) != (LokStruct *)NULL)
               {
                  ZentraleSetMagnetartikel(Data, MagnetartikelCreate());
                  if (ZentraleGetMagnetartikel(Data) != (MagnetartikelStruct *)NULL)
                  {
                     ZentraleSetGleisbild(Data, GleisbildCreate());
                     if (ZentraleGetGleisbild(Data) != (GleisbildStruct *)NULL)
                     {
                        ZentraleSetFahrstrasse(Data, FahrstrasseCreate());
                        if (ZentraleGetFahrstrasse(Data) != (FahrstrasseStruct *)NULL)
                        {
                           ZentraleSetCanMember(Data, CanMemberCreate());
                           if (ZentraleGetCanMember(Data) != (CanMemberStruct *)NULL)
                           {
                              ZentraleSetCs2CfgDaten(Data, Cs2CfgDataCreate());
                              if (ZentraleGetCs2CfgDaten(Data) == (Cs2CfgData *)NULL)
                              {
                                 CanMemberDestroy(ZentraleGetCanMember(Data));
                                 GleisbildDestroy(ZentraleGetGleisbild(Data));
                                 MagnetartikelDestroy(ZentraleGetMagnetartikel(Data));
                                 LokDestroy(ZentraleGetLoks(Data));
                                 CronDestroy(ZentraleGetCronJobs(Data));
                                 ZFileDestroy(ZentraleGetPackedCs2File(Data));
                                 FsmDestroy(ZentraleGetStateMachine(Data));
                                 free(Data);
                                 Data = (ZentraleStruct *)NULL;
                              }
                           }
                           else
                           {
                              GleisbildDestroy(ZentraleGetGleisbild(Data));
                              MagnetartikelDestroy(ZentraleGetMagnetartikel(Data));
                              LokDestroy(ZentraleGetLoks(Data));
                              CronDestroy(ZentraleGetCronJobs(Data));
                              ZFileDestroy(ZentraleGetPackedCs2File(Data));
                              FsmDestroy(ZentraleGetStateMachine(Data));
                              CanMemberDestroy(ZentraleGetCanMember(Data));
                              free(Data);
                              Data = (ZentraleStruct *)NULL;
                           }
                        }
                        else
                        {
                           GleisbildDestroy(ZentraleGetGleisbild(Data));
                           MagnetartikelDestroy(ZentraleGetMagnetartikel(Data));
                           LokDestroy(ZentraleGetLoks(Data));
                           CronDestroy(ZentraleGetCronJobs(Data));
                           ZFileDestroy(ZentraleGetPackedCs2File(Data));
                           FsmDestroy(ZentraleGetStateMachine(Data));
                           free(Data);
                           Data = (ZentraleStruct *)NULL;
                        }
                     }
                     else
                     {
                        MagnetartikelDestroy(ZentraleGetMagnetartikel(Data));
                        LokDestroy(ZentraleGetLoks(Data));
                        CronDestroy(ZentraleGetCronJobs(Data));
                        ZFileDestroy(ZentraleGetPackedCs2File(Data));
                        FsmDestroy(ZentraleGetStateMachine(Data));
                        free(Data);
                        Data = (ZentraleStruct *)NULL;
                     }
                  }
                  else
                  {
                     LokDestroy(ZentraleGetLoks(Data));
                     CronDestroy(ZentraleGetCronJobs(Data));
                     ZFileDestroy(ZentraleGetPackedCs2File(Data));
                     FsmDestroy(ZentraleGetStateMachine(Data));
                     free(Data);
                     Data = (ZentraleStruct *)NULL;
                  }
               }
               else
               {
                  CronDestroy(ZentraleGetCronJobs(Data));
                  ZFileDestroy(ZentraleGetPackedCs2File(Data));
                  FsmDestroy(ZentraleGetStateMachine(Data));
                  free(Data);
                  Data = (ZentraleStruct *)NULL;
               }
            }
            else
            {
               ZFileDestroy(ZentraleGetPackedCs2File(Data));
               FsmDestroy(ZentraleGetStateMachine(Data));
               free(Data);
               Data = (ZentraleStruct *)NULL;
            }
         }
         else
         {
            FsmDestroy(ZentraleGetStateMachine(Data));
            free(Data);
            Data = (ZentraleStruct *)NULL;
         }
      }
      else
      {
         free(Data);
         Data = (ZentraleStruct *)NULL;
      }
   }
   return(Data);
}

void ZentraleDestroy(ZentraleStruct *Data)
{  int i;

   if (ZentraleGetVerbose(Data))
      puts("destroy zentrale");
   if (ZentraleGetCanMember(Data) != (CanMemberStruct *)NULL)
      CanMemberDestroy(ZentraleGetCanMember(Data));
   if (ZentraleGetFahrstrasse(Data) != (FahrstrasseStruct *)NULL)
      FahrstrasseDestroy(ZentraleGetFahrstrasse(Data));
   if (ZentraleGetGleisPages(Data) != (GleisbildPageStruct **)NULL)
   {
      for (i = 0; i < GleisbildGetNumPages(ZentraleGetGleisbild(Data)); i++)
         if (ZentraleGetNrGleisPages(Data, i) != (GleisbildPageStruct *)NULL)
            GleisbildPageDestroy(ZentraleGetNrGleisPages(Data, i));
      free(ZentraleGetGleisPages(Data));
   }
   if (ZentraleGetGleisbild(Data) != (GleisbildStruct *)NULL)
      GleisbildDestroy(ZentraleGetGleisbild(Data));
   if (ZentraleGetMagnetartikel(Data) != (MagnetartikelStruct *)NULL)
      MagnetartikelDestroy(ZentraleGetMagnetartikel(Data));
   if (ZentraleGetLoks(Data) != (LokStruct *)NULL)
      LokDestroy(ZentraleGetLoks(Data));
   if (ZentraleGetCronJobs(Data) != (CronStruct *)NULL)
      CronDestroy(ZentraleGetCronJobs(Data));
   if (ZentraleGetStateMachine(Data) != (FsmStruct *)NULL)
      FsmDestroy(ZentraleGetStateMachine(Data));
   if (ZentraleGetPackedCs2File(Data) != (ZlibFile *)NULL)
      ZFileDestroy(ZentraleGetPackedCs2File(Data));
   if (ZentraleGetCs2CfgDaten(Data) != (Cs2CfgData *)NULL)
      Cs2CfgDataDestroy(ZentraleGetCs2CfgDaten(Data));
   free(Data);
}

static void LoadGleisPage(void *PrivData, MapKeyType Key, MapDataType Daten)
{  ZentraleStruct *Data;
   GleisbildInfo *GleisbildPage;

   Data = (ZentraleStruct *)PrivData;
   GleisbildPage = (GleisbildInfo *)Daten;
   ZentraleSetNrGleisPages(Data, GleisbildInfoGetId(GleisbildPage),
                           GleisbildPageCreate());
   GleisbildPageInit(ZentraleGetNrGleisPages(Data,
                                             GleisbildInfoGetId(GleisbildPage)),
                     ZentraleGetLocPath(Data),
                     GleisbildInfoGetName(GleisbildPage),
                     GleisbildInfoGetId(GleisbildPage));
   GleisbildPageLoadGleisbildPageCs2(ZentraleGetNrGleisPages(Data,
                                                             GleisbildInfoGetId(GleisbildPage)));
}

void ZentraleInit(ZentraleStruct *Data, BOOL Verbose, int MasterMode,
                  char *Interface, char *Addr, int Port, char *LocPath,
                  int Protokolle, char *SystemStart, int SyncMask,
                  char *WakeUpS88, int NumLokFkts, BOOL WriteWeb)
{  Cs2parser *GeraetVrsParser;
   int LineInfo, handle;
   char *FileBuffer, *GeraetVrsFile;
   off_t FileLaenge;
   ssize_t NumBytes;

   ZentraleSetVerbose(Data, Verbose);
   if (ZentraleGetVerbose(Data))
      puts("ZentraleInit");
   ZentraleSetMasterMode(Data, MasterMode);
   ZentraleSetInterface(Data, Interface);
   ZentraleSetAddress(Data, Addr);
   ZentraleSetServerPort(Data, Port);
   ZentraleSetClientSock(Data, -1);
   CronInit(ZentraleGetCronJobs(Data));
   ZentraleInitFsm(Data, ZentraleGetMasterMode(Data));
   ZentraleSetLocPath(Data, LocPath);
   ZentraleSetProtokolle(Data, Protokolle);
   ZentraleSetWriteWeb(Data, WriteWeb);
   ZentraleSetIsInPoll(Data, FALSE);
   ZentraleSetSystemStart(Data,
                          (strcmp(MRSYSTEM_CFG_SYSTEM_START, SystemStart) == 0));
   ZentraleSetSyncMask(Data, SyncMask);
   ZentraleSetShouldWakeUpS88(Data, (strcmp(WakeUpS88,DISABLE_WAKEUP_S88)!=0));
   ZentraleSetActualMesswert(Data, (ConfigMesswertTyp *)NULL);
   ZentraleSetActualKonfig(Data, (ConfigTyp *)NULL);
   ZentraleSetWakeUpS88(Data, WakeUpS88);
   if (ZentraleGetShouldWakeUpS88(Data))
   {  char *token, *TmpPtr;
      int i;

      TmpPtr = ZentraleGetWakeUpS88(Data);
      ZentraleSetS88BusIdxLength(Data, 0, 0);
      ZentraleSetS88BusIdxInterval(Data, 0, 0);
      ZentraleSetS88BusIdxTCycle(Data, 0, 0);
      ZentraleSetS88BusIdxLength(Data, 1, 0);
      ZentraleSetS88BusIdxInterval(Data, 1, 0);
      ZentraleSetS88BusIdxTCycle(Data, 1, 0);
      ZentraleSetS88BusIdxLength(Data, 2, 1);
      ZentraleSetS88BusIdxInterval(Data, 2, 0);
      ZentraleSetS88BusIdxTCycle(Data, 2, 0);
      while ((token = strsep(&TmpPtr, S88_WAKEUP_PARM_DELIMETER)))
      {
         if (*token == 'B')
         {
            token++;
            i = *token - '0';
            if ((i > 0) && (i < 4))
            {
               token++;
               ZentraleSetS88BusIdxLength(Data, i - 1, (int)strtoul(++token, (char **)NULL, 10));
            }
         }
         else if (*token == 'T')
         {
            token++;
            i = *token - '0';
            if ((i > 0) && (i < 4))
            {
               token++;
               ZentraleSetS88BusIdxTCycle(Data, i - 1, (int)strtoul(++token, (char **)NULL, 10));
#if 0
               if (s88_bus[i - 1].tcyc > TCYC_MAX)
               {
                  fprintf(stderr, "Cycle time %d ms greater than TCYC_MAX of %d ms\n", s88_bus[i - 1].tcyc, TCYC_MAX);
                  exit(EXIT_FAILURE);
               }
#endif
            }
         }
      }
   }
   ZentraleSetNumLoks(Data, 2);
   ZentraleSetMaxLoks(Data, LOK_MAX_LOKS);
   ZentraleSetLokNamen(Data,
                       malloc(ZentraleGetMaxLoks(Data) *
                              sizeof(ZentraleLokName)));
   CanMemberInit(ZentraleGetCanMember(Data));
   Cs2CfgDataInit(ZentraleGetCs2CfgDaten(Data), ZentraleGetVerbose(Data));
   LokInit(ZentraleGetLoks(Data), LocPath, NumLokFkts);
   LokLoadLokomotiveCs2(ZentraleGetLoks(Data));
   LokStatusLoadLokomotiveSr2(ZentraleGetLoks(Data));
   GleisbildInit(ZentraleGetGleisbild(Data), LocPath);
   GleisbildLoadGleisbildCs2(ZentraleGetGleisbild(Data));
   if (GleisbildGetNumPages(ZentraleGetGleisbild(Data)) > 0)
   {
      ZentraleSetGleisPages(Data,
                            (GleisbildPageStruct **)malloc(sizeof(GleisbildPageStruct *) *
                                                           GleisbildGetNumPages(ZentraleGetGleisbild(Data))));
      if (ZentraleGetGleisPages(Data) != (GleisbildPageStruct **)NULL)
      {
         if (ZentraleGetVerbose(Data))
            puts("load Gleisbild Pages");
         MapWalkAscend(GleisbildGetGleisbildDb(ZentraleGetGleisbild(Data)),
                       (MapWalkCbFkt)LoadGleisPage,
                       (void *)Data);
      }
   }
   GbsStatLoadGbsStatSr2(ZentraleGetGleisbild(Data));
   FahrstrasseInit(ZentraleGetFahrstrasse(Data), LocPath);
   FahrstrasseLoadFahrstrasseCs2(ZentraleGetFahrstrasse(Data));
   FsStatLoadFsStatSr2(ZentraleGetFahrstrasse(Data));
   MagnetartikelInit(ZentraleGetMagnetartikel(Data), LocPath);
   MagnetartikelLoadMagnetartikelCs2(ZentraleGetMagnetartikel(Data));
   MagStatusLoadMagStatusSr2(ZentraleGetMagnetartikel(Data));
   if (ZentraleGetVerbose(Data))
      puts("read geraet.vrs");
   GeraetVrsFile = (char *)malloc(strlen(ZentraleGetLocPath(Data)) + 
                                  strlen(GERAET_VRS_FILE) + 2);
   if (GeraetVrsFile != (char *)NULL)
   {
      strcpy(GeraetVrsFile, ZentraleGetLocPath(Data));
      if (GeraetVrsFile[strlen(GeraetVrsFile) - 1] != '/')
         strcat(GeraetVrsFile, "/");
      strcat(GeraetVrsFile, GERAET_VRS_FILE);
      handle = open(GeraetVrsFile, O_RDONLY);
      if (handle >= 0)
      {
         FileLaenge = lseek(handle, 0, SEEK_END);
         FileBuffer = malloc(FileLaenge);
         if (FileBuffer != NULL)
         {
            lseek(handle, 0, SEEK_SET);
            NumBytes = read(handle, FileBuffer, (size_t)FileLaenge);
            if (NumBytes > 0)
            {
               GeraetVrsParser = Cs2pCreate();
               Cs2pInit(GeraetVrsParser, PARSER_TYPE_GERAET_VRS, FileBuffer,
                        NumBytes);
               Cs2pSetVerbose(GeraetVrsParser, FALSE);
               do {
                  LineInfo = Cs2pParse(GeraetVrsParser);
                  switch (LineInfo)
                  {
                     case PARSER_ERROR:
                        if (ZentraleGetVerbose(Data))
                           puts("ERROR in geraet.vrs");
                        break;
                     case PARSER_EOF:
                        if (ZentraleGetVerbose(Data))
                           puts("end of geraet.vrs");
                        break;
                     case PARSER_PARAGRAPH:
                        if (ZentraleGetVerbose(Data))
                           printf("new paragraph %s in geraet.vrs\n",
                                  Cs2pGetName(GeraetVrsParser));
                        switch (Cs2pGetSubType(GeraetVrsParser))
                        {
                           case PARSER_PARAGRAPH_GERAET:
                              if (ZentraleGetVerbose(Data))
                                 puts("geraet paragraph in geraet.vrs");
                              break;
                        }
                        break;
                     case PARSER_VALUE:
                        if (ZentraleGetVerbose(Data))
                           printf("new value %s=%s in lok cfg\n",
                                  Cs2pGetName(GeraetVrsParser),
                                  Cs2pGetValue(GeraetVrsParser));
                        switch (Cs2pGetSubType(GeraetVrsParser))
                        {
                           case PARSER_VALUE_GERAET:
                              if (ZentraleGetVerbose(Data))
                                 puts("neuer geraet Eintrag");
                              break;
                           case PARSER_VALUE_VERSION:
                              if (ZentraleGetVerbose(Data))
                                 puts("neuer version Eintrag");
                              break;
                           case PARSER_VALUE_MINOR:
                              if (ZentraleGetVerbose(Data))
                                 puts("minor version");
                              ZentraleSetMinorVersion(Data,
                                                      strtoul(Cs2pGetValue(GeraetVrsParser),
                                                              NULL, 0));
                              break;
                           case PARSER_VALUE_MAJOR:
                              if (ZentraleGetVerbose(Data))
                                 puts("major version");
                              ZentraleSetMajorVersion(Data,
                                                      strtoul(Cs2pGetValue(GeraetVrsParser),
                                                              NULL, 0));
                              break;
                           case PARSER_VALUE_SERNUM:
                              if (ZentraleGetVerbose(Data))
                                 puts("serial number");
                              ZentraleSetSerialNumber(Data,
                                                      strtoul(Cs2pGetValue(GeraetVrsParser),
                                                              NULL, 0));
                              break;
                           case PARSER_VALUE_GFPUID:
                              if (ZentraleGetVerbose(Data))
                                 puts("gfp uid");
                              ZentraleSetGfpUid(Data,
                                                strtoul(Cs2pGetValue(GeraetVrsParser),
                                                        NULL, 0));
                              break;
                           case PARSER_VALUE_GUIUID:
                              if (ZentraleGetVerbose(Data))
                                 puts("gui uid");
                              ZentraleSetUid(Data,
                                             strtoul(Cs2pGetValue(GeraetVrsParser),
                                                     NULL, 0));
                              break;
                           case PARSER_VALUE_HARDVERS:
                              if (ZentraleGetVerbose(Data))
                                 puts("hardware version");
                              ZentraleSetHardVersion(Data,
                                                     strtof(Cs2pGetValue(GeraetVrsParser),
                                                            NULL));
                              break;
                        }
                        break;
                  }
               } while (LineInfo != PARSER_EOF);
               Cs2pDestroy(GeraetVrsParser);
            }
            else
            {
               if (ZentraleGetVerbose(Data))
                  puts("kann kein geraet.vrs nicht lesen");
            }
            free(FileBuffer);
         }
         else
         {
            if (ZentraleGetVerbose(Data))
               printf("kann kein Speicher fuer Dateipuffer (%ld) anlegen\n",
                      FileLaenge);
         }
         close(handle);
      }
      else
      {
         if (ZentraleGetVerbose(Data))
            puts("kann geraet.vrs nicht oeffnen");
      }
      free(GeraetVrsFile);
   }
   else
   {
      if (ZentraleGetVerbose(Data))
         puts("kann kein Speicher fuer Dateiname anlegen");
   }
}

void ZentraleExit(ZentraleStruct *Data)
{  int i;

   if (ZentraleGetVerbose(Data))
      puts("exit zentrale");
   if (ZentraleGetCanMember(Data) != (CanMemberStruct *)NULL)
      CanMemberExit(ZentraleGetCanMember(Data));
   if (ZentraleGetCs2CfgDaten(Data) != (Cs2CfgData *)NULL)
      Cs2CfgDataExit(ZentraleGetCs2CfgDaten(Data));
   if (ZentraleGetFahrstrasse(Data) != (FahrstrasseStruct *)NULL)
      FahrstrasseExit(ZentraleGetFahrstrasse(Data));
   if (ZentraleGetGleisPages(Data) != (GleisbildPageStruct **)NULL)
   {
      for (i = 0; i < GleisbildGetNumPages(ZentraleGetGleisbild(Data)); i++)
         if (ZentraleGetNrGleisPages(Data, i) != (GleisbildPageStruct *)NULL)
            GleisbildPageExit(ZentraleGetNrGleisPages(Data, i));
   }
   if (ZentraleGetGleisbild(Data) != (GleisbildStruct *)NULL)
      GleisbildExit(ZentraleGetGleisbild(Data));
   if (ZentraleGetMagnetartikel(Data) != (MagnetartikelStruct *)NULL)
      MagnetartikelExit(ZentraleGetMagnetartikel(Data));
   if (ZentraleGetLoks(Data) != (LokStruct *)NULL)
      LokExit(ZentraleGetLoks(Data));
   if (ZentraleGetCronJobs(Data) != (CronStruct *)NULL)
      CronExit(ZentraleGetCronJobs(Data));
   if (ZentraleGetStateMachine(Data) != (FsmStruct *)NULL)
      FsmExit(ZentraleGetStateMachine(Data));
   if (ZentraleGetPackedCs2File(Data) == (ZlibFile *)NULL)
      ZFileExit(ZentraleGetPackedCs2File(Data));
}

static void SigHandler(int sig)
{
   Loop = FALSE;
}

static BOOL Start(ZentraleStruct *Data)
{  struct sigaction SigStruct;

   if ((strlen(ZentraleGetInterface(Data)) > 0) &&
       ((strlen(ZentraleGetAddress(Data)) == 0) ||
        (strcmp(ZentraleGetAddress(Data), "0.0.0.0") == 0)))
   {
      ZentraleSetClientSock(Data,
                            MrIpcConnectIf(ZentraleGetInterface(Data),
                                           ZentraleGetServerPort(Data)));
   }
   else
   {
      ZentraleSetClientSock(Data,
                            MrIpcConnect(ZentraleGetAddress(Data),
                                         ZentraleGetServerPort(Data)));
   }
   if (ZentraleGetClientSock(Data) >= 0)
   {
      if (ZentraleGetVerbose(Data))
         puts("ready for incoming comands");
      SigStruct.sa_handler = SigHandler;
      sigemptyset(&SigStruct.sa_mask);
      SigStruct.sa_flags = 0;
      sigaction(SIGINT, &SigStruct, NULL);
      sigaction(SIGQUIT, &SigStruct, NULL);
      sigaction(SIGTERM, &SigStruct, NULL);
      return(TRUE);
   }
   else
   {
      puts("ERROR: can not open socket to 'drehscheibe'");
      return(FALSE);
   }
}

static void Stop(ZentraleStruct *Data)
{
   if (ZentraleGetVerbose(Data))
      puts("stop network client");
   if (ZentraleGetClientSock(Data) >= 0)
   {
      MrIpcClose(ZentraleGetClientSock(Data));
   }
}

static void ProcessSystemData(ZentraleStruct *Data, MrIpcCmdType *CmdFrame)
{
   if (ZentraleGetVerbose(Data))
      printf("FSM State %d\n", FsmGetState(ZentraleGetStateMachine(Data)));
   FsmDo(ZentraleGetStateMachine(Data), MrIpcGetCommand(CmdFrame) + 1,
         (void *)CmdFrame);
}

static void HandleSystemData(ZentraleStruct *Data)
{  MrIpcCmdType CmdFrame;
   int RcvReturnValue;

   MrIpcInit(&CmdFrame);
   RcvReturnValue = MrIpcRecv(ZentraleGetClientSock(Data), &CmdFrame);
   if (RcvReturnValue == MR_IPC_RCV_ERROR)
   {
      if (ZentraleGetVerbose(Data))
         puts("Error in recieve from socket!");
   }
   else if (RcvReturnValue == MR_IPC_RCV_CLOSED)
   {
      if (ZentraleGetVerbose(Data))
         puts("client socket was closed\nmaybe server has stoped");
      Loop = FALSE;
   }
   else
   {
      if (ZentraleGetVerbose(Data))
         printf("read new comand frame from socket %d\n",
                MrIpcGetCommand(&CmdFrame));
      ProcessSystemData(Data, &CmdFrame);
   }
}

static void SwitchOn(ZentraleStruct *Data)
{  MrIpcCmdType Cmd;

   if (ZentraleGetProtokolle(Data) != 0)
   {  int CanProtokolle, CfgProtokolle;

      CfgProtokolle = ZentraleGetProtokolle(Data);
      CanProtokolle = 0;
      if ((CfgProtokolle & MRSYSTEM_CFG_PROTO_MOTOROLA) == MRSYSTEM_CFG_PROTO_MOTOROLA)
         CanProtokolle |= CS2_TRACK_PROTO_MM2;
      if ((CfgProtokolle & MRSYSTEM_CFG_PROTO_MFX) == MRSYSTEM_CFG_PROTO_MFX)
         CanProtokolle |= CS2_TRACK_PROTO_MFX;
      if ((CfgProtokolle & MRSYSTEM_CFG_PROTO_DCC) == MRSYSTEM_CFG_PROTO_DCC)
         CanProtokolle |= CS2_TRACK_PROTO_DCC;
      MrIpcInit(&Cmd);
      MrIpcSetSenderSocket(&Cmd, MR_IPC_SOCKET_ALL);
      MrIpcSetReceiverSocket(&Cmd, MR_IPC_SOCKET_ALL);
      MrIpcSetCanResponse(&Cmd, 0);
      MrIpcCalcHash(&Cmd, ZentraleGetUid(Data));
      MrIpcSetCanCommand(&Cmd, CS2_CMD_SYSTEM);
      MrIpcSetCanPrio(&Cmd, CS2_PRIO_0);
      MrIpcCmdSetTrackProto(&Cmd, CanProtokolle);
      MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
      if (ZentraleGetVerbose(Data))
         printf("enable track protocol 0x%x\n",CanProtokolle);
   }
   if (ZentraleGetSystemStart(Data))
   {
      MrIpcInit(&Cmd);
      MrIpcSetSenderSocket(&Cmd, MR_IPC_SOCKET_ALL);
      MrIpcSetReceiverSocket(&Cmd, MR_IPC_SOCKET_ALL);
      MrIpcSetCanResponse(&Cmd, 0);
      MrIpcCalcHash(&Cmd, ZentraleGetUid(Data));
      MrIpcSetCanCommand(&Cmd, CS2_CMD_SYSTEM);
      MrIpcSetCanPrio(&Cmd, CS2_PRIO_0);
      MrIpcCmdSetRun(&Cmd, On);
      MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
      if (ZentraleGetVerbose(Data))
         puts("switch track signal on");
   }
}

static void StartTimerCheck(ZentraleStruct *Data)
{  MrIpcCmdType Cmd;

   MrIpcInit(&Cmd);
   MrIpcSetSenderSocket(&Cmd, MR_IPC_SOCKET_ALL);
   MrIpcSetReceiverSocket(&Cmd, MR_IPC_SOCKET_ALL);
   MrIpcSetCommand(&Cmd, MrIpcCmdNull);
   MrIpcSetCanResponse(&Cmd, 0);
   MrIpcCalcHash(&Cmd, CS2_UID_BROADCAST);
   MrIpcSetCanCommand(&Cmd, 0xffff);
   MrIpcSetCanPrio(&Cmd, CS2_PRIO_2);
   FsmDo(ZentraleGetStateMachine(Data), 0, &Cmd);
}

void ZentraleRun(ZentraleStruct *Data)
{  fd_set ReadFds;
   int RetVal, HighFd;
   struct timeval SelectTimeout;

   if (Start(Data))
   {
      SwitchOn(Data);
      Loop = TRUE;
      while (Loop)
      {
         FD_ZERO(&ReadFds);
         HighFd = 0;
         if (ZentraleGetVerbose(Data))
            printf("add client socket %d\n", ZentraleGetClientSock(Data));
         FD_SET(ZentraleGetClientSock(Data), &ReadFds);
         if (ZentraleGetClientSock(Data) > HighFd)
            HighFd = ZentraleGetClientSock(Data);
         SelectTimeout.tv_sec = SELECT_TIMEOUT;
         SelectTimeout.tv_usec = 0;
         if (ZentraleGetVerbose(Data))
            printf("wait for %d fd, max %ld s\n", HighFd, SelectTimeout.tv_sec);
         RetVal = select(HighFd + 1, &ReadFds, NULL, NULL, &SelectTimeout);
         if (ZentraleGetVerbose(Data))
            printf("select liefert %d\n", RetVal);
         if (((RetVal == -1) && (errno == EINTR)) || (RetVal == 0))
         {
            if (ZentraleGetVerbose(Data))
               puts("interrupt");
            StartTimerCheck(Data);
         }
         else if (RetVal < 0)
         {
            if (ZentraleGetVerbose(Data))
               puts("error in main loop");
            Loop = FALSE;
         }
         else
         {
            if (FD_ISSET(ZentraleGetClientSock(Data), &ReadFds))
            {
               /* new cmd frame */
               HandleSystemData(Data);
            }
            StartTimerCheck(Data);
         }
      }
      Stop(Data);
   }
   else
   {
      puts("ERROR: can not start zentrale module");
   }
}
