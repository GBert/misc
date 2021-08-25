#include <cs2.h>
#include <mr_ipc.h>
#include "mr_ipchl.h"

void MrIpcHlStatusNameResponse(MrIpcCmdType *Cmd, unsigned long Uid,
                           int ReceiverSock, unsigned char *StatusName)
{
   MrIpcInit(Cmd);
   MrIpcSetSenderSocket(Cmd, MR_IPC_SOCKET_ALL);
   MrIpcSetReceiverSocket(Cmd, ReceiverSock);
   MrIpcSetCanResponse(Cmd, 1);
   MrIpcCalcHash(Cmd, Uid);
   MrIpcSetCanCommand(Cmd, CS2_CMD_STATUS);
   MrIpcCmdSetStatusData(Cmd, StatusName);
}
