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
#include <mr_ipc.h>
#include "log.h"

#define SELECT_TIMEOUT 100

static BOOL Loop = TRUE;

LogStruct *LogCreate(void)
{  LogStruct *Data;

   Data = (LogStruct *)malloc(sizeof(LogStruct));
   if (Data != (LogStruct *)NULL)
   {
      LogSetVerbose(Data, FALSE);
      LogSetInterface(Data, (char *)NULL);
      LogSetServerPort(Data, -1);
      LogSetClientSock(Data, -1);
   }
   return(Data);
}

void LogDestroy(LogStruct *Data)
{
   if (LogGetVerbose(Data))
      puts("destroy drehscheibe");
   free(Data);
}

void LogInit(LogStruct *Data, BOOL Verbose, char *Interface, char *Addr,
             int Port)
{
   LogSetVerbose(Data, Verbose);
   LogSetInterface(Data, Interface);
   LogSetAddress(Data, Addr);
   LogSetServerPort(Data, Port);
   LogSetClientSock(Data, -1);
}

static void SigHandler(int sig)
{
   Loop = FALSE;
}

static BOOL Start(LogStruct *Data)
{  struct sigaction SigStruct;

   if (strlen(LogGetInterface(Data)) > 0)
   {
      LogSetClientSock(Data,
                       MrIpcConnectIf(LogGetInterface(Data),
                                      LogGetServerPort(Data)));
   }
   else
   {
      LogSetClientSock(Data,
                       MrIpcConnect(LogGetAddress(Data),
                                    LogGetServerPort(Data)));
   }
   if (LogGetClientSock(Data) >= 0)
   {
      if (LogGetVerbose(Data))
         puts("ready for incoming comands");
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

static void Stop(LogStruct *Data)
{
   if (LogGetVerbose(Data))
      puts("stop network client");
   if (LogGetClientSock(Data) >= 0)
   {
      MrIpcClose(LogGetClientSock(Data));
   }
}

static void ProcessSystemData(LogStruct *Data, MrIpcCmdType *CmdFrame)
{  unsigned int Param;
   unsigned long Adr;
   DirectionType Direction;
   time_t Now;
   int i;

   Now = time(NULL);
   printf("%s> command frame\n", asctime(localtime(&Now)));
   switch (MrIpcGetCommand(CmdFrame))
   {
      case MrIpcCmdNull:
         puts("   cmd null");
         break;
      case MrIpcCmdLocomotiveDirection:
         MrIpcCmdGetLocomotiveDir(CmdFrame, &Adr, &Direction);
         printf("   cmd locomotive addr %ld dir %d\n",
                Adr, Direction);
         break;
      case MrIpcCmdLocomotiveSpeed:
         MrIpcCmdGetLocomotiveSpeed(CmdFrame, &Adr, &Param);
         printf("   cmd locomotive addr %ld speed %d\n",
                Adr, Param);
         break;
      default:
         printf("   command %d, CAN cmd 0x%x, %d Bytes\n",
                MrIpcGetCommand(CmdFrame),
                MrIpcGetCanCommand(CmdFrame),
                MrIpcGetRawDlc(CmdFrame));
         for (i = 0; i < MR_CS2_NUM_CAN_BYTES; i++)
            printf(" %02x", MrIpcGetRawDataI(CmdFrame, i));
         printf("\n");
         break;
   }
}

static void HandleSystemData(LogStruct *Data)
{  MrIpcCmdType CmdFrame;
   int RcvReturnValue;

   if (LogGetVerbose(Data))
      puts("new data available");
   do {
      MrIpcInit(&CmdFrame);
      RcvReturnValue = MrIpcRecv(LogGetClientSock(Data), &CmdFrame);
      if (RcvReturnValue == MR_IPC_RCV_ERROR)
      {
         if (LogGetVerbose(Data))
            puts("Error in recieve from socket!");
      }
      else if (RcvReturnValue == MR_IPC_RCV_CLOSED)
      {
         if (LogGetVerbose(Data))
            puts("client socket was closed\nmaybe server has stoped");
         Loop = FALSE;
      }
      else
      {
         if (LogGetVerbose(Data))
            puts("read new comand frame from socket");
         ProcessSystemData(Data, &CmdFrame);
         MrIpcExit(&CmdFrame);
      }
   } while (RcvReturnValue > 0);
}

void LogRun(LogStruct *Data)
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
         if (LogGetVerbose(Data))
            printf("add client socket %d\n", LogGetClientSock(Data));
         FD_SET(LogGetClientSock(Data), &ReadFds);
         if (LogGetClientSock(Data) > HighFd)
            HighFd = LogGetClientSock(Data);
         SelectTimeout.tv_sec = SELECT_TIMEOUT;
         SelectTimeout.tv_usec = 0;
         if (LogGetVerbose(Data))
            printf("wait for %d fd, max %ld s\n", HighFd, SelectTimeout.tv_sec);
         RetVal = select(HighFd + 1, &ReadFds, NULL, NULL, &SelectTimeout);
         if (LogGetVerbose(Data))
            printf("select liefert %d\n", RetVal);
         if (((RetVal == -1) && (errno == EINTR)) || (RetVal == 0))
         {
            Now = time(NULL);
            if (LogGetVerbose(Data))
               printf("interrupt at %s\n", asctime(localtime(&Now)));
         }
         else if (RetVal < 0)
         {
            if (LogGetVerbose(Data))
               puts("error in main loop");
            Loop = FALSE;
         }
         else
         {
            if (FD_ISSET(LogGetClientSock(Data), &ReadFds))
            {
               HandleSystemData(Data);
            }
         }
      }
      Stop(Data);
   }
}
