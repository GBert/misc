#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <boolean.h>
#include <menge.h>
#include <stack.h>
#include <mr_ipc.h>
#include "drehscheibe.h"

#define SELECT_TIMEOUT 100

static BOOL Loop = TRUE;

DrehscheibeStruct *DrehscheibeCreate(void)
{  DrehscheibeStruct *Data;

   Data = (DrehscheibeStruct *)malloc(sizeof(DrehscheibeStruct));
   if (Data != (DrehscheibeStruct *)NULL)
   {
      DrehscheibeSetClient(Data, MengeCreate());
      if (DrehscheibeGetClient(Data) != (Menge *)NULL)
      {
         DrehscheibeSetRemove(Data, StackCreate());
         if (DrehscheibeGetRemove(Data) != (Stack *)NULL)
         {
            DrehscheibeSetVerbose(Data, FALSE);
            DrehscheibeSetInterface(Data, (char *)NULL);
            DrehscheibeSetServerPort(Data, -1);
            DrehscheibeSetServerSock(Data, -1);
         }
         else
         {
            MengeDestroy(DrehscheibeGetClient(Data));
            free(Data);
            Data = (DrehscheibeStruct *)NULL;
         }
      }
      else
      {
         free(Data);
         Data = (DrehscheibeStruct *)NULL;
      }
   }
   return(Data);
}

void DrehscheibeDestroy(DrehscheibeStruct *Data)
{
   if (DrehscheibeGetVerbose(Data))
      puts("destroy drehscheibe");
   StackDestroy(DrehscheibeGetRemove(Data));
   MengeDestroy(DrehscheibeGetClient(Data));
   free(Data);
}

static int SocketClientsCompare(void *d1, void *d2)
{
   return((long)d1 - (long)d2);
}

static void StackNullDel(StackDataType Data)
{
}

void DrehscheibeInit(DrehscheibeStruct *Data, BOOL Verbose, char *Interface,
                     char *Addr, int Port)
{
   DrehscheibeSetVerbose(Data, Verbose);
   DrehscheibeSetInterface(Data, Interface);
   DrehscheibeSetAddress(Data, Addr);
   DrehscheibeSetServerPort(Data, Port);
   DrehscheibeSetServerSock(Data, -1);
   MengeInit(DrehscheibeGetClient(Data), SocketClientsCompare, free);
   StackInit(DrehscheibeGetRemove(Data), StackNullDel);
}

static void SigHandler(int sig)
{
   Loop = FALSE;
}

static BOOL Start(DrehscheibeStruct *Data)
{  struct sigaction SigStruct;

   if ((strlen(DrehscheibeGetInterface(Data)) > 0) &&
       ((strlen(DrehscheibeGetAddress(Data)) == 0) ||
        (strcmp(DrehscheibeGetAddress(Data), "0.0.0.0") == 0)))
   {
      DrehscheibeSetServerSock(Data,
                               MrIpcStartServerIf(DrehscheibeGetInterface(Data),
                                                  DrehscheibeGetServerPort(Data)));
   }
   else
   {
      DrehscheibeSetServerSock(Data,
                               MrIpcStartServer(DrehscheibeGetAddress(Data),
                                                DrehscheibeGetServerPort(Data)));
   }
   if (DrehscheibeGetServerSock(Data) >= 0)
   {
      if (DrehscheibeGetVerbose(Data))
         puts("ready for incoming connections");
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
      return(FALSE);
   }
}

static void Stop(DrehscheibeStruct *Data)
{
   if (DrehscheibeGetVerbose(Data))
      puts("stop network server");
   if (DrehscheibeGetServerSock(Data) >= 0)
   {
      MrIpcClose(DrehscheibeGetServerSock(Data));
   }
}

static void AddNewClient(DrehscheibeStruct *Data)
{  DrehscheibeClientStruct *SocketEntry;
   int NewClientSock;

   if (DrehscheibeGetVerbose(Data))
      puts("new incoming connection");
   NewClientSock = MrIpcAccept(DrehscheibeGetServerSock(Data));
   SocketEntry = (DrehscheibeClientStruct *)malloc(sizeof(DrehscheibeClientStruct));
   if (SocketEntry != (DrehscheibeClientStruct *)NULL)
   {
      /* we got one, accept */
      DrehclientSetSock(SocketEntry, NewClientSock);
      if (DrehscheibeGetVerbose(Data))
         printf("accept new connection %d\n", DrehclientGetSock(SocketEntry));
      MengeAdd(DrehscheibeGetClient(Data), (MengeDataType)SocketEntry);
   }
   else
   {
      /* can not create client struct, reject */
      if (DrehscheibeGetVerbose(Data))
         puts("reject new connection");
      MrIpcClose(NewClientSock);
   }
}

static void ProcessSystemData(DrehscheibeStruct *Data, MrIpcCmdType *CmdFrame,
                              DrehscheibeClientStruct *ClientEntry)
{  MengeIterator ClientIter;
   DrehscheibeClientStruct *RcvClientEntry;

   MengeInitIterator(&ClientIter, DrehscheibeGetClient(Data));
   RcvClientEntry = (DrehscheibeClientStruct *)MengeFirst(&ClientIter);
   while (RcvClientEntry != (DrehscheibeClientStruct *)NULL)
   {
      if (DrehclientGetSock(ClientEntry) != DrehclientGetSock(RcvClientEntry))
      {
         /* send only to other clients */
         if (DrehscheibeGetVerbose(Data))
            printf("send to client with socket %d\n",
                   DrehclientGetSock(RcvClientEntry));
         MrIpcSend(DrehclientGetSock(RcvClientEntry), CmdFrame);
      }
      RcvClientEntry = (DrehscheibeClientStruct *)MengeNext(&ClientIter);
   }
}

static BOOL HandleSystemData(DrehscheibeStruct *Data,
                             DrehscheibeClientStruct *ClientEntry)
{  MrIpcCmdType CmdFrame;
   int RcvReturnValue;
   BOOL Ret;

   if (DrehscheibeGetVerbose(Data))
      printf("data on client socket %d available\n",
             DrehclientGetSock(ClientEntry));
   MrIpcInit(&CmdFrame);
   RcvReturnValue = MrIpcRecv(DrehclientGetSock(ClientEntry), &CmdFrame);
   if (RcvReturnValue == MR_IPC_RCV_ERROR)
   {
      if (DrehscheibeGetVerbose(Data))
         puts("Error in recieve from socket!");
      Ret = FALSE;
   }
   else if (RcvReturnValue == MR_IPC_RCV_CLOSED)
   {
      if (DrehscheibeGetVerbose(Data))
         puts("client socket was closed");
      MrIpcClose(DrehclientGetSock(ClientEntry));
      Ret = TRUE;
   }
   else
   {
      if (DrehscheibeGetVerbose(Data))
         printf("read new comand frame from socket %d, cmd %02x\n",
                MrIpcGetCommand(&CmdFrame), CmdFrame.CanCommand);
      ProcessSystemData(Data, &CmdFrame, ClientEntry);
      Ret = FALSE;
   }
   return(Ret);
}

void DrehscheibeRun(DrehscheibeStruct *Data)
{  fd_set ReadFds;
   int RetVal, HighFd;
   struct timeval SelectTimeout;
   DrehscheibeClientStruct *ClientEntry;
   MengeIterator ClientIter;
   time_t Now;

   if (Start(Data))
   {
      while (Loop)
      {
         /* Main loop for receive and send data */

         /* fill fd set for call to select */
         FD_ZERO(&ReadFds);
         HighFd = 0;
         /* 1. server socket */
         if (DrehscheibeGetVerbose(Data))
            printf("add server socket %d\n", DrehscheibeGetServerSock(Data));
         FD_SET(DrehscheibeGetServerSock(Data), &ReadFds);
         if (DrehscheibeGetServerSock(Data) > HighFd)
            HighFd = DrehscheibeGetServerSock(Data);
         /* 2. client sockets */
         MengeInitIterator(&ClientIter, DrehscheibeGetClient(Data));
         ClientEntry = (DrehscheibeClientStruct *)MengeFirst(&ClientIter);
         while (ClientEntry != (DrehscheibeClientStruct *)NULL)
         {
            if (DrehscheibeGetVerbose(Data))
               printf("add client socket %d\n", DrehclientGetSock(ClientEntry));
            FD_SET(DrehclientGetSock(ClientEntry), &ReadFds);
            if (DrehclientGetSock(ClientEntry) > HighFd)
                  HighFd = DrehclientGetSock(ClientEntry);
            ClientEntry = (DrehscheibeClientStruct *)MengeNext(&ClientIter);
         }
         /* 3. Timeout */
         SelectTimeout.tv_sec = SELECT_TIMEOUT;
         SelectTimeout.tv_usec = 0;
         if (DrehscheibeGetVerbose(Data))
            printf("wait for %d fd, max %ld s\n", HighFd, SelectTimeout.tv_sec);

         /* wait for data */
         RetVal = select(HighFd + 1, &ReadFds, NULL, NULL, &SelectTimeout);
         if (DrehscheibeGetVerbose(Data))
            printf("select liefert %d\n", RetVal);

         /* check, who has data */
         if (((RetVal == -1) && (errno == EINTR)) || (RetVal == 0))
         {
            /* timeout, time for periodic tasks */
            Now = time(NULL);
            if (DrehscheibeGetVerbose(Data))
               printf("interrupt at %s\n", asctime(localtime(&Now)));
         }
         else if (RetVal < 0)
         {
            /* error, check if some clients disconnect */
            if (DrehscheibeGetVerbose(Data))
               puts("error in main loop");
            /* 1) check clients for closed socket */
            MengeInitIterator(&ClientIter, DrehscheibeGetClient(Data));
            ClientEntry = (DrehscheibeClientStruct *)MengeFirst(&ClientIter);
            while (ClientEntry != (DrehscheibeClientStruct *)NULL)
            {
               /* check for client socket for data */
               if (HandleSystemData(Data, ClientEntry))
               {
                  StackPush(DrehscheibeGetRemove(Data),
                            (MengeDataType)ClientEntry);
               }
               ClientEntry = (DrehscheibeClientStruct *)MengeNext(&ClientIter);
            }
            /* remove clients with closed socket */
            while (!StackIsEmpty(DrehscheibeGetRemove(Data)))
            {
               ClientEntry = (DrehscheibeClientStruct *)StackPop(DrehscheibeGetRemove(Data));
               MengeRemove(DrehscheibeGetClient(Data),
                           (MengeDataType)ClientEntry);
            }
         }
         else
         {
            /* data, check at which fd */
            /* 1) check server socket */
            if (FD_ISSET(DrehscheibeGetServerSock(Data), &ReadFds))
            {
               /* new connection requested */
               AddNewClient(Data);
            }
            /* 2) check clients for data */
            MengeInitIterator(&ClientIter, DrehscheibeGetClient(Data));
            ClientEntry = (DrehscheibeClientStruct *)MengeFirst(&ClientIter);
            while (ClientEntry != (DrehscheibeClientStruct *)NULL)
            {
               /* check for client socket for data */
               if (FD_ISSET(DrehclientGetSock(ClientEntry), &ReadFds))
               {
                  if (HandleSystemData(Data, ClientEntry))
                  {
                     StackPush(DrehscheibeGetRemove(Data),
                               (MengeDataType)ClientEntry);
                  }
               }
               ClientEntry = (DrehscheibeClientStruct *)MengeNext(&ClientIter);
            }
            /* remove closed clients */
            while (!StackIsEmpty(DrehscheibeGetRemove(Data)))
            {
               ClientEntry = (DrehscheibeClientStruct *)StackPop(DrehscheibeGetRemove(Data));
               MengeRemove(DrehscheibeGetClient(Data),
                           (MengeDataType)ClientEntry);
            }
         }
      }

      /* cleanup */
      if (DrehscheibeGetVerbose(Data))
         puts("close all clients and stop system");
      MengeInitIterator(&ClientIter, DrehscheibeGetClient(Data));
      ClientEntry = (DrehscheibeClientStruct *)MengeFirst(&ClientIter);
      while (ClientEntry != (DrehscheibeClientStruct *)NULL)
      {
         MrIpcClose(DrehclientGetSock(ClientEntry));
         DrehclientSetSock(ClientEntry, -1);
         ClientEntry = (DrehscheibeClientStruct *)MengeNext(&ClientIter);
      }
      Stop(Data);
   }
}
