#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <boolean.h>
#include <mr_ipc.h>
#include <cs2.h>
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

static void SendCmd(int socket, unsigned long Uid, unsigned Response,
                    unsigned Command, unsigned Prio, int DLC, int *CanBytes)
{  MrCs2CanDataType CanMsg;
   struct can_frame CanFrame;
   MrIpcCmdType Cmd;
   unsigned int i;

   CanFrame.can_id = Cs2EncodeId(Cs2CalcHash(Uid), Response, Command, Prio);
   CanFrame.can_dlc = DLC;
   for (i = 0; i < MR_CS2_NUM_CAN_BYTES; i++)
   {
      CanFrame.data[i] = CanBytes[i];
   }
   MrCs2Decode(&CanMsg, &CanFrame);
   MrIpcInit(&Cmd);
   MrIpcEncodeFromCan(&Cmd, &CanMsg);
   MrIpcSetSenderSocket(&Cmd, socket);
   MrIpcSend(socket, &Cmd);
}

void InjectRunCmd(InjectStruct *Data, unsigned long Uid, unsigned Response,
                    unsigned Command, unsigned Prio, int DLC, int *CanBytes)
{
   OpenSocket(Data);
   if (InjectGetClientSock(Data) >= 0)
   {
      SendCmd(InjectGetClientSock(Data), Uid, Response, Command, Prio, DLC, CanBytes);
      MrIpcClose(InjectGetClientSock(Data));
   }
}

void InjectRunFile(InjectStruct *Data, char *Filename)
{  FILE *CmdStream;
   char Line[80], *Param;
   int Response, Command, Prio, DLC, CanBytes[8], i;
   unsigned long Uid;

   CmdStream = fopen(Filename, "r");
   if (CmdStream != (FILE *)NULL)
   {
      OpenSocket(Data);
      if (InjectGetClientSock(Data) >= 0)
      {
         while (fgets(Line, sizeof(Line), CmdStream) != (char *)NULL)
         {
            Param = strtok(Line, " ");
            Uid = strtol(Param, (char **)NULL, 0);
            Param = strtok((char *)NULL, " ");
            Response = strtol(Param, (char **)NULL, 0);
            Param = strtok((char *)NULL, " ");
            Command = strtol(Param, (char **)NULL, 0);
            Param = strtok((char *)NULL, " ");
            Prio = strtol(Param, (char **)NULL, 0);
            Param = strtok((char *)NULL, " ");
            DLC = strtol(Param, (char **)NULL, 0);
            for (i = 0; i < 8; i++)
            {
               Param = strtok((char *)NULL, " ");
               CanBytes[i] = strtol(Param, (char **)NULL, 0);
            }
            SendCmd(InjectGetClientSock(Data), Uid, Response, Command, Prio, DLC, CanBytes);
         }
         MrIpcClose(InjectGetClientSock(Data));
      }
      fclose(CmdStream);
   }
}
