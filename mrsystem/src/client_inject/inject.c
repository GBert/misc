#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <boolean.h>
#include <mr_ipc.h>
#include "inject.h"

InjectStruct *InjectCreate(void)
{  InjectStruct *Data;

   Data = (InjectStruct *)malloc(sizeof(InjectStruct));
   if (Data != (InjectStruct *)NULL)
   {
      InjectSetVerbose(Data, FALSE);
      InjectSetInterface(Data, (char *)NULL);
      InjectSetServerPort(Data, -1);
      InjectSetClientSock(Data, -1);
   }
   return(Data);
}

void InjectDestroy(InjectStruct *Data)
{
   if (InjectGetVerbose(Data))
      puts("destroy mrinject");
   free(Data);
}

void InjectInit(InjectStruct *Data, BOOL Verbose, char *Iface, char *Addr,
                int Port)
{
   InjectSetVerbose(Data, Verbose);
   InjectSetInterface(Data, Iface);
   InjectSetAddress(Data, Addr);
   InjectSetServerPort(Data, Port);
   InjectSetClientSock(Data, -1);
}

static void OpenSocket(InjectStruct *Data)
{
   if ((strlen(InjectGetInterface(Data)) > 0) &&
       ((strlen(InjectGetAddress(Data)) == 0) ||
        (strcmp(InjectGetAddress(Data), "0.0.0.0") == 0)))
   {
      InjectSetClientSock(Data,
                          MrIpcConnectIf(InjectGetInterface(Data),
                                         InjectGetServerPort(Data)));
   }
   else
   {
      InjectSetClientSock(Data,
                          MrIpcConnect(InjectGetAddress(Data),
                                       InjectGetServerPort(Data)));
   }
}

static void SendCmd(int socket, unsigned long CanId, int DLC, int *CanBytes)
{  MrCs2CanDataType CanMsg;
   struct can_frame CanFrame;
   MrIpcCmdType Cmd;

   CanFrame.can_id = CanId;
   CanFrame.can_dlc = DLC;
   memcpy(&(CanFrame.data), CanBytes, MR_CS2_NUM_CAN_BYTES);
   MrCs2Decode(&CanMsg, &CanFrame);
   MrIpcInit(&Cmd);
   MrIpcEncodeFromCan(&Cmd, &CanMsg);
   MrIpcSetSenderSocket(&Cmd, socket);
   MrIpcSend(socket, &Cmd);
}

void InjectRunCmd(InjectStruct *Data, unsigned long CanId, int DLC,
                  int *CanBytes)
{
   OpenSocket(Data);
   if (InjectGetClientSock(Data) >= 0)
   {
      SendCmd(InjectGetClientSock(Data), CanId, DLC, CanBytes);
      MrIpcClose(InjectGetClientSock(Data));
   }
}

void InjectRunFile(InjectStruct *Data, char *Filename)
{  FILE *CmdStream;
   char Line[80], *Param;
   int DLC, CanBytes[8], i;
   unsigned long CanId;

   CmdStream = fopen(Filename, "r");
   if (CmdStream != (FILE *)NULL)
   {
      OpenSocket(Data);
      if (InjectGetClientSock(Data) >= 0)
      {
         while (fgets(Line, sizeof(Line), CmdStream) != (char *)NULL)
         {
            Param = strtok(Line, " ");
            CanId = strtol(Param, (char **)NULL, 0);
            Param = strtok((char *)NULL, " ");
            DLC = strtol(Param, (char **)NULL, 0);
            for (i = 0; i < 8; i++)
            {
               Param = strtok((char *)NULL, " ");
               CanBytes[i] = strtol(Param, (char **)NULL, 0);
            }
            SendCmd(InjectGetClientSock(Data), CanId, DLC, CanBytes);
         }
         MrIpcClose(InjectGetClientSock(Data));
      }
      fclose(CmdStream);
   }
}

void InjectRun(InjectStruct *Data)
{  MrIpcCmdType CmdFrame;

   OpenSocket(Data);
   if (InjectGetClientSock(Data) >= 0)
   {
      if (InjectGetVerbose(Data))
         puts("ready to send commands");
      MrIpcInit(&CmdFrame);
      MrIpcCalcHash(&CmdFrame, 0l);
      MrIpcCmdSetLocomotiveDir(&CmdFrame, 1, Forward);
      MrIpcSend(InjectGetClientSock(Data), &CmdFrame);
      MrIpcExit(&CmdFrame);
      MrIpcInit(&CmdFrame);
      MrIpcCalcHash(&CmdFrame, 0l);
      MrIpcCmdSetLocomotiveSpeed(&CmdFrame, 1, 25);
      MrIpcSend(InjectGetClientSock(Data), &CmdFrame);
      MrIpcExit(&CmdFrame);
      if (InjectGetVerbose(Data))
         puts("stop network client");
      MrIpcClose(InjectGetClientSock(Data));
   }
}
