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
#include <mr_cs2ms2.h>
#include <cs2.h>
#include "cs2sl.h"

#define SELECT_TIMEOUT 100
#define MAX_DATAGRAM_SIZE 4096
#define DEVICE_ID 0xFFFF

static BOOL Loop = TRUE;

Cs2slStruct *Cs2slCreate(void)
{  Cs2slStruct *Data;

   Data = (Cs2slStruct *)malloc(sizeof(Cs2slStruct));
   if (Data != (Cs2slStruct *)NULL)
   {
      Cs2slSetVerbose(Data, FALSE);
      Cs2slSetInterface(Data, (char *)NULL);
      Cs2slSetServerPort(Data, -1);
      Cs2slSetClientSock(Data, -1);
   }
   return(Data);
}

void Cs2slDestroy(Cs2slStruct *Data)
{
   if (Cs2slGetVerbose(Data))
      puts("destroy mrCs2sl");
   free(Data);
}

void Cs2slInit(Cs2slStruct *Data, BOOL Verbose, char *Iface, char *Addr,
               int Port,
#ifdef TRACE
               BOOL Trace,
#endif
               IoFktStruct *IoFunctions)
{
   Cs2slSetVerbose(Data, Verbose);
   if (Cs2slGetVerbose(Data))
      puts("init mrCs2sl");
#ifdef TRACE
   Cs2slSetTrace(Data, Trace);
#endif
   Cs2slSetInterface(Data, Iface);
   Cs2slSetAddress(Data, Addr);
   Cs2slSetServerPort(Data, Port);
   Cs2slSetClientSock(Data, -1);
   Cs2slSetIoFunctions(Data, IoFunctions);
}

static void SigHandler(int sig)
{
   Loop = FALSE;
}

static BOOL Start(Cs2slStruct *Data)
{  struct sigaction SigStruct;

   if (Cs2slGetVerbose(Data))
      puts("start mrCs2sl: connect to system");
   if ((strlen(Cs2slGetInterface(Data)) > 0) &&
       ((strlen(Cs2slGetAddress(Data)) == 0) ||
        (strcmp(Cs2slGetAddress(Data), "0.0.0.0") == 0)))
   {
      Cs2slSetClientSock(Data,
                          MrIpcConnectIf(Cs2slGetInterface(Data),
                                         Cs2slGetServerPort(Data)));
   }
   else
   {
      Cs2slSetClientSock(Data,
                          MrIpcConnect(Cs2slGetAddress(Data),
                                       Cs2slGetServerPort(Data)));
   }
   if (Cs2slGetClientSock(Data) >= 0)
   {
      if (Cs2slGetVerbose(Data))
         puts("start mrCs2sl; start udp client");
      if (Cs2slGetIoFunctions(Data)->Open(Cs2slGetIoFunctions(Data)->private))
      {
         if (Cs2slGetVerbose(Data))
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
         MrIpcClose(Cs2slGetClientSock(Data));
         return(FALSE);
      }
   }
   else
   {
      return(FALSE);
   }
}

static void Stop(Cs2slStruct *Data)
{
   if (Cs2slGetVerbose(Data))
      puts("stop mrCs2sl");
   MrIpcClose(Cs2slGetClientSock(Data));
   Cs2slGetIoFunctions(Data)->Close(Cs2slGetIoFunctions(Data)->private);
}

static void ProcessSystemData(Cs2slStruct *Data, MrIpcCmdType *CmdFrame)
{  MrCs2CanDataType CanMsg;
   int i;

   if (MrIpcGetCommand(CmdFrame) != MrIpcCmdNull)
   {
      switch (MrIpcGetCommand(CmdFrame))
      {
         case MrIpcCmdRequestLocName:
         case MrIpcCmdRequestLocInfo:
            break;
         default:
            MrIpcDecodeToCan(CmdFrame, &CanMsg);
#ifdef TRACE
            if (Cs2slGetVerbose(Data) && Cs2slGetTrace(Data))
               MrCs2Trace(&CanMsg);
#endif
            if (Cs2slGetVerbose(Data))
            {
               printf("send can data 0x%lx %d to UDP client\n    ",
                      MrCs2GetId(&CanMsg), MrCs2GetDlc(&CanMsg));
               for (i = 0; i < 8; i++)
                  printf("0x%02x ", CanMsg.Data[i]);
               printf("\n    hash 0x%x resp 0x%x cmd 0x%x prio 0x%x\n",
                      MrCs2GetHash(&CanMsg), MrCs2GetResponse(&CanMsg),
                      MrCs2GetCommand(&CanMsg), MrCs2GetPrio(&CanMsg));
            }
            Cs2slGetIoFunctions(Data)->Write(Cs2slGetIoFunctions(Data)->private,
                                             MrIpcGetReceiverSocket(CmdFrame), &CanMsg);
            break;
      }
   }
}

static void HandleSystemData(Cs2slStruct *Data)
{  MrIpcCmdType CmdFrame;
   int RcvReturnValue;

   if (Cs2slGetVerbose(Data))
      puts("new data available");
   MrIpcInit(&CmdFrame);
   RcvReturnValue = MrIpcRecv(Cs2slGetClientSock(Data), &CmdFrame);
   if (RcvReturnValue == MR_IPC_RCV_ERROR)
   {
      if (Cs2slGetVerbose(Data))
         puts("Error in recieve from socket!");
   }
   else if (RcvReturnValue == MR_IPC_RCV_CLOSED)
   {
      if (Cs2slGetVerbose(Data))
         puts("client socket was closed\nmaybe server has stoped");
      Loop = FALSE;
   }
   else
   {
      if (Cs2slGetVerbose(Data))
         printf("read new comand frame from socket %d\n",
                MrIpcGetCommand(&CmdFrame));
      ProcessSystemData(Data, &CmdFrame);
   }
}

static void ProcessOutsideData(Cs2slStruct *Data, MrCs2CanDataType *CanMsg,
                               int SenderSocket)
{  MrIpcCmdType Cmd;
   int i;

   if (Cs2slGetVerbose(Data))
   {
      printf("can data 0x%lx %d\n    ",
             MrCs2GetId(CanMsg), MrCs2GetDlc(CanMsg));
      for (i = 0; i < 8; i++)
         printf("0x%02x ", CanMsg->Data[i]);
      printf("\n    hash 0x%x resp 0x%x cmd 0x%x prio 0x%x\n",
             MrCs2GetHash(CanMsg), MrCs2GetResponse(CanMsg),
             MrCs2GetCommand(CanMsg), MrCs2GetPrio(CanMsg));
   }
#ifdef TRACE
   if (Cs2slGetVerbose(Data) && Cs2slGetTrace(Data))
      MrCs2Trace(CanMsg);
#endif
   MrIpcInit(&Cmd);
   MrIpcEncodeFromCan(&Cmd, CanMsg);
   MrIpcSetSenderSocket(&Cmd, SenderSocket);
   MrIpcSetReceiverSocket(&Cmd, MR_IPC_SOCKET_ALL);
   if (MrIpcGetCanHash(&Cmd) == 0)
      MrIpcSetCanHash(&Cmd, Cs2CalcHash(MrIpcGetCanHash(&Cmd)));
   MrIpcSend(Cs2slGetClientSock(Data), &Cmd);
   MrIpcExit(&Cmd);
}

static void HandleOutsideData(Cs2slStruct *Data, int Fd)
{  MrCs2CanDataType CanMsg;
   BOOL Ret;

   Ret = Cs2slGetIoFunctions(Data)->Read(Cs2slGetIoFunctions(Data)->private,
                                          Fd, FALSE, &CanMsg);
   while (Ret)
   {
      if (Cs2slGetVerbose(Data))
         puts("read new udp frame");
      ProcessOutsideData(Data, &CanMsg, Fd);
      Ret = Cs2slGetIoFunctions(Data)->Read(Cs2slGetIoFunctions(Data)->private,
                                            Fd, TRUE, &CanMsg);
   }
}

void Cs2slRun(Cs2slStruct *Data)
{  fd_set ReadFds;
   int RetVal, HighFd, IoFd;
   struct timeval SelectTimeout;
   time_t Now;

   if (Start(Data))
   {
      if (Cs2slGetVerbose(Data))
         puts("run mrCs2sl");
      while (Loop)
      {
         FD_ZERO(&ReadFds);
         HighFd = 0;
         if (Cs2slGetVerbose(Data))
            printf("add client socket %d\n", Cs2slGetClientSock(Data));
         FD_SET(Cs2slGetClientSock(Data), &ReadFds);
         if (Cs2slGetClientSock(Data) > HighFd)
            HighFd = Cs2slGetClientSock(Data);
         while ((IoFd = Cs2slGetIoFunctions(Data)->GetFd(Cs2slGetIoFunctions(Data)->private)) != IOFKT_INVALID_FD)
         {
            if (Cs2slGetVerbose(Data))
               printf("add outside socket %d\n", IoFd);
            FD_SET(IoFd, &ReadFds);
            if (IoFd > HighFd)
               HighFd = IoFd;
         }
         SelectTimeout.tv_sec = SELECT_TIMEOUT;
         SelectTimeout.tv_usec = 0;
         if (Cs2slGetVerbose(Data))
            printf("wait for %d fd, max %ld s\n", HighFd, SelectTimeout.tv_sec);
         RetVal = select(HighFd + 1, &ReadFds, NULL, NULL, &SelectTimeout);
         if (Cs2slGetVerbose(Data))
            printf("select liefert %d\n", RetVal);
         if (((RetVal == -1) && (errno == EINTR)) || (RetVal == 0))
         {
            Now = time(NULL);
            if (Cs2slGetVerbose(Data))
               printf("interrupt at %s\n", asctime(localtime(&Now)));
         }
         else if (RetVal < 0)
         {
            if (Cs2slGetVerbose(Data))
               puts("error in main loop");
            Loop = FALSE;
         }
         else
         {
            if (FD_ISSET(Cs2slGetClientSock(Data), &ReadFds))
            {
               if (Cs2slGetVerbose(Data))
                  puts("data on cmd socket to drehscheibe");
               HandleSystemData(Data);
            }
            while ((IoFd = Cs2slGetIoFunctions(Data)->GetFd(Cs2slGetIoFunctions(Data)->private)) != IOFKT_INVALID_FD)
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
