#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <linux/can.h>
#include <string.h>
#include <errno.h>
#include <menge.h>
#include <boolean.h>
#include <mr_ipc.h>
#include <mr_cs2ms2.h>
#include "srcp.h"

#define MAXPENDING 5
#define MAX_SRCP_LINE_LENGT 1001
#define SRCP_PORT 4303
#define SRCP_SERVER_WELCOME_MSG "srcpd V1.0; SRCP 0.8.4; SRCPOTHER 0.8.3\n"

static int ClientCompare(void *d1, void *d2)
{  ClientInfo *p1, *p2;

   p1 = (ClientInfo *)d1;
   p2 = (ClientInfo *)d2;
   return(p1->ClientSock - p2->ClientSock);
}

static void ClientDestroy(void *d)
{  ClientInfo *p;

   p = (ClientInfo *)d;
   close(SrcpClientGetClientSock(p));
   FsmExit(SrcpClientGetStateMachine(p));
   FsmDestroy(SrcpClientGetStateMachine(p));
   free(p);
}

static int SrcpGetFd(void *private)
{  SrcpStruct *Data;
   int ReturnFd;

   Data = (SrcpStruct *)private;
   switch (SrcpGetFdPos(Data))
   {
      case FD_POS_TCP_SERVER:
         ReturnFd = SrcpGetOutsideTcpSock(Data);
         MengeInitIterator(SrcpGetFdSearchIter(Data), SrcpGetClients(Data));
         SrcpSetFdSearchClient(Data,
                               (ClientInfo *)MengeFirst(SrcpGetFdSearchIter(Data)));
         if (SrcpGetFdSearchClient(Data) == (ClientInfo *)NULL)
            SrcpSetFdPos(Data, FD_POS_ENDE);
         else
            SrcpSetFdPos(Data, FD_POS_TCP_CLIENT);
         break;
      case FD_POS_TCP_CLIENT:
         ReturnFd = SrcpGetFdSearchClient(Data)->ClientSock;
         SrcpSetFdSearchClient(Data,
                               (ClientInfo *)MengeNext(SrcpGetFdSearchIter(Data)));
         if (SrcpGetFdSearchClient(Data) == (ClientInfo *)NULL)
            SrcpSetFdPos(Data, FD_POS_ENDE);
         else
            SrcpSetFdPos(Data, FD_POS_TCP_CLIENT);
         break;
      case FD_POS_ENDE:
         ReturnFd = IOFKT_INVALID_FD;
         SrcpSetFdPos(Data, FD_POS_TCP_SERVER);
         break;
   }
   return(ReturnFd);
}

static BOOL SrcpOpen(void *private)
{  SrcpStruct *Data;
   struct sockaddr_in ServAddr;

   Data = (SrcpStruct *)private;
   if (SrcpGetVerbose(Data))
      puts("start udp server");
   SrcpSetOutsideTcpSock(Data, socket(PF_INET, SOCK_STREAM, IPPROTO_TCP));
   if (SrcpGetOutsideTcpSock(Data) >= 0)
   {
      /* Construct local address structure */
      memset(&ServAddr, 0, sizeof(ServAddr));
      ServAddr.sin_family = AF_INET;
      ServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
      ServAddr.sin_port = htons(SRCP_PORT);
      /* Bind to the local address */
      if (bind(SrcpGetOutsideTcpSock(Data),
               (struct sockaddr *) &ServAddr, sizeof(ServAddr)) < 0)
      {
         close(SrcpGetOutsideTcpSock(Data));
         SrcpSetOutsideTcpSock(Data, IOFKT_INVALID_FD);
         return(FALSE);
      }
      else
      {
         /* Mark the socket so it will listen for incoming connections */
         if (listen(SrcpGetOutsideTcpSock(Data), MAXPENDING) < 0)
         {
            close(SrcpGetOutsideTcpSock(Data));
            SrcpSetOutsideTcpSock(Data, IOFKT_INVALID_FD);
            return(FALSE);
         }
         else
         {
            SrcpSetFdPos(Data, FD_POS_TCP_SERVER);
            SrcpSetNumPending(Data, 0);
            return(TRUE);
         }
      }
   }
   else
   {
      return(FALSE);
   }
}

static void SrcpClose(void *private)
{  SrcpStruct *Data;
   ClientInfo *OneClient;

   Data = (SrcpStruct *)private;
   MengeInitIterator(SrcpGetClientIter(Data), SrcpGetClients(Data));
   OneClient = (ClientInfo *)MengeFirst(SrcpGetClientIter(Data));
   while (OneClient != (ClientInfo *)NULL)
   {
      MrEthCs2Close(OneClient->ClientSock);
      OneClient = (ClientInfo *)MengeNext(SrcpGetClientIter(Data));
   }
   MengeDestroy(SrcpGetClients(Data));
   SrcpSetClients(Data, MengeCreate());
   if (SrcpGetClients(Data) != (Menge *)NULL)
   {
      MengeInit(SrcpGetClients(Data), ClientCompare, free);
   }
   close(SrcpGetOutsideTcpSock(Data));
   SrcpSetOutsideTcpSock(Data, IOFKT_INVALID_FD);
}

static int RecvLine(int sockfd, void *buf, size_t len,
                    struct sockaddr *src_addr, socklen_t *addrlen)
{  char chrbuf[2];
   int ret, aktlen;

   aktlen = 0;
   ret = recvfrom(sockfd, chrbuf, 1, MSG_DONTWAIT, src_addr, addrlen);
   while ((ret > 0) && (aktlen < len) && (chrbuf[0] != '\n'))
   {
      ((char *)buf)[aktlen] = chrbuf[0];
      aktlen++;
      ret = recvfrom(sockfd, chrbuf, 1, MSG_DONTWAIT, src_addr, addrlen);
   }
   ((char *)buf)[aktlen] = '\0';
   if (ret < 0)
   {
      /* Error in read, maybe no data left */
      return MR_ETH_CS2_RCV_ERROR;
   }
   else if (ret == 0)
   {
      /* socket was closed at remote side */
      return MR_ETH_CS2_RCV_CLOSED;
   }
   else
   {
      /* we have one complete message */
      return aktlen;
   }
}

static BOOL SrcpRead(void *private, int fd, BOOL PendingData,
                     MrCs2CanDataType *CanMsg)
{  SrcpStruct *Data;
   char Line[MAX_SRCP_LINE_LENGT];
   BOOL HaveRecv;

   Data = (SrcpStruct *)private;
   HaveRecv = FALSE;
   if (PendingData)
   {
      if (SrcpGetNumPending(Data) > 0)
      {
         memcpy(CanMsg, SrcpGetPendingCanMsg(Data, 0),
                sizeof(MrCs2CanDataType));
         SrcpSetNumPending(Data, SrcpGetNumPending(Data) - 1);
         memmove(SrcpGetPendingCanMsg(Data, 0), SrcpGetPendingCanMsg(Data, 1),
                 sizeof(MrCs2CanDataType) * SrcpGetNumPending(Data));
         HaveRecv = TRUE;
      }
      else
      {
         HaveRecv = FALSE;
      }
   }
   else
   {
      if (SrcpGetOutsideTcpSock(Data) == fd)
      {  ClientInfo *NewClient;

         if (SrcpGetVerbose(Data))
            puts("accept tcp connection");
         NewClient = (ClientInfo *)malloc(sizeof(ClientInfo));
         if (NewClient != (ClientInfo *)NULL)
         {
            SrcpClientSetStateMachine(NewClient, FsmCreate());
            if (SrcpClientGetStateMachine(NewClient) != (FsmStruct *)NULL)
            {
               SrcpClientSetClientSock(NewClient,
                                       MrEthCs2Accept(SrcpGetOutsideTcpSock(Data)));
               MengeAdd(SrcpGetClients(Data), (MengeDataType)NewClient);
               memset((void *)(&(SrcpClientGetClientAddr(NewClient))), 0,
                      sizeof(struct sockaddr_in));
               SrcpInitFsm(Data, NewClient);
               SrcpClientSetClientMode(NewClient, SCRP_CLIENT_MODE_COMMAND);
               SrcpClientSetLastMsgTstamp(NewClient, time(NULL));
               send(SrcpClientGetClientSock(NewClient),
                    SRCP_SERVER_WELCOME_MSG, strlen(SRCP_SERVER_WELCOME_MSG),
                    0);
            }
            else
            {
               free(NewClient);
            }
         }
         HaveRecv = FALSE;
      }
      else
      {  ClientInfo *OneClient;

         MengeInitIterator(SrcpGetClientIter(Data), SrcpGetClients(Data));
         OneClient = (ClientInfo *)MengeFirst(SrcpGetClientIter(Data));
         while (OneClient != (ClientInfo *)NULL)
         {
            if (OneClient->ClientSock == fd)
            {  int RcvReturnValue;
               unsigned int ClntAddrLen;

               ClntAddrLen = sizeof(SrcpClientGetClientAddr(OneClient));
               RcvReturnValue = RecvLine(SrcpClientGetClientSock(OneClient),
                                         Line, MAX_SRCP_LINE_LENGT,
                                         (struct sockaddr *)&(SrcpClientGetClientAddr(OneClient)),
                                         &ClntAddrLen);
               if (RcvReturnValue == MR_ETH_CS2_RCV_ERROR)
               {
                  if (SrcpGetVerbose(Data))
                     puts("Error in receive from app socket!");
                  OneClient = (ClientInfo *)MengeNext(SrcpGetClientIter(Data));
               }
               else if (RcvReturnValue == MR_ETH_CS2_RCV_CLOSED)
               {
                  if (SrcpGetVerbose(Data))
                     puts("app socket was closed");
                  OneClient = (ClientInfo *)MengeIterRemove(SrcpGetClientIter(Data));
               }
               else
               {
                  if (SrcpGetVerbose(Data))
                     printf("receive cmd %s\n", Line);
                  HaveRecv = SrcpDoFsm(Data, OneClient, Line, RcvReturnValue, CanMsg);
                  OneClient = (ClientInfo *)MengeNext(SrcpGetClientIter(Data));
               }
            }
            else
            {
               OneClient = (ClientInfo *)MengeNext(SrcpGetClientIter(Data));
            }
         }
      }
   }
   return(HaveRecv);
}

static BOOL SrcpWrite(void *private, int ReceiverSocket,
                      MrCs2CanDataType *CanMsg)
{  SrcpStruct *Data;
   char UdpFrame[MR_CS2_UDP_LENGTH];
   ClientInfo *OneClient;

   Data = (SrcpStruct *)private;
   MrEthCs2Encode(UdpFrame, CanMsg);
   MengeInitIterator(SrcpGetClientIter(Data), SrcpGetClients(Data));
   OneClient = (ClientInfo *)MengeFirst(SrcpGetClientIter(Data));
   while (OneClient != (ClientInfo *)NULL)
   {
      if ((ReceiverSocket == MR_IPC_SOCKET_ALL) ||
          (ReceiverSocket == OneClient->ClientSock))
      {
         if (SrcpGetVerbose(Data))
            printf("send to tcp socket %d\n", OneClient->ClientSock);
         MrEthCs2Send(OneClient->ClientSock, UdpFrame);
      }
      OneClient = (ClientInfo *)MengeNext(SrcpGetClientIter(Data));
   }
   return(TRUE);
}

static IoFktStruct SrcpIoFunctions =
{
   NULL,
   SrcpGetFd,
   SrcpOpen,
   SrcpClose,
   SrcpRead,
   SrcpWrite
};

IoFktStruct *SrcpInit(BOOL Verbose)
{  SrcpStruct *Data;
   IoFktStruct *IoFunctions;

   Data = (SrcpStruct *)malloc(sizeof(SrcpStruct));
   if (Data != (SrcpStruct *)NULL)
   {
      SrcpSetVerbose(Data, Verbose);
      SrcpSetClients(Data, MengeCreate());
      if (SrcpGetClients(Data) != (Menge *)NULL)
      {
         SrcpSetClientIter(Data, MengeCreateIter());
         if (SrcpGetClientIter(Data) != (MengeIterator *)NULL)
         {
            SrcpSetFdSearchIter(Data, MengeCreateIter());
            if (SrcpGetFdSearchIter(Data) != (MengeIterator *)NULL)
            {
               MengeInit(SrcpGetClients(Data), ClientCompare, ClientDestroy);
               SrcpSetOutsideTcpSock(Data, IOFKT_INVALID_FD);
               SrcpIoFunctions.private = (void *)Data;
               IoFunctions = &SrcpIoFunctions;
            }
            else
            {
               MengeDestroyIterator(SrcpGetClientIter(Data));
               MengeDestroy(SrcpGetClients(Data));
               free(Data);
               IoFunctions = (IoFktStruct *)NULL;
            }
         }
         else
         {
            MengeDestroy(SrcpGetClients(Data));
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

void SrcpExit(IoFktStruct *Data)
{  SrcpStruct *SrcpData;

   if (Data->private != (void *)NULL)
   {
      SrcpData = Data->private;
      if (SrcpGetFdSearchIter(SrcpData) != (MengeIterator *)NULL)
      {
         MengeDestroyIterator(SrcpGetFdSearchIter(SrcpData));
      }
      if (SrcpGetClientIter(SrcpData) != (MengeIterator *)NULL)
      {
         MengeDestroyIterator(SrcpGetClientIter(SrcpData));
      }
      if (SrcpGetClientIter(SrcpData) != (MengeIterator *)NULL)
      {
         MengeDestroy(SrcpGetClients(SrcpData));
      }
      free(Data->private);
   }
}

void SrcpSendAnswer(ClientInfo *Data, char *Msg)
{  char ErrorMsg[100];
   struct timeval Now;

   gettimeofday(&Now, NULL);
   sprintf(ErrorMsg, "%lu.%.3lu %s\n", Now.tv_sec, Now.tv_usec / 1000, Msg);
   send(SrcpClientGetClientSock(Data), ErrorMsg, strlen(ErrorMsg), 0);
}
