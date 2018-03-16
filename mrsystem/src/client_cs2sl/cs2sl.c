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
      Cs2slSetCs2Addr(Data, FALSE);
      Cs2slSetDoTcp(Data, FALSE);
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
                int Port, BOOL DoTcp
#ifdef TRACE
               , BOOL Trace
#endif
               )
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
   Cs2slSetCs2Addr(Data, FALSE);
   Cs2slSetDoTcp(Data, DoTcp);
   Cs2slSetClientSock(Data, -1);
   Cs2slSetOutsideUdpSock(Data, -1);
   Cs2slSetOutsideTcpSock(Data, -1);
   memset((void *)(&(Cs2slGetClientAddr(Data))), 0,
          sizeof(struct sockaddr_in));
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
      Cs2slSetOutsideUdpSock(Data, MrEthCs2ConnectClient());
      if (Cs2slGetOutsideUdpSock(Data) >= 0)
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
   if (Cs2slGetOutsideUdpSock(Data) >= -1)
      MrEthCs2Close(Cs2slGetOutsideUdpSock(Data));
   if (Cs2slGetOutsideTcpSock(Data) >= 0)
      MrEthCs2Close(Cs2slGetOutsideTcpSock(Data));
}

static void ProcessSystemData(Cs2slStruct *Data, MrIpcCmdType *CmdFrame)
{  char UdpFrame[MR_CS2_UDP_LENGTH];
   MrCs2CanDataType CanMsg;
   int i;

   if (MrIpcGetCommand(CmdFrame) != MrIpcCmdNull)
   {
      switch (MrIpcGetCommand(CmdFrame))
      {
         case MrIpcCmdRequestLocName:
         case MrIpcCmdRequestLocInfo:
            break;
         default:
            if ((MrIpcGetReceiverSocket(CmdFrame) == MR_IPC_SOCKET_ALL) ||
                (MrIpcGetReceiverSocket(CmdFrame) == Cs2slGetOutsideUdpSock(Data)))
            {
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
               MrEthCs2Encode(UdpFrame, &CanMsg);
               if (Cs2slGetOutsideTcpSock(Data) != -1)
               {
                  MrEthCs2Send(Cs2slGetOutsideTcpSock(Data), UdpFrame);
               }
               else if (Cs2slGetCs2Addr(Data))
               {
                  MrEthCs2SendTo(Cs2slGetOutsideUdpSock(Data),
                                 (struct sockaddr_in *)&(Cs2slGetClientAddr(Data)),
                                 UdpFrame);
               }
            }
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
      MrIpcSetCanHash(&Cmd, MrCs2CalcHash(MrIpcGetCanHash(&Cmd)));
   MrIpcSend(Cs2slGetClientSock(Data), &Cmd);
   MrIpcExit(&Cmd);
}

static void HandleOutsideTcpData(Cs2slStruct *Data)
{  char UdpFrame[MAX_DATAGRAM_SIZE];
   MrCs2CanDataType CanMsg;
   BOOL HaveRecv;

   HaveRecv = MrEthCs2Recv(Cs2slGetOutsideTcpSock(Data),
                           &Cs2slGetClientAddr(Data), UdpFrame);
   if (HaveRecv)
   {
      if (Cs2slGetVerbose(Data))
         puts("read new udp frame");
      MrEthCs2Decode(&CanMsg, UdpFrame);
      ProcessOutsideData(Data, &CanMsg, Cs2slGetOutsideTcpSock(Data));
   } 
}

static void HandleOutsideData(Cs2slStruct *Data)
{  char UdpFrame[MAX_DATAGRAM_SIZE];
   MrCs2CanDataType CanMsg;
   BOOL HaveRecv;

   HaveRecv = MrEthCs2Recv(Cs2slGetOutsideUdpSock(Data),
                           &Cs2slGetClientAddr(Data), UdpFrame);
   if (!Cs2slGetCs2Addr(Data))
   {
      if (Cs2slGetVerbose(Data))
         puts("found new cs2");
      Cs2slSetCs2Addr(Data, TRUE);
      if (Cs2slGetDoTcp(Data))
      {
         Cs2slSetOutsideTcpSock(Data,
                                MrEthCs2ConnectTcpClient(&Cs2slGetClientAddr(Data)));
         if (Cs2slGetOutsideTcpSock(Data) != -1)
         {
            MrEthCs2Close(Cs2slGetOutsideUdpSock(Data));
            Cs2slSetOutsideUdpSock(Data, -1);
            if (Cs2slGetVerbose(Data))
               printf("tcp connection socket %d\n",
                      Cs2slGetOutsideTcpSock(Data));
         }
      }
   }
   if (HaveRecv)
   {
      if (Cs2slGetVerbose(Data))
         puts("read new udp frame");
      MrEthCs2Decode(&CanMsg, UdpFrame);
      ProcessOutsideData(Data, &CanMsg, Cs2slGetOutsideUdpSock(Data));
   } 
}

void Cs2slRun(Cs2slStruct *Data)
{  fd_set ReadFds;
   int RetVal, HighFd;
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
         if (Cs2slGetOutsideUdpSock(Data) != -1)
         {
            if (Cs2slGetVerbose(Data))
               printf("add outside udp socket %d\n",
                      Cs2slGetOutsideUdpSock(Data));
            FD_SET(Cs2slGetOutsideUdpSock(Data), &ReadFds);
            if (Cs2slGetOutsideUdpSock(Data) > HighFd)
               HighFd = Cs2slGetOutsideUdpSock(Data);
         }
         if (Cs2slGetOutsideTcpSock(Data) != -1)
         {
            if (Cs2slGetVerbose(Data))
               printf("add outside tcp socket %d\n",
                      Cs2slGetOutsideTcpSock(Data));
            FD_SET(Cs2slGetOutsideTcpSock(Data), &ReadFds);
            if (Cs2slGetOutsideTcpSock(Data) > HighFd)
               HighFd = Cs2slGetOutsideTcpSock(Data);
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
            if (Cs2slGetOutsideUdpSock(Data) != -1)
            {
               if (FD_ISSET(Cs2slGetOutsideUdpSock(Data), &ReadFds))
               {
                  if (Cs2slGetVerbose(Data))
                     puts("data on udp socket to remote");
                  HandleOutsideData(Data);
               }
            }
            if (Cs2slGetOutsideTcpSock(Data) != -1)
            {
               if (FD_ISSET(Cs2slGetOutsideTcpSock(Data), &ReadFds))
               {
                  if (Cs2slGetVerbose(Data))
                     puts("data on tcp socket server to remote");
                  HandleOutsideTcpData(Data);
               }
            }
         }
      }
      Stop(Data);
   }
}
