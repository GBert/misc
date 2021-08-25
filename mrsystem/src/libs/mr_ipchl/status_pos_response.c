#include <cs2.h>
#include <mr_ipc.h>
#include "mr_ipchl.h"

void MrIpcHlStatusPosResponse(MrIpcCmdType *Cmd, unsigned long Uid,
                              int ReceiverSock, unsigned long Addr,
                              unsigned int Index, unsigned int NumPackets)
{
   MrIpcInit(Cmd);
   MrIpcSetSenderSocket(Cmd, MR_IPC_SOCKET_ALL);
   MrIpcSetReceiverSocket(Cmd, ReceiverSock);
   MrIpcSetCanResponse(Cmd, 1);
   MrIpcCalcHash(Cmd, Uid);
   MrIpcSetCanCommand(Cmd, CS2_CMD_STATUS);
   MrIpcCmdSetStatusPos(Cmd, Addr, Index, NumPackets);
}
