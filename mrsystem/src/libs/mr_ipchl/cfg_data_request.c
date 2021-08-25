#include <cs2.h>
#include <mr_ipc.h>
#include "mr_ipchl.h"

void MrIpcHlCfgDataRequest(MrIpcCmdType *Cmd, unsigned long Uid,
                           int ReceiverSock, char *Name)
{
   MrIpcInit(Cmd);
   MrIpcSetSenderSocket(Cmd, MR_IPC_SOCKET_ALL);
   MrIpcSetReceiverSocket(Cmd, ReceiverSock);
   MrIpcSetCanResponse(Cmd, 0);
   MrIpcCalcHash(Cmd, Uid);
   MrIpcSetCanCommand(Cmd, CS2_CMD_CFGDAT_STREAM);
   MrIpcSetCanPrio(Cmd, CS2_PRIO_0);
   MrIpcCmdSetCfgData(Cmd, Name);
}
