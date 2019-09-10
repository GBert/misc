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

   if ((strlen(LogGetInterface(Data)) > 0) &&
       ((strlen(LogGetAddress(Data)) == 0) ||
        (strcmp(LogGetAddress(Data), "0.0.0.0") == 0)))
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
{  unsigned int Param1, Param2;
   unsigned long Adr;
   char Name[32];
   SwitchType Switch;
   DirectionType Direction;
   PositionType Position;
   time_t Now;
   int i;

   Now = time(NULL);
   printf("%s> command frame\n", asctime(localtime(&Now)));
   switch (MrIpcGetCommand(CmdFrame))
   {
      case MrIpcCmdNull:
         printf("   cmd null, CAN cmd 0x%x, %d Bytes\n",
                MrIpcGetCanCommand(CmdFrame),
                MrIpcGetRawDlc(CmdFrame));
         printf("sender %d receiver %d\n",
                MrIpcGetSenderSocket(CmdFrame),
                MrIpcGetReceiverSocket(CmdFrame));
         printf("lp1 %lx ip1 %0x ip2%0x\n",
                MrIpcGetIntLp1(CmdFrame),
                MrIpcGetIntIp1(CmdFrame),
                MrIpcGetIntIp2(CmdFrame));
         for (i = 0; i < MR_CS2_NUM_CAN_BYTES; i++)
            printf(" %02x", MrIpcGetRawDataI(CmdFrame, i));
         printf("\n");
         break;
      case MrIpcCmdRun:
         MrIpcCmdGetRun(CmdFrame, &Switch);
         switch (Switch)
         {
            case Off:
               puts("   cmd switch system to off");
               break;
            case On:
               puts("   cmd switch system to on");
               break;
         }
         break;
      case MrIpcCmdTrackProto:
         MrIpcCmdGetTrackProto(CmdFrame, &Param1);
         printf("   cmd track protocol 0x%x\n", Param1);
         break;
      case MrIpcCmdLocomotiveSpeed:
         MrIpcCmdGetLocomotiveSpeed(CmdFrame, &Adr, &Param1);
         printf("   cmd locomotive addr %ld speed %d\n",
                Adr, Param1);
         break;
      case MrIpcCmdLocomotiveDirection:
         MrIpcCmdGetLocomotiveDir(CmdFrame, &Adr, &Direction);
         printf("   cmd locomotive addr %ld dir %d\n",
                Adr, Direction);
         break;
      case MrIpcCmdLocomotiveFunction:
         /* command to set function of a locomotive */
         MrIpcCmdGetLocomotiveFkt(CmdFrame, &Adr, &Param1, &Switch);
         switch (Switch)
         {
            case Off:
               printf("   cmd locomotive addr %ld function %d to off\n",
                      Adr, Param1);
               break;
            case On:
               printf("   cmd locomotive addr %ld function %d to on\n",
                      Adr, Param1);
               break;
         }
         break;
      case MrIpcCmdAccSwitch:
         MrIpcCmdGetAccPos(CmdFrame, &Adr, &Position, (int *)&Param1);
         switch (Position)
         {
            case Left:
               printf("   cmd accessory addr %ld to left\n",
                      Adr);
               break;
            case Right:
               printf("   cmd accessory addr %ld to right\n",
                      Adr);
               break;
         };
         break;
      case MrIpcCmdRequestMember:
         puts("   cmd request member");
         break;
      case MrIpcCmdMember:
         MrIpcCmdGetMember(CmdFrame, &Adr, &Param1, &Param2);
         printf("   cmd member addr %ld Version 0x%x Type 0x%x\n",
                Adr, Param1, Param2);
         break;
      case MrIpcCmdRequestLocName:
         MrIpcCmdGetReqestLocname(CmdFrame, &Param1, &Param2);
         printf("   cmd request locname startidx %d endidx %d\n",
                Param1, Param2);
         break;
      case MrIpcCmdRequestLocInfo:
         MrIpcCmdGetReqestLocinfo(CmdFrame, Name);
         printf("   cmd request locinfo for %s\n",
                Name);
         break;
      case MrIpcCmdRequestFile:
         MrIpcCmdGetQuery(CmdFrame, Name);
         printf("   cmd request file %s\n",
                Name);
         break;
      case MrIpcCmdCfgHeader:
         MrIpcCmdGetCfgHeader(CmdFrame, &Adr, &Param1);
         printf("   cmd cfg header length %ld CRC 0x%x\n",
                Adr, Param1);
         break;
      case MrIpcCmdCfgZHeader:
         MrIpcCmdGetCfgZHeader(CmdFrame, &Adr, &Param1);
         printf("   cmd cfg header length %ld CRC 0x%x\n",
                Adr, Param1);
         break;
      case MrIpcCmdCfgData:
         puts("   cmd cfg data");
         for (i = 0; i < MR_CS2_NUM_CAN_BYTES; i++)
            printf(" %02x", MrIpcGetRawDataI(CmdFrame, i));
         printf("\n");
         break;
      case MrIpcCmdCanBootldrGeb:
         MrIpcCmdGetCanBootldr(CmdFrame, &Param1, Name);
         printf("   cmd can bootloader gebunden DLC %d",
                Param1);
         for (i = 0; i < MR_CS2_NUM_CAN_BYTES; i++)
            printf(" %02x", MrIpcGetRawDataI(CmdFrame, i));
         printf("\n");
         break;
      case MrIpcCmdStatusRequest:
         MrIpcCmdGetStatusRequest(CmdFrame, &Adr, &Param1);
         printf("   cmd status request addr %ld index%d\n",
                Adr, Param1);
         break;
      case MrIpcCmdStatusSize:
         MrIpcCmdGetStatusPos(CmdFrame, &Adr, &Param1, &Param2);
         printf("   cmd status request addr %ld index %d num packets %d\n",
                Adr, Param1, Param2);
         break;
      case MrIpcCmdStatusData:
         puts("   cmd status data");
         for (i = 0; i < MR_CS2_NUM_CAN_BYTES; i++)
            printf(" %02x", MrIpcGetRawDataI(CmdFrame, i));
         printf("\n");
         break;
      case MrIpcCmdStatusResponse:
         MrIpcCmdGetStatusResponse(CmdFrame, &Adr, &Param1);
         printf("   cmd status response addr %ld index %d\n", Adr, Param1);
         break;
      case MrIpcCmdRequestMesswert:
         MrIpcCmdGetMesswertRequest(CmdFrame, &Adr, &Param1);
         printf("   cmd messwert request addr %ld channel %d\n",
                Adr, Param1);
         break;
      case MrIpcCmdSetConfig:
         MrIpcCmdGetSetConfig(CmdFrame, &Adr, &Param1, &Param2);
         printf("   cmd system config addr %ld channel %d value %d\n",
                Adr, Param1, Param2);
         break;
      case MrIpcCmdConfigResponse:
         MrIpcCmdGetConfigResponse(CmdFrame, &Adr, &Param1, &Param2);
         printf("   cmd system config response addr %ld channel %d response %d\n",
                Adr, Param1, Param2);
         break;
      case MrIpcCmdMesswertResponse:
         MrIpcCmdGetMesswertResponse(CmdFrame, &Adr, &Param1, &Param2);
         printf("   cmd messwert response addr %ld channel %d value %d\n",
                Adr, Param1, Param2);
         break;
      case MrIpcCmdRequestLokListe:
         MrIpcCmdGetReqestLoclist(CmdFrame);
         puts("   cmd request locliste");
         break;
      case MrIpcCmdIntern:
         puts("   cmd internal command");
         switch (MrIpcGetCanCommand(CmdFrame))
         {
            case MrIpcInternalPollMs2:
               puts("   subcmd pol ms2");
               break;
         }
         break;
      default:
         printf("   command %d, CAN cmd 0x%x, %d Bytes\n",
                MrIpcGetCommand(CmdFrame),
                MrIpcGetCanCommand(CmdFrame),
                MrIpcGetRawDlc(CmdFrame));
         printf("sender %d receiver %d\n",
                MrIpcGetSenderSocket(CmdFrame),
                MrIpcGetReceiverSocket(CmdFrame));
         printf("lp1 %lx ip1 %0x ip2 %0x\n",
                MrIpcGetIntLp1(CmdFrame),
                MrIpcGetIntIp1(CmdFrame),
                MrIpcGetIntIp2(CmdFrame));
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
