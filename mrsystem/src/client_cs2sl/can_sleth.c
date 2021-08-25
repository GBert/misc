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
#include "can_sleth.h"

#define MAX_DATAGRAM_SIZE 4096

static int SlethClientGetFd(void *private)
{  SlethClientStruct *Data;
   int ReturnFd;

   Data = (SlethClientStruct *)private;
   switch (SlethClientGetFdPos(Data))
   {
      case FD_POS_UDP:
         if (SlethClientGetOutsideUdpSock(Data) != IOFKT_INVALID_FD)
         {
            ReturnFd = SlethClientGetOutsideUdpSock(Data);
            SlethClientSetFdPos(Data, FD_POS_TCP);
         }
         else
         {
            if (SlethClientGetOutsideTcpSock(Data) != IOFKT_INVALID_FD)
            {
               ReturnFd = SlethClientGetOutsideTcpSock(Data);
               SlethClientSetFdPos(Data, FD_POS_ENDE);
            }
            else
            {
               ReturnFd = IOFKT_INVALID_FD;
               SlethClientSetFdPos(Data, FD_POS_UDP);
            }
         }
         break;
      case FD_POS_TCP:
         if (SlethClientGetOutsideTcpSock(Data) != IOFKT_INVALID_FD)
         {
            ReturnFd = SlethClientGetOutsideTcpSock(Data);
            SlethClientSetFdPos(Data, FD_POS_ENDE);
         }
         else
         {
            ReturnFd = IOFKT_INVALID_FD;
            SlethClientSetFdPos(Data, FD_POS_UDP);
          }
         break;
      case FD_POS_ENDE:
         ReturnFd = IOFKT_INVALID_FD;
         SlethClientSetFdPos(Data, FD_POS_UDP);
         break;
   }
   return(ReturnFd);
}

static BOOL SlethClientOpen(void *private)
{  SlethClientStruct *Data;
   int Socket;

   Data = (SlethClientStruct *)private;
   if (SlethClientGetVerbose(Data))
      puts("start udp client");
   Socket = MrEthCs2ConnectClient();
   if (Socket >= 0)
   {
      SlethClientSetOutsideUdpSock(Data, Socket);
      SlethClientSetFdPos(Data, FD_POS_UDP);
   }
   return(SlethClientGetOutsideUdpSock(Data) >= 0);
}

static void SlethClientClose(void *private)
{  SlethClientStruct *Data;

   Data = (SlethClientStruct *)private;
   if (SlethClientGetOutsideUdpSock(Data) != IOFKT_INVALID_FD)
      MrEthCs2Close(SlethClientGetOutsideUdpSock(Data));
   if (SlethClientGetOutsideTcpSock(Data) !=IOFKT_INVALID_FD)
      MrEthCs2Close(SlethClientGetOutsideTcpSock(Data));
   SlethClientSetOutsideUdpSock(Data, IOFKT_INVALID_FD);
   SlethClientSetOutsideTcpSock(Data, IOFKT_INVALID_FD);
   SlethClientSetCs2Addr(Data, FALSE);
}

static BOOL SlethClientRead(void *private, int fd, BOOL PendingData,
                            MrCs2CanDataType *CanMsg)
{  SlethClientStruct *Data;
   char UdpFrame[MAX_DATAGRAM_SIZE];
   int Ret;

   Data = (SlethClientStruct *)private;
   if (PendingData)
   {
      Ret = FALSE;
   }
   else
   {
      if (fd == SlethClientGetOutsideUdpSock(Data))
      {
         Ret = MrEthCs2Recv(SlethClientGetOutsideUdpSock(Data),
                            &SlethClientGetClientAddr(Data), UdpFrame);
         if (!SlethClientGetCs2Addr(Data))
         {
            if (SlethClientGetVerbose(Data))
               puts("found new cs2");
            SlethClientSetCs2Addr(Data, TRUE);
            if (SlethClientGetDoTcp(Data))
            {
               SlethClientSetOutsideTcpSock(Data,
                                            MrEthCs2ConnectTcpClient(&SlethClientGetClientAddr(Data)));
               if (SlethClientGetOutsideTcpSock(Data) != IOFKT_INVALID_FD)
               {
                  MrEthCs2Close(SlethClientGetOutsideUdpSock(Data));
                  SlethClientSetOutsideUdpSock(Data, IOFKT_INVALID_FD);
                  if (SlethClientGetVerbose(Data))
                     printf("tcp connection socket %d\n",
                            SlethClientGetOutsideTcpSock(Data));
               }
            }
         }
         if (Ret > 0)
         {
            if (SlethClientGetVerbose(Data))
               printf("read new udp frame from %s\n",
                      inet_ntoa(SlethClientGetClientAddr(Data).sin_addr));
            MrEthCs2Decode(CanMsg, UdpFrame);
         }
      }
      else if (fd == SlethClientGetOutsideTcpSock(Data))
      {
         Ret = MrEthCs2Recv(SlethClientGetOutsideTcpSock(Data),
                            &SlethClientGetClientAddr(Data), UdpFrame);
         if (Ret)
         {
            if (SlethClientGetVerbose(Data))
               puts("read new udp frame");
            MrEthCs2Decode(CanMsg, UdpFrame);
         }
      }
      else
      {
         Ret = FALSE;
      }
   }
   return(Ret);
}

static BOOL SlethClientWrite(void *private, int ReceiverSocket,
                             MrCs2CanDataType *CanMsg)
{  SlethClientStruct *Data;
   char UdpFrame[MR_CS2_UDP_LENGTH];
   BOOL Ret;

   Data = (SlethClientStruct *)private;
   MrEthCs2Encode(UdpFrame, CanMsg);
   if (((ReceiverSocket == MR_IPC_SOCKET_ALL) ||
        (ReceiverSocket == SlethClientGetOutsideTcpSock(Data))) &&
       (SlethClientGetOutsideTcpSock(Data) != IOFKT_INVALID_FD))
   {
      MrEthCs2Send(SlethClientGetOutsideTcpSock(Data), UdpFrame);
      Ret = TRUE;
   }
   else if (((ReceiverSocket == MR_IPC_SOCKET_ALL) ||
             (ReceiverSocket == SlethClientGetOutsideUdpSock(Data))) &&
            (SlethClientGetCs2Addr(Data)))
   {
      MrEthCs2SendTo(SlethClientGetOutsideUdpSock(Data),
                     (struct sockaddr_in *)&(SlethClientGetClientAddr(Data)),
                     UdpFrame);
      Ret = TRUE;
   }
   else
   {
      Ret = FALSE;
   }
   return(Ret);
}

static IoFktStruct CanClientIoFunctions =
{
   NULL,
   SlethClientGetFd,
   SlethClientOpen,
   SlethClientClose,
   SlethClientRead,
   SlethClientWrite
};

IoFktStruct *SlethClientInit(BOOL Verbose, BOOL DoTcp)
{  SlethClientStruct *Data;

   Data = (SlethClientStruct *)malloc(sizeof(SlethClientStruct));
   if (Data != (SlethClientStruct *)NULL)
   {
      SlethClientSetVerbose(Data, Verbose);
      SlethClientSetDoTcp(Data, DoTcp);
      SlethClientSetOutsideUdpSock(Data, IOFKT_INVALID_FD);
      SlethClientSetOutsideTcpSock(Data, IOFKT_INVALID_FD);
      SlethClientSetCs2Addr(Data, FALSE);
      memset((void *)(&(SlethClientGetClientAddr(Data))), 0,
             sizeof(struct sockaddr_in));
      SlethClientSetFdPos(Data, FD_POS_UDP);
      CanClientIoFunctions.private = (void *)Data;
      return(&CanClientIoFunctions);
   }
   else
      return(NULL);
}

void SlethClientExit(IoFktStruct *Data)
{
   if (Data->private != (void *)NULL)
      free(Data->private);
}
