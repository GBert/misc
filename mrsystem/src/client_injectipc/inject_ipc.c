#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <boolean.h>
#include <mr_ipc.h>
#include <cs2.h>
#include "inject_ipc.h"

InjectIpcStruct *InjectIpcCreate(void)
{  InjectIpcStruct *Data;

   Data = (InjectIpcStruct *)malloc(sizeof(InjectIpcStruct));
   if (Data != (InjectIpcStruct *)NULL)
   {
      InjectIpcSetVerbose(Data, FALSE);
      InjectIpcSetInterface(Data, (char *)NULL);
      InjectIpcSetServerPort(Data, -1);
      InjectIpcSetClientSock(Data, -1);
   }
   return(Data);
}

void InjectIpcDestroy(InjectIpcStruct *Data)
{
   if (InjectIpcGetVerbose(Data))
      puts("destroy mrinject");
   free(Data);
}

void InjectIpcInit(InjectIpcStruct *Data, BOOL Verbose, char *Iface,
                   char *Addr, int Port)
{
   InjectIpcSetVerbose(Data, Verbose);
   InjectIpcSetInterface(Data, Iface);
   InjectIpcSetAddress(Data, Addr);
   InjectIpcSetServerPort(Data, Port);
   InjectIpcSetClientSock(Data, -1);
}

static void OpenSocket(InjectIpcStruct *Data)
{
   if ((strlen(InjectIpcGetInterface(Data)) > 0) &&
       ((strlen(InjectIpcGetAddress(Data)) == 0) ||
        (strcmp(InjectIpcGetAddress(Data), "0.0.0.0") == 0)))
   {
      InjectIpcSetClientSock(Data,
                             MrIpcConnectIf(InjectIpcGetInterface(Data),
                                            InjectIpcGetServerPort(Data)));
   }
   else
   {
      InjectIpcSetClientSock(Data,
                             MrIpcConnect(InjectIpcGetAddress(Data),
                                          InjectIpcGetServerPort(Data)));
   }
}

static void SendCmd(int socket, MrIpcCommandValue Command, int SenderSocket,
                    int ReceiverSocket, unsigned int CanResponse,
                    unsigned int CanHash, unsigned int CanCommand,
                    unsigned int CanPrio, unsigned long Lp1,
                    unsigned int Ip1, unsigned int Ip2)
{  MrIpcCmdType Cmd;

   MrIpcInit(&Cmd);
   MrIpcSetCommand(&Cmd, Command);
   MrIpcSetSenderSocket(&Cmd, SenderSocket);
   MrIpcSetReceiverSocket(&Cmd, ReceiverSocket);
   MrIpcSetCanResponse(&Cmd, CanResponse);
   MrIpcSetCanHash(&Cmd, CanHash);
   MrIpcSetCanCommand(&Cmd, CanCommand);
   MrIpcSetCanPrio(&Cmd, CanPrio);
   MrIpcSetIntLp1(&Cmd, Lp1);
   MrIpcSetIntIp1(&Cmd, Ip1);
   MrIpcSetIntIp2(&Cmd, Ip2);
   MrIpcSend(socket, &Cmd);
}

void InjectIpcRunCmd(InjectIpcStruct *Data, MrIpcCommandValue Command,
                     int SenderSocket, int ReceiverSocket,
                     unsigned int CanResponse, unsigned int CanHash,
                     unsigned int CanCommand, unsigned int CanPrio,
                     unsigned long Lp1, unsigned int Ip1, unsigned int Ip2)
{
   OpenSocket(Data);
   if (InjectIpcGetClientSock(Data) >= 0)
   {
      SendCmd(InjectIpcGetClientSock(Data), Command, SenderSocket,
              ReceiverSocket, CanResponse, CanHash, CanCommand, CanPrio,
              Lp1, Ip1, Ip2);
      MrIpcClose(InjectIpcGetClientSock(Data));
   }

}

void InjectIpcRunFile(InjectIpcStruct *Data, char *Filename)
{  FILE *CmdStream;
   char Line[80], *Param;
   MrIpcCommandValue Command;
   int SenderSocket, ReceiverSocket;
   unsigned int CanResponse, CanHash, CanCommand, CanPrio, Ip1, Ip2;
   unsigned long Lp1;

   CmdStream = fopen(Filename, "r");
   if (CmdStream != (FILE *)NULL)
   {
      OpenSocket(Data);
      if (InjectIpcGetClientSock(Data) >= 0)
      {
         while (fgets(Line, sizeof(Line), CmdStream) != (char *)NULL)
         {
            Param = strtok(Line, " ");
            Command = strtol(Param, (char **)NULL, 0);
            Param = strtok((char *)NULL, " ");
            SenderSocket = strtol(Param, (char **)NULL, 0);
            Param = strtok((char *)NULL, " ");
            ReceiverSocket = strtol(Param, (char **)NULL, 0);
            Param = strtok((char *)NULL, " ");
            CanResponse = strtol(Param, (char **)NULL, 0);
            Param = strtok((char *)NULL, " ");
            CanHash = strtol(Param, (char **)NULL, 0);
            Param = strtok((char *)NULL, " ");
            CanCommand = strtol(Param, (char **)NULL, 0);
            Param = strtok((char *)NULL, " ");
            CanPrio = strtol(Param, (char **)NULL, 0);
            Param = strtok((char *)NULL, " ");
            Lp1 = strtol(Param, (char **)NULL, 0);
            Param = strtok((char *)NULL, " ");
            Ip1 = strtol(Param, (char **)NULL, 0);
            Param = strtok((char *)NULL, " ");
            Ip2 = strtol(Param, (char **)NULL, 0);
            SendCmd(InjectIpcGetClientSock(Data), Command, SenderSocket,
                    ReceiverSocket, CanResponse, CanHash, CanCommand, CanPrio,
                    Lp1, Ip1, Ip2);
         }
         MrIpcClose(InjectIpcGetClientSock(Data));
      }
      fclose(CmdStream);
   }
}
