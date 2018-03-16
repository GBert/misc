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
#include "can_io.h"
#include "ms2.h"

#define SELECT_LONG_TIMEOUT  100
#define SELECT_SHORT_TIMEOUT   1
#define TIMER_INTERVALL  10
#define NUM_BUFFERS 20

static BOOL Loop = TRUE;

static void SendMagicStart60113Frame(Ms2Struct *Data, int Verbose)
{  CanFrameStruct CanFrame;

   CanFrame.CanId = 0x360301UL;
   /* use EFF */
   CanFrame.CanId &= CAN_EFF_MASK;
   CanFrame.CanId |= CAN_EFF_FLAG;
   CanFrame.CanDlc = 5;
   CanFrame.CanData[0] = 0;
   CanFrame.CanData[1] = 0;
   CanFrame.CanData[2] = 0;
   CanFrame.CanData[3] = 0;
   CanFrame.CanData[4] = 0x11;
   if (Ms2GetIoFunctions(Data)->Write(Ms2GetIoFunctions(Data)->private,
                                      &CanFrame) != sizeof(CanFrame))
   {
      perror("CAN magic 60113 start write __");
   }
   else
   {
      if (Verbose)
         puts("CAN magic 60113 start write");
   }
}

static BOOL CreateBuffers(Queue *BufferQueue)
{  int i;
   MrCs2CanDataType *Buf;
   BOOL Ret;

   Ret = TRUE;
   for (i = 0; i < NUM_BUFFERS; i++)
   {
      Buf = (MrCs2CanDataType *)malloc(sizeof(MrCs2CanDataType));
      Ret = Ret && (Buf != (MrCs2CanDataType *)NULL);
      if (Ret)
      {
         QueueAdd(BufferQueue, (QueueDataType)Buf);
      }
   }
   return(Ret);
}

Ms2Struct *Ms2Create(void)
{  Ms2Struct *Data;

   Data = (Ms2Struct *)malloc(sizeof(Ms2Struct));
   if (Data != (Ms2Struct *)NULL)
   {
      Ms2SetVerbose(Data, FALSE);
      Ms2SetInterface(Data, (char *)NULL);
      Ms2SetServerPort(Data, -1);
      Ms2SetClientSock(Data, -1);
      Ms2SetZentraleMode(Data, MASTER_MODE_MS2_MASTER);
      Ms2SetGbCommands(Data, QueueCreate());
      Ms2SetCmdBuffers(Data, QueueCreate());
      Ms2SetIoFunctions(Data, (IoFktStruct *)NULL);
      if (Ms2GetGbCommands(Data) == (Queue *)NULL)
      {
         if (Ms2GetCmdBuffers(Data) != (Queue *)NULL)
         {
            QueueDestroy(Ms2GetCmdBuffers(Data));
            Ms2SetCmdBuffers(Data, (Queue *)NULL);
         }
         free(Data);
         Data = (Ms2Struct *)NULL;
      }
      else if (Ms2GetCmdBuffers(Data) == (Queue *)NULL)
      {
         if (Ms2GetGbCommands(Data) != (Queue *)NULL)
         {
            QueueDestroy(Ms2GetGbCommands(Data));
            Ms2SetGbCommands(Data, (Queue *)NULL);
         }
         free(Data);
         Data = (Ms2Struct *)NULL;
      }
      else
      {
         QueueInit(Ms2GetGbCommands(Data), free);
         QueueInit(Ms2GetCmdBuffers(Data), free);
         if (!CreateBuffers(Ms2GetCmdBuffers(Data)))
         {
            QueueDestroy(Ms2GetCmdBuffers(Data));
            Ms2SetCmdBuffers(Data, (Queue *)NULL);
            QueueDestroy(Ms2GetGbCommands(Data));
            Ms2SetGbCommands(Data, (Queue *)NULL);
            free(Data);
            Data = (Ms2Struct *)NULL;
            Ms2SetNumBuffers(Data, 0);
         }
         else
         {
            Ms2SetNumBuffers(Data, NUM_BUFFERS);
         }
      }
   }
   return(Data);
}

void Ms2Destroy(Ms2Struct *Data)
{
   if (Ms2GetVerbose(Data))
      puts("destroy mrms2");
   if (Ms2GetGbCommands(Data) != (Queue *)NULL)
   {
      QueueDestroy(Ms2GetGbCommands(Data));
      Ms2SetGbCommands(Data, (Queue *)NULL);
   }
   if (Ms2GetCmdBuffers(Data) != (Queue *)NULL)
   {
      QueueDestroy(Ms2GetCmdBuffers(Data));
      Ms2SetCmdBuffers(Data, (Queue *)NULL);
   }
   free(Data);
}

void Ms2Init(Ms2Struct *Data, BOOL Verbose, char *Interface,
                   char *Addr, int Port, int ZentraleMode,
                   IoFktStruct *IoFunctions)
{
   Ms2SetVerbose(Data, Verbose);
   Ms2SetInterface(Data, Interface);
   Ms2SetAddress(Data, Addr);
   Ms2SetServerPort(Data, Port);
   Ms2SetClientSock(Data, -1);
   Ms2SetZentraleMode(Data, ZentraleMode);
   Ms2SetActualCmd(Data, (MrCs2CanDataType *)NULL);
   Ms2SetIoFunctions(Data, IoFunctions);
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
      Ms2SetClientSock(Data, MrIpcConnectIf(Ms2GetInterface(Data),
                                            Ms2GetServerPort(Data)));
   }
   else
   {
      Ms2SetClientSock(Data, MrIpcConnect(Ms2GetAddress(Data),
                                          Ms2GetServerPort(Data)));
   }
   if (Ms2GetClientSock(Data) >= 0)
   {
      if (Ms2GetVerbose(Data))
         puts("start mrm2: open can socket");
      if (Ms2GetIoFunctions(Data)->Open(Ms2GetIoFunctions(Data)->private))
      {
         if (Ms2GetVerbose(Data))
            puts("ready for incoming comands from system");
         SigStruct.sa_handler = SigHandler;
         sigemptyset(&SigStruct.sa_mask);
         SigStruct.sa_flags = 0;
         sigaction(SIGINT, &SigStruct, NULL);
         sigaction(SIGQUIT, &SigStruct, NULL);
         sigaction(SIGTERM, &SigStruct, NULL);
         SendMagicStart60113Frame(Data, Ms2GetVerbose(Data));
         return(TRUE);
      }
      else
      {
         puts("ERROR: can not open CAN connection");
         Ms2GetIoFunctions(Data)->Close(Ms2GetIoFunctions(Data)->private);
         return(FALSE);
      }
   }
   else
   {
      puts("ERROR: can not open socket to 'drehscheibe'");
      return(FALSE);
   }
}

static void Stop(Ms2Struct *Data)
{
   if (Ms2GetVerbose(Data))
      puts("stop network client");
   MrIpcClose(Ms2GetClientSock(Data));
   Ms2GetIoFunctions(Data)->Close(Ms2GetIoFunctions(Data)->private);
}

static void QueryLoknamen(Ms2Struct *Data, int Start, int End)
{  MrCs2CanDataType CanMsg;
   CanFrameStruct CanFrame;
   char MsgBuf[9];

   if (Ms2GetVerbose(Data))
      printf("querry loknamen %d - %d\n", Start, End);
   MrCs2SetHash(&CanMsg, MrMs2CalcHash(MR_CS2_UID_BROADCAST));
   MrCs2SetResponse(&CanMsg, 0);
   MrCs2SetPrio(&CanMsg, MR_CS2_PRIO_0);
   MrCs2EncConfigQuery(&CanMsg, MR_MS2_CFG_LOCNAMES);
   MrMs2Encode(&CanMsg, &CanFrame);
   Ms2GetIoFunctions(Data)->Write(Ms2GetIoFunctions(Data)->private,
                                        &CanFrame);
   MrCs2SetHash(&CanMsg, MrMs2CalcHash(MR_CS2_UID_BROADCAST));
   MrCs2SetResponse(&CanMsg, 0);
   MrCs2SetPrio(&CanMsg, MR_CS2_PRIO_0);
   sprintf(MsgBuf, "%d %d", Start, End);
   MrCs2EncConfigQuery(&CanMsg, MsgBuf);
   MrMs2Encode(&CanMsg, &CanFrame);
   Ms2GetIoFunctions(Data)->Write(Ms2GetIoFunctions(Data)->private,
                                        &CanFrame);
}

static void QueryLokinfo(Ms2Struct *Data, char *Locname)
{  MrCs2CanDataType CanMsg;
   CanFrameStruct CanFrame;
   char MsgBuf[9];

   if (Ms2GetVerbose(Data))
      printf("querry lokinfo >%s<\n", Locname);
   MrCs2SetHash(&CanMsg, MrMs2CalcHash(MR_CS2_UID_BROADCAST));
   MrCs2SetResponse(&CanMsg, 0);
   MrCs2SetPrio(&CanMsg, MR_CS2_PRIO_0);
   MrCs2EncConfigQuery(&CanMsg, MR_MS2_CFG_LOCINFO);
   MrMs2Encode(&CanMsg, &CanFrame);
   Ms2GetIoFunctions(Data)->Write(Ms2GetIoFunctions(Data)->private,
                                        &CanFrame);
   MrCs2SetHash(&CanMsg, MrMs2CalcHash(MR_CS2_UID_BROADCAST));
   MrCs2SetResponse(&CanMsg, 0);
   MrCs2SetPrio(&CanMsg, MR_CS2_PRIO_0);
   strncpy(MsgBuf, Locname, 8);
   MsgBuf[8] = '\0';
   MrCs2EncConfigQuery(&CanMsg, MsgBuf);
   MrMs2Encode(&CanMsg, &CanFrame);
   Ms2GetIoFunctions(Data)->Write(Ms2GetIoFunctions(Data)->private,
                                        &CanFrame);
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
   Ms2GetIoFunctions(Data)->Write(Ms2GetIoFunctions(Data)->private,
                                        &CanFrame);
}

static BOOL ShouldQueue(Ms2Struct *Data, MrIpcCmdType *CmdFrame)
{
   return((MrIpcGetCanResponse(CmdFrame) == 0u) &&
          ((MrIpcGetCanCommand(CmdFrame) == MrIpcCmdLocomotiveSpeed) ||
           (MrIpcGetCanCommand(CmdFrame) == MrIpcCmdLocomotiveDirection) ||
           (MrIpcGetCanCommand(CmdFrame) == MrIpcCmdLocomotiveFunction) ||
           (MrIpcGetCanCommand(CmdFrame) == MrIpcCmdAccSwitch)));
}

static void ForwardToCan(Ms2Struct *Data, MrMs2CanDataType *CanMsg)
{  int i;
   CanFrameStruct CanFrame;

   MrMs2Encode(CanMsg, &CanFrame);
   if (Ms2GetVerbose(Data))
   {
      printf("send can data 0x%lx %d\n    ",
             MrCs2GetId(CanMsg), MrCs2GetDlc(CanMsg));
      for (i = 0; i < 8; i++)
         printf("0x%02x ", CanMsg->Data[i]);
      printf("\n    hash 0x%x resp 0x%x cmd 0x%x prio 0x%x\n",
             MrCs2GetHash(CanMsg), MrCs2GetResponse(CanMsg),
             MrCs2GetCommand(CanMsg), MrCs2GetPrio(CanMsg));
   }
   if ((Ms2GetZentraleMode(Data) == MASTER_MODE_PROXY) ||
       (MrCs2GetCommand(CanMsg) != MR_CS2_CMD_BOOTLDR_CAN))
   {
      Ms2GetIoFunctions(Data)->Write(Ms2GetIoFunctions(Data)->private,
                                     &CanFrame);
   }
}

static void ProcessSystemData(Ms2Struct *Data, MrIpcCmdType *CmdFrame)
{  MrMs2CanDataType CanMsg, *BufCanMsg;
   CanFrameStruct CanFrame;
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
            Ms2GetIoFunctions(Data)->Write(Ms2GetIoFunctions(Data)->private,
                                                 &CanFrame);
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
         if ((MrIpcGetReceiverSocket(CmdFrame) == MR_IPC_SOCKET_ALL) ||
             (MrIpcGetReceiverSocket(CmdFrame) == Ms2GetIoFunctions(Data)->GetFd(Ms2GetIoFunctions(Data)->private)))
         {
            if (!ShouldQueue(Data, CmdFrame))
            {
               MrIpcDecodeToCan(CmdFrame, &CanMsg);
               ForwardToCan(Data, &CanMsg);
            }
            else
            {
               if (QueueIsEmpty(Ms2GetCmdBuffers(Data)))
               {
                  if (!CreateBuffers(Ms2GetCmdBuffers(Data)))
                  {
                     MrIpcDecodeToCan(CmdFrame, &CanMsg);
                     ForwardToCan(Data, &CanMsg);
                  }
                  else
                  {
                     Ms2SetNumBuffers(Data,
                                      Ms2GetNumBuffers(Data) + NUM_BUFFERS);
                  }
               }
               if (!QueueIsEmpty(Ms2GetCmdBuffers(Data)))
               {
                  BufCanMsg = (MrMs2CanDataType *)QueueGet(Ms2GetCmdBuffers(Data));
                  MrIpcDecodeToCan(CmdFrame, BufCanMsg);
                  if (Ms2GetActualCmd(Data) == (MrCs2CanDataType *)NULL)
                  {
                     Ms2SetActualCmd(Data, BufCanMsg);
                     ForwardToCan(Data, BufCanMsg);
                     Ms2SetLastTime(Data, time(NULL));
                  }
                  else
                  {
                     QueueAdd(Ms2GetCmdBuffers(Data), (QueueDataType)BufCanMsg);
                  }
               }
            }
         }
         break;
   }
}

static void HandleSystemData(Ms2Struct *Data)
{  MrIpcCmdType CmdFrame;
   int RcvReturnValue;

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

static BOOL DataEqual(unsigned int Dlc, unsigned char *Data1,
                      unsigned char *Data2)
{  BOOL Ret;
   int i;

   Ret = TRUE;
   for (i = 0; (i < Dlc) && (i < MR_CS2_NUM_CAN_BYTES); i++)
   {
      Ret = Ret && (Data1[i] == Data2[i]);
   }
   return(Ret);
}

static BOOL IsQueuedAnswer(Ms2Struct *Data, MrMs2CanDataType *CanMsg)
{
   return((MrCs2GetResponse(CanMsg) == 1u) &&
          (MrCs2GetCommand(CanMsg) == MrCs2GetCommand(Ms2GetActualCmd(Data))) &&
          (MrCs2GetDlc(CanMsg) == MrCs2GetDlc(Ms2GetActualCmd(Data))) &&
          DataEqual(MrCs2GetDlc(CanMsg), MrCs2GetData(CanMsg),
                    MrCs2GetData(Ms2GetActualCmd(Data))));
}

static void ProcessCanData(Ms2Struct *Data, MrMs2CanDataType *CanMsg,
                           int SenderSocket)
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
      MrIpcSetSenderSocket(&Cmd, SenderSocket);
      MrIpcSetReceiverSocket(&Cmd, MR_IPC_SOCKET_ALL);
      MrIpcSend(Ms2GetClientSock(Data), &Cmd);
      if ((Ms2GetActualCmd(Data) != (MrCs2CanDataType *)NULL) &&
          IsQueuedAnswer(Data, CanMsg))
      {
         if (Ms2GetNumBuffers(Data) > NUM_BUFFERS)
         {
            free((void *)Ms2GetActualCmd(Data));
            Ms2SetNumBuffers(Data, Ms2GetNumBuffers(Data) - 1);
         }
         else
         {
            QueueAdd(Ms2GetCmdBuffers(Data),
                     (QueueDataType)Ms2GetActualCmd(Data));
         }
         Ms2SetActualCmd(Data, (QueueDataType)NULL);
         if (!QueueIsEmpty(Ms2GetGbCommands(Data)))
         {
            Ms2SetActualCmd(Data,
                            (MrCs2CanDataType *)QueueGet(Ms2GetGbCommands(Data)));
            ForwardToCan(Data, Ms2GetActualCmd(Data));
            Ms2SetLastTime(Data, time(NULL));
         }
      }
   }
}

static void HandleCanData(Ms2Struct *Data)
{  CanFrameStruct CanFrame;
   MrMs2CanDataType CanMsg;

   if (Ms2GetVerbose(Data))
      puts("new can data available");
   if (Ms2GetIoFunctions(Data)->Read(Ms2GetIoFunctions(Data)->private,
                                           &CanFrame) == sizeof(CanFrameStruct))
   {
      MrMs2Decode(&CanMsg, &CanFrame);
      ProcessCanData(Data, &CanMsg,
                     Ms2GetIoFunctions(Data)->GetFd(Ms2GetIoFunctions(Data)->private));
   }
}

static void HandleTimeout(Ms2Struct *Data)
{  int i;

   if (Ms2GetActualCmd(Data) != (MrCs2CanDataType *)NULL)
   {
      if (Ms2GetVerbose(Data))
      {
         printf("timeout for can data 0x%lx %d\n    ",
                MrCs2GetId(Ms2GetActualCmd(Data)),
                MrCs2GetDlc(Ms2GetActualCmd(Data)));
         for (i = 0; i < 8; i++)
            printf("0x%02x ", Ms2GetActualCmd(Data)->Data[i]);
         printf("\n    hash 0x%x resp 0x%x cmd 0x%x prio 0x%x\n",
                MrCs2GetHash(Ms2GetActualCmd(Data)),
                MrCs2GetResponse(Ms2GetActualCmd(Data)),
                MrCs2GetCommand(Ms2GetActualCmd(Data)), 
                MrCs2GetPrio(Ms2GetActualCmd(Data)));
      }
      if (Ms2GetNumBuffers(Data) > NUM_BUFFERS)
      {
         free((void *)Ms2GetActualCmd(Data));
         Ms2SetNumBuffers(Data, Ms2GetNumBuffers(Data) - 1);
      }
      Ms2SetActualCmd(Data, (QueueDataType)NULL);
      if (!QueueIsEmpty(Ms2GetGbCommands(Data)))
      {
         Ms2SetActualCmd(Data,
                        (MrCs2CanDataType *)QueueGet(Ms2GetGbCommands(Data)));
         ForwardToCan(Data, Ms2GetActualCmd(Data));
         Ms2SetLastTime(Data, time(NULL));
      }
   }
}

void Ms2Run(Ms2Struct *Data)
{  fd_set ReadFds;
   int RetVal, HighFd;
   struct timeval SelectTimeout;
   time_t Now;

   if (Start(Data))
   {
      Ms2SetLastTime(Data, time(NULL));
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
            printf("add can socket %d\n",
                   Ms2GetIoFunctions(Data)->GetFd(Ms2GetIoFunctions(Data)->private));
         FD_SET(Ms2GetIoFunctions(Data)->GetFd(Ms2GetIoFunctions(Data)->private),
                &ReadFds);
         if (Ms2GetIoFunctions(Data)->GetFd(Ms2GetIoFunctions(Data)->private) > HighFd)
            HighFd = Ms2GetIoFunctions(Data)->GetFd(Ms2GetIoFunctions(Data)->private);
         if (Ms2GetActualCmd(Data) != (MrCs2CanDataType *)NULL)
            SelectTimeout.tv_sec = SELECT_SHORT_TIMEOUT;
         else
            SelectTimeout.tv_sec = SELECT_LONG_TIMEOUT;
         SelectTimeout.tv_usec = 0;
         if (Ms2GetVerbose(Data))
            printf("wait for %d fd, max %ld s\n", HighFd, SelectTimeout.tv_sec);
         RetVal = select(HighFd + 1, &ReadFds, NULL, NULL, &SelectTimeout);
         Now = time(NULL);
         if (Ms2GetVerbose(Data))
            printf("select liefert %d\n", RetVal);
         if (((RetVal == -1) && (errno == EINTR)) || (RetVal == 0))
         {
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
            if (FD_ISSET(Ms2GetIoFunctions(Data)->GetFd(Ms2GetIoFunctions(Data)->private),
                         &ReadFds))
            {
               if (Ms2GetVerbose(Data))
                  puts("new data on can socket");
               HandleCanData(Data);
            }
         }
         if ((Ms2GetActualCmd(Data) != (MrCs2CanDataType *)NULL) &&
             ((Now - Ms2GetLastTime(Data)) > TIMER_INTERVALL))
         {
            HandleTimeout(Data);
         }
      }
      Stop(Data);
   }
   else
   {
      puts("ERROR: can not start Ms2 module");
   }
}
