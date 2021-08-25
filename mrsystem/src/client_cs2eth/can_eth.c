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
#include "can_eth.h"

static int ClientCompare(void *d1, void *d2)
{  ClientInfo *p1, *p2;

   p1 = (ClientInfo *)d1;
   p2 = (ClientInfo *)d2;
   return(p1->ClientSock - p2->ClientSock);
}

static int CanEthGetFd(void *private)
{  CanEthStruct *Data;
   int ReturnFd;

   Data = (CanEthStruct *)private;
   switch (CanEthGetFdPos(Data))
   {
      case FD_POS_UDP:
         ReturnFd = CanEthGetOutsideUdpSock(Data);
         CanEthSetFdPos(Data, FD_POS_TCP_SERVER);
         break;
      case FD_POS_TCP_SERVER:
         ReturnFd = CanEthGetOutsideTcpSock(Data);
         MengeInitIterator(CanEthGetFdSearchIter(Data), CanEthGetClients(Data));
         CanEthSetFdSearchClient(Data,
                                 (ClientInfo *)MengeFirst(CanEthGetFdSearchIter(Data)));
         if (CanEthGetFdSearchClient(Data) == (ClientInfo *)NULL)
            CanEthSetFdPos(Data, FD_POS_ENDE);
         else
            CanEthSetFdPos(Data, FD_POS_TCP_CLIENT);
         break;
      case FD_POS_TCP_CLIENT:
         ReturnFd = CanEthGetFdSearchClient(Data)->ClientSock;
         CanEthSetFdSearchClient(Data,
                                 (ClientInfo *)MengeNext(CanEthGetFdSearchIter(Data)));
         if (CanEthGetFdSearchClient(Data) == (ClientInfo *)NULL)
            CanEthSetFdPos(Data, FD_POS_ENDE);
         else
            CanEthSetFdPos(Data, FD_POS_TCP_CLIENT);
         break;
      case FD_POS_ENDE:
         ReturnFd = IOFKT_INVALID_FD;
         CanEthSetFdPos(Data, FD_POS_UDP);
         break;
   }
   return(ReturnFd);
}

static BOOL CanEthOpen(void *private)
{  CanEthStruct *Data;

   Data = (CanEthStruct *)private;
   if (CanEthGetVerbose(Data))
      puts("start tcp server");
   CanEthSetOutsideTcpSock(Data, MrEthCs2StartAppServer());
   if (CanEthGetOutsideTcpSock(Data) >= 0)
   {
      if (CanEthGetVerbose(Data))
         puts("start udp server");
      if (CanEthGetSendUdpBc(Data))
      {
         CanEthSetOutsideUdpSock(Data, MrEthCs2StartBcServer());
         if (CanEthGetOutsideUdpSock(Data) >= 0)
         {
            MrEthCs2MkBcAddr(&CanEthGetClientAddr(Data), CanEthGetBcIp(Data));
            CanEthSetUdpConnected(Data, TRUE);
            CanEthSetFdPos(Data, FD_POS_UDP);
            return(TRUE);
         }
         else
         {
            MrEthCs2Close(CanEthGetOutsideTcpSock(Data));
            return(FALSE);
         }
      }
      else
      {
         CanEthSetOutsideUdpSock(Data, MrEthCs2StartServer());
         if (CanEthGetOutsideUdpSock(Data) >= 0)
         {
            CanEthSetFdPos(Data, FD_POS_UDP);
            return(TRUE);
         }
         else
         {
            return(FALSE);
         }
      }
   }
   else
   {
      return(FALSE);
   }
}

static void CanEthClose(void *private)
{  CanEthStruct *Data;
   ClientInfo *OneClient;

   Data = (CanEthStruct *)private;
   MengeInitIterator(CanEthGetClientIter(Data), CanEthGetClients(Data));
   OneClient = (ClientInfo *)MengeFirst(CanEthGetClientIter(Data));
   while (OneClient != (ClientInfo *)NULL)
   {
      MrEthCs2Close(OneClient->ClientSock);
      OneClient = (ClientInfo *)MengeNext(CanEthGetClientIter(Data));
   }
   MengeDestroy(CanEthGetClients(Data));
   CanEthSetClients(Data, MengeCreate());
   if (CanEthGetClients(Data) != (Menge *)NULL)
   {
      MengeInit(CanEthGetClients(Data), ClientCompare, free);
   }
   close(CanEthGetOutsideTcpSock(Data));
   CanEthSetOutsideTcpSock(Data, IOFKT_INVALID_FD);
   close(CanEthGetOutsideUdpSock(Data));
   CanEthSetOutsideUdpSock(Data, IOFKT_INVALID_FD);
   CanEthSetUdpConnected(Data, FALSE);
   memset((void *)(&(CanEthGetClientAddr(Data))), 0,
          sizeof(struct sockaddr_in));
}

static void ForwardOutsideData(CanEthStruct *Data, char *UdpFrame,
                               int SenderSocket)
{  ClientInfo *OneClient;

   if (CanEthGetUdpConnected(Data) &&
       (SenderSocket != CanEthGetOutsideUdpSock(Data)))
   {
      if (CanEthGetVerbose(Data))
         puts("forward as udp");
      MrEthCs2Send(CanEthGetOutsideUdpSock(Data), UdpFrame);
   }
   MengeInitIterator(CanEthGetClientIter(Data), CanEthGetClients(Data));
   OneClient = (ClientInfo *)MengeFirst(CanEthGetClientIter(Data));
   while (OneClient != (ClientInfo *)NULL)
   {
      if (SenderSocket != OneClient->ClientSock)
      {
         if (CanEthGetVerbose(Data))
            printf("forward to tcp socket %d\n", OneClient->ClientSock);
         MrEthCs2Send(OneClient->ClientSock, UdpFrame);
      }
      OneClient = (ClientInfo *)MengeNext(CanEthGetClientIter(Data));
   }
}

static BOOL CanEthRead(void *private, int fd, BOOL PendingData,
                       MrCs2CanDataType *CanMsg)
{  CanEthStruct *Data;
   char UdpFrame[MR_CS2_UDP_LENGTH];
   BOOL HaveRecv;

   Data = (CanEthStruct *)private;
   HaveRecv = FALSE;
   if (!PendingData)
   {
      if (CanEthGetOutsideUdpSock(Data) == fd)
      {  struct sockaddr_in caddr;

         if (CanEthGetVerbose(Data))
            puts("data on udp socket to remote");
         if (CanEthGetSendUdpBc(Data))
         {  int Ret;

            Ret = MrEthCs2Recv(CanEthGetOutsideUdpSock(Data),
                                &caddr, UdpFrame);
            if (Ret == MR_ETH_CS2_RCV_OK)
            {
               HaveRecv = TRUE;
               MrEthCs2Decode(CanMsg, UdpFrame);
               ForwardOutsideData(Data, UdpFrame, fd);
            }
         }
         else
         {  int Ret;

            CanEthSetUdpConnected(Data, TRUE);
            Ret = MrEthCs2Recv(CanEthGetOutsideUdpSock(Data),
                               &CanEthGetClientAddr(Data),
                               UdpFrame);
            if (Ret == MR_ETH_CS2_RCV_OK)
            {
               HaveRecv = TRUE;
               MrEthCs2Decode(CanMsg, UdpFrame);
               ForwardOutsideData(Data, UdpFrame, fd);
            }
         }
      }
      else if (CanEthGetOutsideTcpSock(Data) == fd)
      {  ClientInfo *NewClient;

         if (CanEthGetVerbose(Data))
            puts("accept tcp connection");
         NewClient = (ClientInfo *)malloc(sizeof(ClientInfo));
         if (NewClient != (ClientInfo *)NULL)
         {
            ClientInfoSetClientSock(NewClient,
                                    MrEthCs2Accept(CanEthGetOutsideTcpSock(Data)));
            MengeAdd(CanEthGetClients(Data), (MengeDataType)NewClient);
         }
         HaveRecv = FALSE;
      }
      else
      {  ClientInfo *OneClient;

         HaveRecv = FALSE;
         MengeInitIterator(CanEthGetClientIter(Data), CanEthGetClients(Data));
         OneClient = (ClientInfo *)MengeFirst(CanEthGetClientIter(Data));
         while (OneClient != (ClientInfo *)NULL)
         {
            if (OneClient->ClientSock == fd)
            {  int RcvReturnValue;

               RcvReturnValue = MrEthCs2Recv(OneClient->ClientSock,
                                             &ClientInfoGetClientAddr(OneClient),
                                             UdpFrame);
               if (RcvReturnValue == MR_ETH_CS2_RCV_ERROR)
               {
                  if (CanEthGetVerbose(Data))
                     puts("Error in recieve from app socket!");
                  OneClient = (ClientInfo *)MengeNext(CanEthGetClientIter(Data));
               }
               else if (RcvReturnValue == MR_ETH_CS2_RCV_CLOSED)
               {
                  if (CanEthGetVerbose(Data))
                     puts("app socket was closed");
                  OneClient = (ClientInfo *)MengeIterRemove(CanEthGetClientIter(Data));
               }
               else
               {
                  MrEthCs2Decode(CanMsg, UdpFrame);
                  ForwardOutsideData(Data, UdpFrame, fd);
                  OneClient = (ClientInfo *)MengeNext(CanEthGetClientIter(Data));
                  HaveRecv = TRUE;
               }
            }
            else
            {
               OneClient = (ClientInfo *)MengeNext(CanEthGetClientIter(Data));
            }
         }
      }
   }
   return(HaveRecv);
}

static BOOL CanEthWrite(void *private, int ReceiverSocket,
                        MrCs2CanDataType *CanMsg)
{  CanEthStruct *Data;
   char UdpFrame[MR_CS2_UDP_LENGTH];
   ClientInfo *OneClient;

   Data = (CanEthStruct *)private;
   MrEthCs2Encode(UdpFrame, CanMsg);
   if (CanEthGetSendUdpBc(Data) || CanEthGetUdpConnected(Data))
   {
      if ((ReceiverSocket == MR_IPC_SOCKET_ALL) ||
          (ReceiverSocket == CanEthGetOutsideUdpSock(Data)))
      {
         if (CanEthGetVerbose(Data))
            puts("send as udp");
         if (CanEthGetUdpConnected(Data))
         {
            MrEthCs2SendTo(CanEthGetOutsideUdpSock(Data),
                           (struct sockaddr_in *)&(CanEthGetClientAddr(Data)),
                           UdpFrame);
         }
         else
         {  struct sockaddr_in BcAddr;

            MrEthCs2MkBcAddr(&BcAddr, CanEthGetBcIp(Data));
            MrEthCs2SendTo(CanEthGetOutsideUdpSock(Data), &BcAddr, UdpFrame);
         }
      }
   }
   MengeInitIterator(CanEthGetClientIter(Data),
                     CanEthGetClients(Data));
   OneClient = (ClientInfo *)MengeFirst(CanEthGetClientIter(Data));
   while (OneClient != (ClientInfo *)NULL)
   {
      if ((ReceiverSocket == MR_IPC_SOCKET_ALL) ||
          (ReceiverSocket == ClientInfoGetClientSock(OneClient)))
      {
         if (CanEthGetVerbose(Data))
            printf("send to tcp socket %d\n", OneClient->ClientSock);
         MrEthCs2Send(ClientInfoGetClientSock(OneClient), UdpFrame);
      }
      OneClient = (ClientInfo *)MengeNext(CanEthGetClientIter(Data));
   }
   return(TRUE);
}

static IoFktStruct CanEthIoFunctions =
{
   NULL,
   CanEthGetFd,
   CanEthOpen,
   CanEthClose,
   CanEthRead,
   CanEthWrite
};

IoFktStruct *CanEthInit(BOOL Verbose, BOOL SendBc, char *BcAddr)
{  CanEthStruct *Data;
   IoFktStruct *IoFunctions;

   Data = (CanEthStruct *)malloc(sizeof(CanEthStruct));
   if (Data != (CanEthStruct *)NULL)
   {
      CanEthSetClients(Data, MengeCreate());
      if (CanEthGetClients(Data) != (Menge *)NULL)
      {
         CanEthSetClientIter(Data, MengeCreateIter());
         if (CanEthGetClientIter(Data) != (MengeIterator *)NULL)
         {
            CanEthSetFdSearchIter(Data, MengeCreateIter());
            if (CanEthGetFdSearchIter(Data) != (MengeIterator *)NULL)
            {
               MengeInit(CanEthGetClients(Data), ClientCompare, free);
               CanEthSetVerbose(Data, Verbose);
               CanEthSetSendUdpBc(Data, SendBc);
               CanEthSetUdpConnected(Data, FALSE);
               CanEthSetBcIp(Data, BcAddr);
               CanEthSetOutsideUdpSock(Data, IOFKT_INVALID_FD);
               CanEthSetOutsideTcpSock(Data, IOFKT_INVALID_FD);
               memset((void *)(&(CanEthGetClientAddr(Data))), 0,
                      sizeof(struct sockaddr_in));
               CanEthIoFunctions.private = (void *)Data;
               IoFunctions = &CanEthIoFunctions;
            }
            else
            {
               MengeDestroyIterator(CanEthGetClientIter(Data));
               MengeDestroy(CanEthGetClients(Data));
               free(Data);
               IoFunctions = (IoFktStruct *)NULL;
            }
         }
         else
         {
            MengeDestroy(CanEthGetClients(Data));
            free(Data);
            IoFunctions = (IoFktStruct *)NULL;
         }
      }
      else
      {
         free(Data);
         IoFunctions = (IoFktStruct *)NULL;
      }
   }
   else
   {
      IoFunctions = (IoFktStruct *)NULL;
   }
   return(IoFunctions);
}

void CanEthExit(IoFktStruct *Data)
{
   if (Data->private != (void *)NULL)
   {  CanEthStruct *CanEthData;

      CanEthData = (CanEthStruct *)(Data->private);
      if (CanEthGetFdSearchIter(CanEthData) != (MengeIterator *)NULL)
      {
         MengeDestroyIterator(CanEthGetFdSearchIter(CanEthData));
      }
      if (CanEthGetClientIter(CanEthData) != (MengeIterator *)NULL)
      {
         MengeDestroyIterator(CanEthGetClientIter(CanEthData));
      }
      if (CanEthGetClients(CanEthData) != (Menge *)NULL)
      {
         MengeDestroy(CanEthGetClients(CanEthData));
      }
      free(Data->private);
   }
}
