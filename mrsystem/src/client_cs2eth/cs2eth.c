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
#include "cs2eth.h"

#define SELECT_TIMEOUT 100
#define MAX_DATAGRAM_SIZE 4096
#define DEVICE_ID 0xFFFF

typedef struct {
   int ClientSock;
   struct sockaddr_in ClientAddr;
} ClientInfo;

static BOOL Loop = TRUE;

Cs2ethStruct *Cs2ethCreate(void)
{  Cs2ethStruct *Data;

   Data = (Cs2ethStruct *)malloc(sizeof(Cs2ethStruct));
   if (Data != (Cs2ethStruct *)NULL)
   {
      Cs2ethSetClients(Data, MengeCreate());
      if (Cs2ethGetClients(Data) != (Menge *)NULL)
      {
         Cs2ethSetClientIter(Data, MengeCreateIter());
         if (Cs2ethGetClientIter(Data) != (MengeIterator *)NULL)
         {
            Cs2ethSetVerbose(Data, FALSE);
            Cs2ethSetInterface(Data, "");
            Cs2ethSetServerPort(Data, -1);
            Cs2ethSetClientSock(Data, -1);
         }
         else
         {
            MengeDestroy(Cs2ethGetClients(Data));
            free(Data);
            Data = (Cs2ethStruct *)NULL;
         }
      }
      else
      {
         free(Data);
         Data = (Cs2ethStruct *)NULL;
      }
   }
   return(Data);
}

void Cs2ethDestroy(Cs2ethStruct *Data)
{
   if (Cs2ethGetVerbose(Data))
      puts("destroy mrcs2eth");
   MengeDestroyIterator(Cs2ethGetClientIter(Data));
   MengeDestroy(Cs2ethGetClients(Data));
   free(Data);
}

static int ClientCompare(void *d1, void *d2)
{  ClientInfo *p1, *p2;

   p1 = (ClientInfo *)d1;
   p2 = (ClientInfo *)d2;
   return(p1->ClientSock - p2->ClientSock);
}

void Cs2ethInit(Cs2ethStruct *Data, BOOL Verbose, char *Iface, char *Addr,
                int Port, BOOL SendBc, char *BcAddr
#ifdef TRACE
               , BOOL Trace
#endif
               )
{
   Cs2ethSetVerbose(Data, Verbose);
   if (Cs2ethGetVerbose(Data))
   {
      puts("init mrcs2eth");
      printf("     iface %s\n", Iface);
      printf("     addr %s\n", Addr);
      printf("     port %d\n", Port);
      printf("     bc addr %s\n", BcAddr);
   }
#ifdef TRACE
   Cs2ethSetTrace(Data, Trace);
#endif
   Cs2ethSetSendUdpBc(Data, SendBc);
   Cs2ethSetUdpConnected(Data, FALSE);
   Cs2ethSetInterface(Data, Iface);
   Cs2ethSetAddress(Data, Addr);
   Cs2ethSetBcIp(Data, BcAddr);
   Cs2ethSetServerPort(Data, Port);
   Cs2ethSetClientSock(Data, -1);
   Cs2ethSetOutsideUdpSock(Data, -1);
   Cs2ethSetOutsideTcpSock(Data, -1);
   memset((void *)(&(Cs2ethGetClientAddr(Data))), 0,
          sizeof(struct sockaddr_in));
   MengeInit(Cs2ethGetClients(Data), ClientCompare, free);
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
      if (Cs2ethGetVerbose(Data))
         puts("start mrcs2eth: start tcp server");
      Cs2ethSetOutsideTcpSock(Data, MrEthCs2StartAppServer());
      if (Cs2ethGetOutsideTcpSock(Data) >= 0)
      {
         if (Cs2ethGetSendUdpBc(Data))
         {
            if (Cs2ethGetVerbose(Data))
               puts("start mrcs2eth: start bc server");
            Cs2ethSetOutsideUdpSock(Data, MrEthCs2StartBcServer());
            if (Cs2ethGetOutsideUdpSock(Data) >= 0)
            {
               MrEthCs2MkBcAddr(&Cs2ethGetClientAddr(Data),
                                Cs2ethGetBcIp(Data));
               Cs2ethSetUdpConnected(Data, TRUE);
               return(TRUE);
            }
            else
            {
               MrEthCs2Close(Cs2ethGetOutsideTcpSock(Data));
               MrIpcClose(Cs2ethGetClientSock(Data));
               return(FALSE);
            }
         }
         else
         {
            if (Cs2ethGetVerbose(Data))
               puts("start mrcs2eth; start udp server");
            Cs2ethSetOutsideUdpSock(Data, MrEthCs2StartServer());
            if (Cs2ethGetOutsideUdpSock(Data) >= 0)
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
{  ClientInfo *OneClient;

   if (Cs2ethGetVerbose(Data))
      puts("stop mrcs2eth");
   MengeInitIterator(Cs2ethGetClientIter(Data), Cs2ethGetClients(Data));
   OneClient = (ClientInfo *)MengeFirst(Cs2ethGetClientIter(Data));
   while (OneClient != (ClientInfo *)NULL)
   {
      MrEthCs2Close(OneClient->ClientSock);
      OneClient = (ClientInfo *)MengeNext(Cs2ethGetClientIter(Data));
   }
   MrIpcClose(Cs2ethGetClientSock(Data));
   MrEthCs2Close(Cs2ethGetOutsideUdpSock(Data));
   MrEthCs2Close(Cs2ethGetOutsideTcpSock(Data));
}

static void ProcessSystemData(Cs2ethStruct *Data, MrIpcCmdType *CmdFrame)
{  char UdpFrame[MR_CS2_UDP_LENGTH];
   MrCs2CanDataType CanMsg;
   int i;
   ClientInfo *OneClient;

   if (MrIpcGetCommand(CmdFrame) != MrIpcCmdNull)
   {
      switch (MrIpcGetCommand(CmdFrame))
      {
         case MrIpcCmdRequestLocName:
         case MrIpcCmdRequestLocInfo:
            break;
         default:
            MrIpcDecodeToCan(CmdFrame, &CanMsg);
            if (Cs2ethGetVerbose(Data))
            {
#ifdef TRACE
               if (Cs2ethGetTrace(Data))
                  MrCs2Trace(&CanMsg);
#endif
               printf("send can data 0x%lx %d to UDP client\n    ",
                      MrCs2GetId(&CanMsg), MrCs2GetDlc(&CanMsg));
               for (i = 0; i < 8; i++)
                  printf("0x%02x ", CanMsg.Data[i]);
               printf("\n    hash 0x%x resp 0x%x cmd 0x%x prio 0x%x\n",
                      MrCs2GetHash(&CanMsg), MrCs2GetResponse(&CanMsg),
                      MrCs2GetCommand(&CanMsg), MrCs2GetPrio(&CanMsg));
            }
            MrEthCs2Encode(UdpFrame, &CanMsg);
            if (Cs2ethGetUdpConnected(Data))
            {
               if ((MrIpcGetReceiverSocket(CmdFrame) == MR_IPC_SOCKET_ALL) ||
                   (MrIpcGetReceiverSocket(CmdFrame) == Cs2ethGetOutsideUdpSock(Data)))
               {
                  if (Cs2ethGetVerbose(Data))
                     puts("send as udp");
                  MrEthCs2SendTo(Cs2ethGetOutsideUdpSock(Data),
                                 (struct sockaddr_in *)&(Cs2ethGetClientAddr(Data)),
                                 UdpFrame);
               }
            }
            MengeInitIterator(Cs2ethGetClientIter(Data),
                              Cs2ethGetClients(Data));
            OneClient = (ClientInfo *)MengeFirst(Cs2ethGetClientIter(Data));
            while (OneClient != (ClientInfo *)NULL)
            {
               if ((MrIpcGetReceiverSocket(CmdFrame) == MR_IPC_SOCKET_ALL) ||
                   (MrIpcGetReceiverSocket(CmdFrame) == OneClient->ClientSock))
               {
                 if (Cs2ethGetVerbose(Data))
                     printf("send to tcp socket %d\n", OneClient->ClientSock);
                  MrEthCs2Send(OneClient->ClientSock, UdpFrame);
               }
               OneClient = (ClientInfo *)MengeNext(Cs2ethGetClientIter(Data));
            }
            break;
      }
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
      switch (MrMs2GetCommand(CanMsg))
      {
         case MR_MS2_CMD_PING:
            switch (MrCs2GetNumParamBytes(CanMsg))
            {
               case 0:
                  MrCs2DecPing0(CanMsg);
                  if (Cs2ethGetVerbose(Data))
                     puts("Softwarestand Anfrage/Teilnehmer Ping");
                  break;
               case 8:
                  MrCs2DecPing8(CanMsg, &Uid, &SoftwareVersion, &DeviceId);
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
      MrIpcSetCanHash(&Cmd, MrCs2CalcHash(MrIpcGetCanHash(&Cmd)));
   MrIpcSend(Cs2ethGetClientSock(Data), &Cmd);
   MrIpcExit(&Cmd);
}

static void ForwardOutsideData(Cs2ethStruct *Data, char *UdpFrame,
                               int SenderSocket)
{  ClientInfo *OneClient;

   if ((Cs2ethGetUdpConnected(Data)) &&
       (SenderSocket != Cs2ethGetOutsideUdpSock(Data)))
   {
      if (Cs2ethGetVerbose(Data))
         puts("forward as udp");
      MrEthCs2Send(Cs2ethGetOutsideUdpSock(Data), UdpFrame);
   }
   MengeInitIterator(Cs2ethGetClientIter(Data), Cs2ethGetClients(Data));
   OneClient = (ClientInfo *)MengeFirst(Cs2ethGetClientIter(Data));
   while (OneClient != (ClientInfo *)NULL)
   {
      if (SenderSocket != OneClient->ClientSock)
      {
         if (Cs2ethGetVerbose(Data))
            printf("forward to tcp socket %d\n", OneClient->ClientSock);
         MrEthCs2Send(OneClient->ClientSock, UdpFrame);
      }
      OneClient = (ClientInfo *)MengeNext(Cs2ethGetClientIter(Data));
   }
}

static void HandleOutsideData(Cs2ethStruct *Data)
{  char UdpFrame[MAX_DATAGRAM_SIZE];
   MrCs2CanDataType CanMsg;
   BOOL HaveRecv;
   struct sockaddr_in caddr;

   if (Cs2ethGetSendUdpBc(Data))
   {
      HaveRecv = MrEthCs2Recv(Cs2ethGetOutsideUdpSock(Data), &caddr, UdpFrame);
   }
   else
   {
      Cs2ethSetUdpConnected(Data, TRUE);
      HaveRecv = MrEthCs2Recv(Cs2ethGetOutsideUdpSock(Data),
                              &Cs2ethGetClientAddr(Data),
                              UdpFrame);
   }
   if (HaveRecv)
   {
      if (Cs2ethGetVerbose(Data))
         puts("read new udp frame");
      MrEthCs2Decode(&CanMsg, UdpFrame);
      ForwardOutsideData(Data, UdpFrame, Cs2ethGetOutsideUdpSock(Data));
      ProcessOutsideData(Data, &CanMsg, Cs2ethGetOutsideUdpSock(Data));
   } 
}

static void HandleOutsideTcpConnection(Cs2ethStruct *Data)
{  ClientInfo *NewClient;

   if (Cs2ethGetVerbose(Data))
      puts("accept tcp connection");
   NewClient = (ClientInfo *)malloc(sizeof(ClientInfo));
   if (NewClient != (ClientInfo *)NULL)
   {
      NewClient->ClientSock = MrEthCs2Accept(Cs2ethGetOutsideTcpSock(Data));
      MengeAdd(Cs2ethGetClients(Data), (MengeDataType)NewClient);
   }
}

static BOOL HandleAppData(Cs2ethStruct *Data, ClientInfo *Client)
{  char UdpFrame[MAX_DATAGRAM_SIZE];
   MrCs2CanDataType CanMsg;
   int RcvReturnValue;

   RcvReturnValue = MrEthCs2Recv(Client->ClientSock,
                                 &(Client->ClientAddr),
                                 UdpFrame);
   if (RcvReturnValue == MR_IPC_RCV_ERROR)
   {
      if (Cs2ethGetVerbose(Data))
         puts("Error in recieve from app socket!");
      return(TRUE);
   }
   else if (RcvReturnValue == MR_IPC_RCV_CLOSED)
   {
      if (Cs2ethGetVerbose(Data))
         puts("app socket was closed");
      return(FALSE);
   }
   else
   {
      if (Cs2ethGetVerbose(Data))
         puts("read new app frame");
      MrEthCs2Decode(&CanMsg, UdpFrame);
      ForwardOutsideData(Data, UdpFrame, Client->ClientSock);
      ProcessOutsideData(Data, &CanMsg, Client->ClientSock);
      return(TRUE);
   }
}

void Cs2ethRun(Cs2ethStruct *Data)
{  fd_set ReadFds;
   int RetVal, HighFd;
   struct timeval SelectTimeout;
   time_t Now;
   ClientInfo *OneClient;

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
         if (Cs2ethGetVerbose(Data))
            printf("add outside udp socket %d\n", Cs2ethGetOutsideUdpSock(Data));
         FD_SET(Cs2ethGetOutsideUdpSock(Data), &ReadFds);
         if (Cs2ethGetOutsideUdpSock(Data) > HighFd)
            HighFd = Cs2ethGetOutsideUdpSock(Data);
         if (Cs2ethGetVerbose(Data))
            printf("add outside tcp socket %d\n", Cs2ethGetOutsideTcpSock(Data));
         FD_SET(Cs2ethGetOutsideTcpSock(Data), &ReadFds);
         if (Cs2ethGetOutsideTcpSock(Data) > HighFd)
            HighFd = Cs2ethGetOutsideTcpSock(Data);
         MengeInitIterator(Cs2ethGetClientIter(Data), Cs2ethGetClients(Data));
         OneClient = (ClientInfo *)MengeFirst(Cs2ethGetClientIter(Data));
         while (OneClient != (ClientInfo *)NULL)
         {
            if (Cs2ethGetVerbose(Data))
               printf("add app socket %d\n", OneClient->ClientSock);
            FD_SET(OneClient->ClientSock, &ReadFds);
            if (OneClient->ClientSock > HighFd)
               HighFd = OneClient->ClientSock;
            OneClient = (ClientInfo *)MengeNext(Cs2ethGetClientIter(Data));
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
            if (FD_ISSET(Cs2ethGetOutsideUdpSock(Data), &ReadFds))
            {
               if (Cs2ethGetVerbose(Data))
                  puts("data on udp socket to remote");
               HandleOutsideData(Data);
            }
            if (FD_ISSET(Cs2ethGetOutsideTcpSock(Data), &ReadFds))
            {
               if (Cs2ethGetVerbose(Data))
                  puts("data on tcp socket server to remote");
               HandleOutsideTcpConnection(Data);
            }
            MengeInitIterator(Cs2ethGetClientIter(Data), Cs2ethGetClients(Data));
            OneClient = (ClientInfo *)MengeFirst(Cs2ethGetClientIter(Data));
            while (OneClient != (ClientInfo *)NULL)
            {
               if (Cs2ethGetVerbose(Data))
                  printf("check client socket %d\n", OneClient->ClientSock);
               if (FD_ISSET(OneClient->ClientSock, &ReadFds))
               {
                  if (Cs2ethGetVerbose(Data))
                     puts("data on app socket to remote");
                  if (HandleAppData(Data, OneClient))
                     OneClient = (ClientInfo *)MengeNext(Cs2ethGetClientIter(Data));
                  else
                     OneClient = (ClientInfo *)MengeIterRemove(Cs2ethGetClientIter(Data));
               }
               else
                  OneClient = (ClientInfo *)MengeNext(Cs2ethGetClientIter(Data));
            }
         }
      }
      Stop(Data);
   }
}
