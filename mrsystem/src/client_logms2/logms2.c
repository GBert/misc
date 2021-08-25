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
#include "logms2.h"

#define SELECT_TIMEOUT 100

static BOOL Loop = TRUE;

Logms2Struct *Logms2Create(void)
{  Logms2Struct *Data;

   Data = (Logms2Struct *)malloc(sizeof(Logms2Struct));
   if (Data != (Logms2Struct *)NULL)
   {
      Logms2SetVerbose(Data, FALSE);
      Logms2SetInterface(Data, (char *)NULL);
      Logms2SetServerPort(Data, -1);
      Logms2SetClientSock(Data, -1);
      Logms2SetCanName(Data, strdup("can0"));
      Logms2SetCanSock(Data, -1);
   }
   return(Data);
}

void Logms2Destroy(Logms2Struct *Data)
{
   if (Logms2GetVerbose(Data))
      puts("destroy mrms2");
   free(Data);
}

void Logms2Init(Logms2Struct *Data, BOOL Verbose, char *Interface, char *Addr,
                int Port, char *CanIf)
{
   Logms2SetVerbose(Data, Verbose);
   Logms2SetInterface(Data, Interface);
   Logms2SetAddress(Data, Addr);
   Logms2SetServerPort(Data, Port);
   Logms2SetClientSock(Data, -1);
   Logms2SetCanSock(Data, -1);
   if (CanIf != (char *)NULL)
   {
      Logms2SetCanName(Data, CanIf);
   }
}

static void SigHandler(int sig)
{
   Loop = FALSE;
}

static BOOL Start(Logms2Struct *Data)
{  struct sigaction SigStruct;

   if ((strlen(Logms2GetInterface(Data)) > 0) &&
       ((strlen(Logms2GetAddress(Data)) == 0) ||
        (strcmp(Logms2GetAddress(Data), "0.0.0.0") == 0)))
   {
      Logms2SetClientSock(Data,
                          MrIpcConnectIf(Logms2GetInterface(Data),
                                         Logms2GetServerPort(Data)));
   }
   else
   {
      Logms2SetClientSock(Data,
                          MrIpcConnect(Logms2GetAddress(Data),
                                       Logms2GetServerPort(Data)));
   }
   Logms2SetCanSock(Data, MrMs2Connect(Logms2GetCanName(Data)));
   if (Logms2GetCanSock(Data) >= 0)
   {
      if (Logms2GetVerbose(Data))
         puts("ready for incoming commands from system");
      SigStruct.sa_handler = SigHandler;
      sigemptyset(&SigStruct.sa_mask);
      SigStruct.sa_flags = 0;
      sigaction(SIGINT, &SigStruct, NULL);
      sigaction(SIGQUIT, &SigStruct, NULL);
      sigaction(SIGTERM, &SigStruct, NULL);
      SendMagicStart60113Frame(Logms2GetCanSock(Data), Logms2GetVerbose(Data));
      return(TRUE);
   }
   else
   {
      if (Logms2GetClientSock(Data) != -1)
         MrIpcClose(Logms2GetClientSock(Data));
      return(FALSE);
   }
}

static void Stop(Logms2Struct *Data)
{
   if (Logms2GetVerbose(Data))
      puts("stop mrms2");
   if (Logms2GetClientSock(Data) != -1)
      MrIpcClose(Logms2GetClientSock(Data));
   MrMs2Close(Logms2GetCanSock(Data));
}

static void ProcessSystemData(Logms2Struct *Data, MrIpcCmdType *CmdFrame)
{
}

static void HandleSystemData(Logms2Struct *Data)
{  MrIpcCmdType CmdFrame;
   int RcvReturnValue;

   if (Logms2GetVerbose(Data))
      puts("new data available");
   MrIpcInit(&CmdFrame);
   RcvReturnValue = MrIpcRecv(Logms2GetClientSock(Data), &CmdFrame);
   if (RcvReturnValue == MR_IPC_RCV_ERROR)
   {
      if (Logms2GetVerbose(Data))
         puts("Error in recieve from socket!");
   }
   else if (RcvReturnValue == MR_IPC_RCV_CLOSED)
   {
      if (Logms2GetVerbose(Data))
         puts("client socket was closed\nmaybe server has stoped");
      Loop = FALSE;
   }
   else
   {
      if (Logms2GetVerbose(Data))
         printf("read new comand frame from socket %d\n",
                MrIpcGetCommand(&CmdFrame));
      ProcessSystemData(Data, &CmdFrame);
   }
}

static void ProcessCanData(Logms2Struct *Data, struct can_frame *CanFrame)
{  MrMs2CanDataType CanMsg;

   if (Logms2GetVerbose(Data))
   {
      MrMs2Decode(&CanMsg, CanFrame);
      if (MrMs2IsMs2(&CanMsg))
      {
         MrMs2Trace(&CanMsg);
      }
   }
}

static void HandleCanData(Logms2Struct *Data)
{  struct can_frame frame;

   if (read(Logms2GetCanSock(Data), &frame, sizeof(struct can_frame)) ==
       sizeof(struct can_frame))
   {
      ProcessCanData(Data, &frame);
   }
}

void Logms2Run(Logms2Struct *Data)
{  fd_set ReadFds;
   int RetVal, HighFd;
   struct timeval SelectTimeout;
   time_t Now;

   if (Start(Data))
   {
      while (Loop)
      {
         FD_ZERO(&ReadFds);
         HighFd = 0;
         if (Logms2GetClientSock(Data) != -1)
         {
            if (Logms2GetVerbose(Data))
               printf("add client socket %d\n", Logms2GetClientSock(Data));
            FD_SET(Logms2GetClientSock(Data), &ReadFds);
            if (Logms2GetClientSock(Data) > HighFd)
               HighFd = Logms2GetClientSock(Data);
         }
         if (Logms2GetVerbose(Data))
            printf("add can socket %d\n", Logms2GetCanSock(Data));
         FD_SET(Logms2GetCanSock(Data), &ReadFds);
         if (Logms2GetCanSock(Data) > HighFd)
            HighFd = Logms2GetCanSock(Data);
         SelectTimeout.tv_sec = SELECT_TIMEOUT;
         SelectTimeout.tv_usec = 0;
         if (Logms2GetVerbose(Data))
            printf("wait for %d fd, max %ld s\n", HighFd, SelectTimeout.tv_sec);
         RetVal = select(HighFd + 1, &ReadFds, NULL, NULL, &SelectTimeout);
         if (Logms2GetVerbose(Data))
            printf("select liefert %d\n", RetVal);
         if (((RetVal == -1) && (errno == EINTR)) || (RetVal == 0))
         {
            Now = time(NULL);
            if (Logms2GetVerbose(Data))
               printf("interrupt at %s\n", asctime(localtime(&Now)));
         }
         else if (RetVal < 0)
         {
            if (Logms2GetVerbose(Data))
               puts("error in main loop");
            Loop = FALSE;
         }
         else
         {
            if ((Logms2GetClientSock(Data) != -1) &&
                (FD_ISSET(Logms2GetClientSock(Data), &ReadFds)))
            {
               HandleSystemData(Data);
            }
            if (FD_ISSET(Logms2GetCanSock(Data), &ReadFds))
            {
               HandleCanData(Data);
            }
         }
      }
      Stop(Data);
   }
}
