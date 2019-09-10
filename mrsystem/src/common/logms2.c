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
#include "can_io.h"
#include "logms2.h"

#define SELECT_TIMEOUT 100

static BOOL Loop = TRUE;

Logms2Struct *Logms2Create(void)
{  Logms2Struct *Data;

   Data = (Logms2Struct *)malloc(sizeof(Logms2Struct));
   if (Data != (Logms2Struct *)NULL)
   {
      Logms2SetVerbose(Data, FALSE);
      Logms2SetIoFunctions(Data, (IoFktStruct *)NULL);
   }
   return(Data);
}

void Logms2Destroy(Logms2Struct *Data)
{
   if (Logms2GetVerbose(Data))
      puts("destroy mrms2");
   free(Data);
}

void Logms2Init(Logms2Struct *Data, BOOL Verbose, IoFktStruct *IoFunctions)
{
   Logms2SetVerbose(Data, Verbose);
   Logms2SetIoFunctions(Data, IoFunctions);
}

static void SigHandler(int sig)
{
   Loop = FALSE;
}

static BOOL Start(Logms2Struct *Data)
{  struct sigaction SigStruct;

   if (Logms2GetIoFunctions(Data)->Open(Logms2GetIoFunctions(Data)->private))
   {
      if (Logms2GetVerbose(Data))
         puts("ready for incoming commands from system");
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

static void Stop(Logms2Struct *Data)
{
   if (Logms2GetVerbose(Data))
      puts("stop mrms2");
   Logms2GetIoFunctions(Data)->Close(Logms2GetIoFunctions(Data)->private);
}

static void ProcessCanData(Logms2Struct *Data, MrCs2CanDataType *CanMsg)
{
   if (Logms2GetVerbose(Data))
   {
      if (MrCs2GetIsCs2(CanMsg))
      {
         MrCs2Trace(CanMsg);
      }
   }
}

static void HandleCanData(Logms2Struct *Data, int Fd)
{  MrCs2CanDataType CanMsg;
   BOOL Ret;

   Ret = Logms2GetIoFunctions(Data)->Read(Logms2GetIoFunctions(Data)->private,
                                          Fd, FALSE, &CanMsg);
   while (Ret)
   {
      ProcessCanData(Data, &CanMsg);
      Ret = Logms2GetIoFunctions(Data)->Read(Logms2GetIoFunctions(Data)->private,
                                             Fd, TRUE, &CanMsg);
   }
}

void Logms2Run(Logms2Struct *Data)
{  fd_set ReadFds;
   int RetVal, HighFd, IoFd;
   struct timeval SelectTimeout;
   time_t Now;

   if (Start(Data))
   {
      while (Loop)
      {
         FD_ZERO(&ReadFds);
         HighFd = 0;
         while ((IoFd = Logms2GetIoFunctions(Data)->GetFd(Logms2GetIoFunctions(Data)->private)) != IOFKT_INVALID_FD)
         {
            if (Logms2GetVerbose(Data))
               printf("add can socket %d\n", IoFd);
            FD_SET(IoFd, &ReadFds);
            if (IoFd > HighFd)
               HighFd = IoFd;
         }
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
            while ((IoFd = Logms2GetIoFunctions(Data)->GetFd(Logms2GetIoFunctions(Data)->private)) != IOFKT_INVALID_FD)
            {
               if (FD_ISSET(IoFd, &ReadFds))
               {
                  if (Logms2GetVerbose(Data))
                     puts("new data on can socket");
                  HandleCanData(Data, IoFd);
               }
            }
         }
      }
      Stop(Data);
   }
}
