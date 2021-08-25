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
#include "cc_eth.h"

#define MAXPENDING 5

static int ClientCompare(void *d1, void *d2)
{  ClientInfo *p1, *p2;

   p1 = (ClientInfo *)d1;
   p2 = (ClientInfo *)d2;
   return(p1->ClientSock - p2->ClientSock);
}

static int CcEthGetFd(void *private)
{  CcEthStruct *Data;
   int ReturnFd;

   Data = (CcEthStruct *)private;
   switch (CcEthGetFdPos(Data))
   {
      case FD_POS_TCP_SERVER:
         ReturnFd = CcEthGetOutsideTcpSock(Data);
         MengeInitIterator(CcEthGetFdSearchIter(Data),
                           CcEthGetClients(Data));
         CcEthSetFdSearchClient(Data,
                               (ClientInfo *)MengeFirst(CcEthGetFdSearchIter(Data)));
         if (CcEthGetFdSearchClient(Data) == (ClientInfo *)NULL)
            CcEthSetFdPos(Data, FD_POS_ENDE);
         else
            CcEthSetFdPos(Data, FD_POS_TCP_CLIENT);
         break;
      case FD_POS_TCP_CLIENT:
         ReturnFd = CcEthGetFdSearchClient(Data)->ClientSock;
         CcEthSetFdSearchClient(Data,
                               (ClientInfo *)MengeNext(CcEthGetFdSearchIter(Data)));
         if (CcEthGetFdSearchClient(Data) == (ClientInfo *)NULL)
            CcEthSetFdPos(Data, FD_POS_ENDE);
         else
            CcEthSetFdPos(Data, FD_POS_TCP_CLIENT);
         break;
      case FD_POS_ENDE:
         ReturnFd = IOFKT_INVALID_FD;
         CcEthSetFdPos(Data, FD_POS_TCP_SERVER);
         break;
   }
   return(ReturnFd);
}

static BOOL CcEthOpen(void *private)
{  CcEthStruct *Data;
   struct sockaddr_in ServAddr;

   Data = (CcEthStruct *)private;
   if (CcEthGetVerbose(Data))
      puts("start udp server");
   CcEthSetOutsideTcpSock(Data, socket(PF_INET, SOCK_STREAM, IPPROTO_TCP));
   if (CcEthGetOutsideTcpSock(Data) >= 0)
   {
      /* Construct local address structure */
      memset(&ServAddr, 0, sizeof(ServAddr));
      ServAddr.sin_family = AF_INET;
      ServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
      ServAddr.sin_port = htons(CcEthGetOutsideTcpPort(Data));
      /* Bind to the local address */
      if (bind(CcEthGetOutsideTcpSock(Data),
               (struct sockaddr *) &ServAddr, sizeof(ServAddr)) < 0)
      {
         close(CcEthGetOutsideTcpSock(Data));
         CcEthSetOutsideTcpSock(Data, IOFKT_INVALID_FD);
         return(FALSE);
      }
      else
      {
         /* Mark the socket so it will listen for incoming connections */
         if (listen(CcEthGetOutsideTcpSock(Data), MAXPENDING) < 0)
         {
            close(CcEthGetOutsideTcpSock(Data));
            CcEthSetOutsideTcpSock(Data, IOFKT_INVALID_FD);
            return(FALSE);
         }
         else
         {
            CcEthSetFdPos(Data, FD_POS_TCP_SERVER);
            return(TRUE);
         }
      }
   }
   else
   {
      return(FALSE);
   }
}

static void CcEthClose(void *private)
{  CcEthStruct *Data;
   ClientInfo *OneClient;

   Data = (CcEthStruct *)private;
   MengeInitIterator(CcEthGetClientIter(Data), CcEthGetClients(Data));
   OneClient = (ClientInfo *)MengeFirst(CcEthGetClientIter(Data));
   while (OneClient != (ClientInfo *)NULL)
   {
      MrEthCs2Close(OneClient->ClientSock);
      OneClient = (ClientInfo *)MengeNext(CcEthGetClientIter(Data));
   }
   MengeDestroy(CcEthGetClients(Data));
   CcEthSetClients(Data, MengeCreate());
   if (CcEthGetClients(Data) != (Menge *)NULL)
   {
      MengeInit(CcEthGetClients(Data), ClientCompare, free);
      CcEthSetOutsideTcpSock(Data, IOFKT_INVALID_FD);
      memset((void *)(&(CcEthGetClientAddr(Data))), 0,
             sizeof(struct sockaddr_in));
   }
   close(CcEthGetOutsideTcpSock(Data));
   CcEthSetOutsideTcpSock(Data, IOFKT_INVALID_FD);
   memset((void *)(&(CcEthGetClientAddr(Data))), 0,
          sizeof(struct sockaddr_in));
}

static void ForwardOutsideData(CcEthStruct *Data, char *UdpFrame,
                               int SenderSocket)
{  ClientInfo *OneClient;

   MengeInitIterator(CcEthGetClientIter(Data), CcEthGetClients(Data));
   OneClient = (ClientInfo *)MengeFirst(CcEthGetClientIter(Data));
   while (OneClient != (ClientInfo *)NULL)
   {
      if (SenderSocket != OneClient->ClientSock)
      {
         if (CcEthGetVerbose(Data))
            printf("forward to tcp socket %d\n", OneClient->ClientSock);
         MrEthCs2Send(OneClient->ClientSock, UdpFrame);
      }
      OneClient = (ClientInfo *)MengeNext(CcEthGetClientIter(Data));
   }
}

static BOOL CcEthRead(void *private, int fd, BOOL PendingData,
                      MrCs2CanDataType *CanMsg)
{  CcEthStruct *Data;
   char UdpFrame[MR_CS2_UDP_LENGTH];
   BOOL HaveRecv;

   Data = (CcEthStruct *)private;
   if (PendingData)
   {
      HaveRecv = FALSE;
   }
   else
   {
      if (CcEthGetOutsideTcpSock(Data) == fd)
      {  ClientInfo *NewClient;

         if (CcEthGetVerbose(Data))
            puts("accept tcp connection");
         NewClient = (ClientInfo *)malloc(sizeof(ClientInfo));
         if (NewClient != (ClientInfo *)NULL)
         {
            NewClient->ClientSock = MrEthCs2Accept(CcEthGetOutsideTcpSock(Data));
            MengeAdd(CcEthGetClients(Data), (MengeDataType)NewClient);
         }
         HaveRecv = FALSE;
      }
      else
      {  ClientInfo *OneClient;

         MengeInitIterator(CcEthGetClientIter(Data), CcEthGetClients(Data));
         OneClient = (ClientInfo *)MengeFirst(CcEthGetClientIter(Data));
         while (OneClient != (ClientInfo *)NULL)
         {
            if (OneClient->ClientSock == fd)
            {  int RcvReturnValue;

               RcvReturnValue = MrEthCs2Recv(OneClient->ClientSock,
                                             &(OneClient->ClientAddr),
                                             UdpFrame);
               if (RcvReturnValue == MR_ETH_CS2_RCV_ERROR)
               {
                  if (CcEthGetVerbose(Data))
                     puts("Error in recieve from app socket!");
                  OneClient = (ClientInfo *)MengeNext(CcEthGetClientIter(Data));
                  HaveRecv = FALSE;
               }
               else if (RcvReturnValue == MR_ETH_CS2_RCV_CLOSED)
               {
                  if (CcEthGetVerbose(Data))
                     puts("app socket was closed");
                  OneClient = (ClientInfo *)MengeIterRemove(CcEthGetClientIter(Data));
                  HaveRecv = FALSE;
               }
               else
               {
                  MrEthCs2Decode(CanMsg, UdpFrame);
                  ForwardOutsideData(Data, UdpFrame, fd);
                  OneClient = (ClientInfo *)MengeNext(CcEthGetClientIter(Data));
                  HaveRecv = TRUE;
               }
            }
         }
      }
   }
   return(HaveRecv);
}

static BOOL CcEthWrite(void *private, int ReceiverSocket,
                       MrCs2CanDataType *CanMsg)
{  CcEthStruct *Data;
   char UdpFrame[MR_CS2_UDP_LENGTH];
   ClientInfo *OneClient;

   Data = (CcEthStruct *)private;
   MengeInitIterator(CcEthGetClientIter(Data),
                     CcEthGetClients(Data));
   OneClient = (ClientInfo *)MengeFirst(CcEthGetClientIter(Data));
   while (OneClient != (ClientInfo *)NULL)
   {
      if ((ReceiverSocket == MR_IPC_SOCKET_ALL) ||
          (ReceiverSocket == OneClient->ClientSock))
      {
         if (CcEthGetVerbose(Data))
            printf("send to tcp socket %d\n", OneClient->ClientSock);
         MrEthCs2Send(OneClient->ClientSock, UdpFrame);
      }
      OneClient = (ClientInfo *)MengeNext(CcEthGetClientIter(Data));
   }
   return(TRUE);
}

static IoFktStruct CcEthIoFunctions =
{
   NULL,
   CcEthGetFd,
   CcEthOpen,
   CcEthClose,
   CcEthRead,
   CcEthWrite
};

IoFktStruct *CcEthInit(BOOL Verbose, int Port)
{  CcEthStruct *Data;
   IoFktStruct *IoFunctions;

   Data = (CcEthStruct *)malloc(sizeof(CcEthStruct));
   if (Data != (CcEthStruct *)NULL)
   {
      CcEthSetClients(Data, MengeCreate());
      if (CcEthGetClients(Data) != (Menge *)NULL)
      {
         CcEthSetClientIter(Data, MengeCreateIter());
         if (CcEthGetClientIter(Data) != (MengeIterator *)NULL)
         {
            CcEthSetFdSearchIter(Data, MengeCreateIter());
            if (CcEthGetFdSearchIter(Data) != (MengeIterator *)NULL)
            {
               MengeInit(CcEthGetClients(Data), ClientCompare, free);
               CcEthSetVerbose(Data, Verbose);
               CcEthSetOutsideTcpPort(Data, Port);
               CcEthSetOutsideTcpSock(Data, IOFKT_INVALID_FD);
               CcEthSetClientTcpSock(Data, IOFKT_INVALID_FD);
               memset((void *)(&(CcEthGetClientAddr(Data))), 0,
                      sizeof(struct sockaddr_in));
               CcEthIoFunctions.private = (void *)Data;
               IoFunctions = &CcEthIoFunctions;
            }
            else
            {
               MengeDestroyIterator(CcEthGetClientIter(Data));
               MengeDestroy(CcEthGetClients(Data));
               free(Data);
               IoFunctions = (IoFktStruct *)NULL;
            }
         }
         else
         {
            MengeDestroy(CcEthGetClients(Data));
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

void CcEthExit(IoFktStruct *Data)
{
   if (Data->private != (void *)NULL)
   {  CcEthStruct *CcEthData;

      CcEthData = (CcEthStruct *)(Data->private);
      if (CcEthGetFdSearchIter(CcEthData) != (MengeIterator *)NULL)
      {
         MengeDestroyIterator(CcEthGetFdSearchIter(CcEthData));
      }
      if (CcEthGetClientIter(CcEthData) != (MengeIterator *)NULL)
      {
         MengeDestroyIterator(CcEthGetClientIter(CcEthData));
      }
      if (CcEthGetClients(CcEthData) != (Menge *)NULL)
      {
         MengeDestroy(CcEthGetClients(CcEthData));
      }
      free(Data->private);
   }
}
