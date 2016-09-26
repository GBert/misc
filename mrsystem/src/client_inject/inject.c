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

void InjectRun(InjectStruct *Data)
{  MrIpcCmdType CmdFrame;

   if (strlen(InjectGetInterface(Data)) > 0)
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
