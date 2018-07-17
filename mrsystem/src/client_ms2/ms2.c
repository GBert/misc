#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/can.h>
#include <string.h>
#include <errno.h>
#include <boolean.h>
#include <mr_ipc.h>
#include <mr_can.h>
#include "ms2.h"

#define SELECT_TIMEOUT 100

static BOOL Loop = TRUE;

Ms2Struct *Ms2Create(void)
{  Ms2Struct *Data;

   Data = (Ms2Struct *)malloc(sizeof(Ms2Struct));
   if (Data != (Ms2Struct *)NULL)
   {
      Ms2SetVerbose(Data, FALSE);
      Ms2SetInterface(Data, (char *)NULL);
      Ms2SetServerPort(Data, -1);
      Ms2SetClientSock(Data, -1);
      Ms2SetCanName(Data, (char *)NULL);
      Ms2SetCanSock(Data, -1);
      Ms2SetZentraleMode(Data, MASTER_MODE_MS2_MASTER);
   }
   return(Data);
}

void Ms2Destroy(Ms2Struct *Data)
{
   if (Ms2GetVerbose(Data))
      puts("destroy mrms2");
   free(Data);
}

void Ms2Init(Ms2Struct *Data, BOOL Verbose, char *Interface, char *Addr,
             int Port, char *CanIf, int ZentraleMode)
{
   Ms2SetVerbose(Data, Verbose);
   Ms2SetInterface(Data, Interface);
   Ms2SetAddress(Data, Addr);
   Ms2SetServerPort(Data, Port);
   Ms2SetClientSock(Data, -1);
   Ms2SetCanSock(Data, -1);
   Ms2SetCanName(Data, CanIf);
   Ms2SetZentraleMode(Data, ZentraleMode);
}

static void SigHandler(int sig)
{
   Loop = FALSE;
}

static BOOL Start(Ms2Struct *Data)
{  struct sigaction SigStruct;

   if (Ms2GetVerbose(Data))
      puts("start mrm2: open socket to system");
   if ((strlen(Ms2GetInterface(Data)) > 0) &&
       ((strlen(Ms2GetAddress(Data)) == 0) ||
        (strcmp(Ms2GetAddress(Data), "0.0.0.0") == 0)))
   {
      Ms2SetClientSock(Data,
                       MrIpcConnectIf(Ms2GetInterface(Data),
                                      Ms2GetServerPort(Data)));
   }
   else
   {
      Ms2SetClientSock(Data,
                       MrIpcConnect(Ms2GetAddress(Data),
                                    Ms2GetServerPort(Data)));
   }
   if (Ms2GetClientSock(Data) >= 0)
   {
      if (Ms2GetVerbose(Data))
         puts("start mrm2: open can socket");
      Ms2SetCanSock(Data, MrMs2Connect(Ms2GetCanName(Data)));
      if (Ms2GetCanSock(Data) >= 0)
      {
         if (Ms2GetVerbose(Data))
            puts("ready for incoming comands from system");
         SigStruct.sa_handler = SigHandler;
         sigemptyset(&SigStruct.sa_mask);
         SigStruct.sa_flags = 0;
         sigaction(SIGINT, &SigStruct, NULL);
         sigaction(SIGQUIT, &SigStruct, NULL);
         sigaction(SIGTERM, &SigStruct, NULL);
         SendMagicStart60113Frame(Ms2GetCanSock(Data), Ms2GetVerbose(Data));
         return(TRUE);
      }
      else
      {
         MrIpcClose(Ms2GetClientSock(Data));
         return(FALSE);
      }
   }
   else
   {
      return(FALSE);
   }
}

static void Stop(Ms2Struct *Data)
{
   if (Ms2GetVerbose(Data))
      puts("stop network client");
   MrIpcClose(Ms2GetClientSock(Data));
   MrMs2Close(Ms2GetCanSock(Data));
}

static void QueryLoknamen(Ms2Struct *Data, int Start, int End)
{  MrCs2CanDataType CanMsg;
   struct can_frame CanFrame;
   char MsgBuf[9];

   if (Ms2GetVerbose(Data))
      printf("querry loknamen %d - %d\n", Start, End);
   MrCs2SetHash(&CanMsg, MrMs2CalcHash(MR_CS2_UID_BROADCAST));
   MrCs2SetResponse(&CanMsg, 0);
   MrCs2SetPrio(&CanMsg, MR_CS2_PRIO_0);
   MrCs2EncConfigQuery(&CanMsg, MR_MS2_CFG_LOCNAMES);
   MrMs2Encode(&CanMsg, &CanFrame);
   MrMs2Send(Ms2GetCanSock(Data), &CanFrame);
   MrCs2SetHash(&CanMsg, MrMs2CalcHash(MR_CS2_UID_BROADCAST));
   MrCs2SetResponse(&CanMsg, 0);
   MrCs2SetPrio(&CanMsg, MR_CS2_PRIO_0);
   sprintf(MsgBuf, "%d %d", Start, End);
   MrCs2EncConfigQuery(&CanMsg, MsgBuf);
   MrMs2Encode(&CanMsg, &CanFrame);
   MrMs2Send(Ms2GetCanSock(Data), &CanFrame);
}

static void QueryLokinfo(Ms2Struct *Data, char *Locname)
{  MrCs2CanDataType CanMsg;
   struct can_frame CanFrame;
   char MsgBuf[9];

   if (Ms2GetVerbose(Data))
      printf("querry lokinfo >%s<\n", Locname);
   MrCs2SetHash(&CanMsg, MrMs2CalcHash(MR_CS2_UID_BROADCAST));
   MrCs2SetResponse(&CanMsg, 0);
   MrCs2SetPrio(&CanMsg, MR_CS2_PRIO_0);
   MrCs2EncConfigQuery(&CanMsg, MR_MS2_CFG_LOCINFO);
   MrMs2Encode(&CanMsg, &CanFrame);
   MrMs2Send(Ms2GetCanSock(Data), &CanFrame);
   MrCs2SetHash(&CanMsg, MrMs2CalcHash(MR_CS2_UID_BROADCAST));
   MrCs2SetResponse(&CanMsg, 0);
   MrCs2SetPrio(&CanMsg, MR_CS2_PRIO_0);
   strncpy(MsgBuf, Locname, 8);
   MsgBuf[8] = '\0';
   MrCs2EncConfigQuery(&CanMsg, MsgBuf);
   MrMs2Encode(&CanMsg, &CanFrame);
   MrMs2Send(Ms2GetCanSock(Data), &CanFrame);
   MrCs2SetHash(&CanMsg, MrMs2CalcHash(MR_CS2_UID_BROADCAST));
   MrCs2SetResponse(&CanMsg, 0);
   MrCs2SetPrio(&CanMsg, MR_CS2_PRIO_0);
   if (strlen(Locname) > 8)
   {
      strncpy(MsgBuf, Locname + 8, 8);
      MsgBuf[8] = '\0';
   }
   else
   {
      MsgBuf[0] = '\0';
   }
   MrCs2EncConfigQuery(&CanMsg, MsgBuf);
   MrMs2Encode(&CanMsg, &CanFrame);
   MrMs2Send(Ms2GetCanSock(Data), &CanFrame);
}

static void ProcessSystemData(Ms2Struct *Data, MrIpcCmdType *CmdFrame)
{  MrMs2CanDataType CanMsg;
   struct can_frame CanFrame;
   unsigned int i, StartIdx, EndIdx;
   char LokName[17];

   switch (MrIpcGetCommand(CmdFrame))
   {
      case MrIpcCmdMember:
      case MrIpcCmdRequestFile:
      case MrIpcCmdCfgHeader:
      case MrIpcCmdCfgZHeader:
      case MrIpcCmdCfgData:
         if (Ms2GetZentraleMode(Data) == MASTER_MODE_PROXY)
         {
            MrIpcDecodeToCan(CmdFrame, &CanMsg);
            MrMs2Encode(&CanMsg, &CanFrame);
            if (Ms2GetVerbose(Data))
            {
               printf("send can data 0x%lx %d\n    ",
                      MrCs2GetId(&CanMsg), MrCs2GetDlc(&CanMsg));
               for (i = 0; i < 8; i++)
                  printf("0x%02x ", CanMsg.Data[i]);
               printf("\n    hash 0x%x resp 0x%x cmd 0x%x prio 0x%x\n",
                      MrCs2GetHash(&CanMsg), MrCs2GetResponse(&CanMsg),
                      MrCs2GetCommand(&CanMsg), MrCs2GetPrio(&CanMsg));
            }
            MrMs2Send(Ms2GetCanSock(Data), &CanFrame);
         }
         break;
      case MrIpcCmdRequestLocName:
         MrIpcCmdGetReqestLocname(CmdFrame, &StartIdx, &EndIdx);
         QueryLoknamen(Data, StartIdx, EndIdx);
         break;
      case MrIpcCmdRequestLocInfo:
         MrIpcCmdGetReqestLocinfo(CmdFrame, LokName);
         QueryLokinfo(Data, LokName);
         break;
      default:
         MrIpcDecodeToCan(CmdFrame, &CanMsg);
         MrMs2Encode(&CanMsg, &CanFrame);
         if (Ms2GetVerbose(Data))
         {
            printf("send can data 0x%lx %d\n    ",
                   MrCs2GetId(&CanMsg), MrCs2GetDlc(&CanMsg));
            for (i = 0; i < 8; i++)
               printf("0x%02x ", CanMsg.Data[i]);
            printf("\n    hash 0x%x resp 0x%x cmd 0x%x prio 0x%x\n",
                   MrCs2GetHash(&CanMsg), MrCs2GetResponse(&CanMsg),
                   MrCs2GetCommand(&CanMsg), MrCs2GetPrio(&CanMsg));
         }
         if ((Ms2GetZentraleMode(Data) == MASTER_MODE_PROXY) ||
             (MrCs2GetCommand(&CanMsg) != MR_CS2_CMD_BOOTLDR_CAN))
         {
            MrMs2Send(Ms2GetCanSock(Data), &CanFrame);
         }
         break;
   }
}

static void HandleSystemData(Ms2Struct *Data)
{  MrIpcCmdType CmdFrame;
   int RcvReturnValue;

   if (Ms2GetVerbose(Data))
      puts("new data available");
   MrIpcInit(&CmdFrame);
   RcvReturnValue = MrIpcRecv(Ms2GetClientSock(Data), &CmdFrame);
   if (RcvReturnValue == MR_IPC_RCV_ERROR)
   {
      if (Ms2GetVerbose(Data))
         puts("Error in recieve from socket!");
   }
   else if (RcvReturnValue == MR_IPC_RCV_CLOSED)
   {
      if (Ms2GetVerbose(Data))
         puts("client socket was closed\nmaybe server has stoped");
      Loop = FALSE;
   }
   else
   {
      if (Ms2GetVerbose(Data))
         printf("read new comand frame from socket %d\n",
                MrIpcGetCommand(&CmdFrame));
      ProcessSystemData(Data, &CmdFrame);
   }
}

static void ProcessCanData(Ms2Struct *Data, MrMs2CanDataType *CanMsg)
{  MrIpcCmdType Cmd;
   int i;

   if (MrCs2GetIsCs2(CanMsg))
   {
      if (Ms2GetVerbose(Data))
      {
         printf("get can data 0x%lx %d\n    ",
                MrCs2GetId(CanMsg), MrCs2GetDlc(CanMsg));
         for (i = 0; i < 8; i++)
            printf("0x%02x ", CanMsg->Data[i]);
         printf("\n    hash 0x%x resp 0x%x cmd 0x%x prio 0x%x\n",
                MrCs2GetHash(CanMsg), MrCs2GetResponse(CanMsg),
                MrCs2GetCommand(CanMsg), MrCs2GetPrio(CanMsg));
         puts("send to drehscheibe");
      }
      MrIpcInit(&Cmd);
      MrIpcEncodeFromCan(&Cmd, CanMsg);
      MrIpcSend(Ms2GetClientSock(Data), &Cmd);
   }
}

static void HandleCanData(Ms2Struct *Data)
{  struct can_frame frame;
   MrMs2CanDataType CanMsg;

   if (read(Ms2GetCanSock(Data), &frame, sizeof(struct can_frame)) ==
       sizeof(struct can_frame))
   {
      MrMs2Decode(&CanMsg, &frame);
      ProcessCanData(Data, &CanMsg);
   }
}

void Ms2Run(Ms2Struct *Data)
{  fd_set ReadFds;
   int RetVal, HighFd;
   struct timeval SelectTimeout;
   time_t Now;

   if (Start(Data))
   {
      while (Loop)
      {
         FD_ZERO(&ReadFds);
         HighFd = 0;
         if (Ms2GetVerbose(Data))
            printf("add client socket %d\n", Ms2GetClientSock(Data));
         FD_SET(Ms2GetClientSock(Data), &ReadFds);
         if (Ms2GetClientSock(Data) > HighFd)
            HighFd = Ms2GetClientSock(Data);
         if (Ms2GetVerbose(Data))
            printf("add can socket %d\n", Ms2GetCanSock(Data));
         FD_SET(Ms2GetCanSock(Data), &ReadFds);
         if (Ms2GetCanSock(Data) > HighFd)
            HighFd = Ms2GetCanSock(Data);
         SelectTimeout.tv_sec = SELECT_TIMEOUT;
         SelectTimeout.tv_usec = 0;
         if (Ms2GetVerbose(Data))
            printf("wait for %d fd, max %ld s\n", HighFd, SelectTimeout.tv_sec);
         RetVal = select(HighFd + 1, &ReadFds, NULL, NULL, &SelectTimeout);
         if (Ms2GetVerbose(Data))
            printf("select liefert %d\n", RetVal);
         if (((RetVal == -1) && (errno == EINTR)) || (RetVal == 0))
         {
            Now = time(NULL);
            if (Ms2GetVerbose(Data))
               printf("interrupt at %s\n", asctime(localtime(&Now)));
         }
         else if (RetVal < 0)
         {
            if (Ms2GetVerbose(Data))
               puts("error in main loop");
            Loop = FALSE;
         }
         else
         {
            if (FD_ISSET(Ms2GetClientSock(Data), &ReadFds))
            {
               if (Ms2GetVerbose(Data))
                  puts("new data on cmd socket to drehscheibe");
               HandleSystemData(Data);
            }
            if (FD_ISSET(Ms2GetCanSock(Data), &ReadFds))
            {
               if (Ms2GetVerbose(Data))
                  puts("new data on can socket");
               HandleCanData(Data);
            }
         }
      }
      Stop(Data);
   }
}
