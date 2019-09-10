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
#include <cs2.h>
#include <mr_cs2ms2.h>
#include <config.h>
#include "can_io.h"
#include "cs2eth.h"

#define SELECT_TIMEOUT 100
#define MAX_DATAGRAM_SIZE 4096
#define DEVICE_ID 0xFFFF

static BOOL Loop = TRUE;

Cs2ethStruct *Cs2ethCreate(void)
{  Cs2ethStruct *Data;

   Data = (Cs2ethStruct *)malloc(sizeof(Cs2ethStruct));
   if (Data != (Cs2ethStruct *)NULL)
   {
      Cs2ethSetVerbose(Data, FALSE);
      Cs2ethSetInterface(Data, "");
      Cs2ethSetServerPort(Data, -1);
      Cs2ethSetClientSock(Data, -1);
      Cs2ethSetHideMs2(Data, FALSE);
   }
   return(Data);
}

void Cs2ethDestroy(Cs2ethStruct *Data)
{
   if (Cs2ethGetVerbose(Data))
      puts("destroy mrcs2eth");
   free(Data);
}

void Cs2ethInit(Cs2ethStruct *Data, BOOL Verbose, char *Iface, char *Addr,
                int Port, char *HideMs2,
#ifdef TRACE
                BOOL Trace,
#endif
                IoFktStruct *IoFunctions)
{
   Cs2ethSetVerbose(Data, Verbose);
   if (Cs2ethGetVerbose(Data))
   {
      puts("init mrcs2eth");
      printf("     iface %s\n", Iface);
      printf("     addr %s\n", Addr);
      printf("     port %d\n", Port);
   }
#ifdef TRACE
   Cs2ethSetTrace(Data, Trace);
#endif
   Cs2ethSetInterface(Data, Iface);
   Cs2ethSetAddress(Data, Addr);
   Cs2ethSetServerPort(Data, Port);
   Cs2ethSetClientSock(Data, -1);
   Cs2ethSetHideMs2(Data, strcmp(HideMs2, MRSYSTEM_CFG_SYSTEM_HIDE) == 0);
   Cs2ethSetIoFunctions(Data, IoFunctions);
}

static void SigHandler(int sig)
{
   Loop = FALSE;
}

static BOOL Start(Cs2ethStruct *Data)
{  struct sigaction SigStruct;

   if (Cs2ethGetVerbose(Data))
      puts("start mrcs2eth: connect to system");
   if ((strlen(Cs2ethGetInterface(Data)) > 0) &&
       ((strlen(Cs2ethGetAddress(Data)) == 0) ||
        (strcmp(Cs2ethGetAddress(Data), "0.0.0.0") == 0)))
   {
      Cs2ethSetClientSock(Data,
                          MrIpcConnectIf(Cs2ethGetInterface(Data),
                                         Cs2ethGetServerPort(Data)));
   }
   else
   {
      Cs2ethSetClientSock(Data,
                          MrIpcConnect(Cs2ethGetAddress(Data),
                                       Cs2ethGetServerPort(Data)));
   }
   if (Cs2ethGetClientSock(Data) >= 0)
   {
      if (Cs2ethGetIoFunctions(Data)->Open(Cs2ethGetIoFunctions(Data)->private))
      {
         if (Cs2ethGetVerbose(Data))
            puts("ready for incoming comands from system");
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
         MrIpcClose(Cs2ethGetClientSock(Data));
         return(FALSE);
      }
   }
   else
   {
      return(FALSE);
   }
}

static void Stop(Cs2ethStruct *Data)
{
   if (Cs2ethGetVerbose(Data))
      puts("stop mrcs2eth");
   MrIpcClose(Cs2ethGetClientSock(Data));
   Cs2ethGetIoFunctions(Data)->Close(Cs2ethGetIoFunctions(Data)->private);
}

static void ProcessSystemData(Cs2ethStruct *Data, MrIpcCmdType *CmdFrame)
{  MrCs2CanDataType CanMsg;
   unsigned long Addr;
   unsigned Version, Type;

   switch (MrIpcGetCommand(CmdFrame))
   {
      case MrIpcCmdIntern:
         break;
      case MrIpcCmdRequestLocName:
      case MrIpcCmdRequestLocInfo:
      case MrIpcCmdRequestLokListe:
         break;
      case MrIpcCmdMember:
         MrIpcCmdGetMember(CmdFrame, &Addr, &Version, &Type);
         if (!Cs2ethGetHideMs2(Data) ||
             ((Type != CS2_DEVID_MS2_1) && (Type != CS2_DEVID_MS2_2)))
         {
            MrIpcDecodeToCan(CmdFrame, &CanMsg);
            if (Cs2ethGetVerbose(Data))
            {
#ifdef TRACE
               if (Cs2ethGetTrace(Data))
                  MrCs2Trace(&CanMsg);
#endif
               MrCs2DumpCanMsg(&CanMsg, "send can data to UDP client");
            }
            Cs2ethGetIoFunctions(Data)->Write(Cs2ethGetIoFunctions(Data)->private,
                                 MrIpcGetReceiverSocket(CmdFrame), &CanMsg);
         }
         break;
      default:
         MrIpcDecodeToCan(CmdFrame, &CanMsg);
         if (Cs2ethGetVerbose(Data))
         {
#ifdef TRACE
            if (Cs2ethGetTrace(Data))
               MrCs2Trace(&CanMsg);
#endif
            MrCs2DumpCanMsg(&CanMsg, "send can data to UDP client");
         }
         Cs2ethGetIoFunctions(Data)->Write(Cs2ethGetIoFunctions(Data)->private,
                              MrIpcGetReceiverSocket(CmdFrame), &CanMsg);
         break;
   }
}

static void HandleSystemData(Cs2ethStruct *Data)
{  MrIpcCmdType CmdFrame;
   int RcvReturnValue;

   if (Cs2ethGetVerbose(Data))
      puts("new data available");
   MrIpcInit(&CmdFrame);
   RcvReturnValue = MrIpcRecv(Cs2ethGetClientSock(Data), &CmdFrame);
   if (RcvReturnValue == MR_IPC_RCV_ERROR)
   {
      if (Cs2ethGetVerbose(Data))
         puts("Error in recieve from socket!");
   }
   else if (RcvReturnValue == MR_IPC_RCV_CLOSED)
   {
      if (Cs2ethGetVerbose(Data))
         puts("client socket was closed\nmaybe server has stoped");
      Loop = FALSE;
   }
   else
   {
      if (Cs2ethGetVerbose(Data))
         printf("read new comand %d frame from socket %d\n",
                MrIpcGetCommand(&CmdFrame), Cs2ethGetClientSock(Data));
      ProcessSystemData(Data, &CmdFrame);
   }
}

static void ProcessOutsideData(Cs2ethStruct *Data, MrCs2CanDataType *CanMsg,
                               int SenderSocket)
{  MrIpcCmdType Cmd;
#ifdef TRACE
   unsigned long Uid;
   int SoftwareVersion, DeviceId;
#endif
   int i;

   if (Cs2ethGetVerbose(Data))
   {
      printf("can data 0x%lx %d\n    ",
             MrCs2GetId(CanMsg), MrCs2GetDlc(CanMsg));
      for (i = 0; i < 8; i++)
         printf("0x%02x ", CanMsg->Data[i]);
      printf("\n    hash 0x%x resp 0x%x cmd 0x%x prio 0x%x\n",
             MrCs2GetHash(CanMsg), MrCs2GetResponse(CanMsg),
             MrCs2GetCommand(CanMsg), MrCs2GetPrio(CanMsg));
#ifdef TRACE
      switch (MrCs2GetCommand(CanMsg))
      {
         case CS2_CMD_PING:
            switch (MrCs2GetDlc(CanMsg))
            {
               case 0:
                  Cs2DecPing0(MrCs2GetData(CanMsg));
                  if (Cs2ethGetVerbose(Data))
                     puts("Softwarestand Anfrage/Teilnehmer Ping");
                  break;
               case 8:
                  Cs2DecPing8(MrCs2GetData(CanMsg), &Uid, &SoftwareVersion, &DeviceId);
                  if (Cs2ethGetVerbose(Data))
                     printf("Softwarestand Anfrage/Teilnehmer Ping (Uid = 0x%lx, SwVersion = 0x%x, DeviceId = 0x%x)\n",
                            Uid, SoftwareVersion, DeviceId);
                  break;
               default:
                  if (Cs2ethGetVerbose(Data))
                     puts("Softwarestand Anfrage/Teilnehmer Ping (unknown parameter)");
                  break;
            }
            break;
         default:
            break;
      }
      if (Cs2ethGetTrace(Data))
         MrCs2Trace(CanMsg);
#endif
   }
   MrIpcInit(&Cmd);
   MrIpcEncodeFromCan(&Cmd, CanMsg);
   MrIpcSetSenderSocket(&Cmd, SenderSocket);
   MrIpcSetReceiverSocket(&Cmd, MR_IPC_SOCKET_ALL);
   if (MrIpcGetCanHash(&Cmd) == 0)
      MrIpcSetCanHash(&Cmd, Cs2CalcHash(MrIpcGetCanHash(&Cmd)));
   MrIpcSend(Cs2ethGetClientSock(Data), &Cmd);
   MrIpcExit(&Cmd);
}

static void HandleOutsideData(Cs2ethStruct *Data, int Fd)
{  MrCs2CanDataType CanMsg;
   BOOL Ret;

   Ret = Cs2ethGetIoFunctions(Data)->Read(Cs2ethGetIoFunctions(Data)->private,
                                          Fd, FALSE, &CanMsg);
   while (Ret)
   {
      if (Cs2ethGetVerbose(Data))
         puts("read new udp frame");
      ProcessOutsideData(Data, &CanMsg, Fd);
      Ret = Cs2ethGetIoFunctions(Data)->Read(Cs2ethGetIoFunctions(Data)->private,
                                             Fd, TRUE, &CanMsg);
   }
}

void Cs2ethRun(Cs2ethStruct *Data)
{  fd_set ReadFds;
   int RetVal, HighFd, IoFd;
   struct timeval SelectTimeout;
   time_t Now;

   if (Start(Data))
   {
      if (Cs2ethGetVerbose(Data))
         puts("run mrcs2eth");
      while (Loop)
      {
         FD_ZERO(&ReadFds);
         HighFd = 0;
         if (Cs2ethGetVerbose(Data))
            printf("add client socket %d\n", Cs2ethGetClientSock(Data));
         FD_SET(Cs2ethGetClientSock(Data), &ReadFds);
         if (Cs2ethGetClientSock(Data) > HighFd)
            HighFd = Cs2ethGetClientSock(Data);
         while ((IoFd = Cs2ethGetIoFunctions(Data)->GetFd(Cs2ethGetIoFunctions(Data)->private)) != IOFKT_INVALID_FD)
         {
            if (Cs2ethGetVerbose(Data))
               printf("add outside socket %d\n", IoFd);
            FD_SET(IoFd, &ReadFds);
            if (IoFd > HighFd)
               HighFd = IoFd;
         }
         SelectTimeout.tv_sec = SELECT_TIMEOUT;
         SelectTimeout.tv_usec = 0;
         if (Cs2ethGetVerbose(Data))
            printf("wait for %d fd, max %ld s\n", HighFd, SelectTimeout.tv_sec);
         RetVal = select(HighFd + 1, &ReadFds, NULL, NULL, &SelectTimeout);
         if (Cs2ethGetVerbose(Data))
            printf("select liefert %d\n", RetVal);
         if (((RetVal == -1) && (errno == EINTR)) || (RetVal == 0))
         {
            Now = time(NULL);
            if (Cs2ethGetVerbose(Data))
               printf("interrupt at %s\n", asctime(localtime(&Now)));
         }
         else if (RetVal < 0)
         {
            if (Cs2ethGetVerbose(Data))
               puts("error in main loop");
            Loop = FALSE;
         }
         else
         {
            if (FD_ISSET(Cs2ethGetClientSock(Data), &ReadFds))
            {
               if (Cs2ethGetVerbose(Data))
                  puts("data on cmd socket to drehscheibe");
               HandleSystemData(Data);
            }
            while ((IoFd = Cs2ethGetIoFunctions(Data)->GetFd(Cs2ethGetIoFunctions(Data)->private)) != IOFKT_INVALID_FD)
            {
               if (FD_ISSET(IoFd, &ReadFds))
               {
                  HandleOutsideData(Data, IoFd);
               }
            }
         }
      }
      Stop(Data);
   }
}
