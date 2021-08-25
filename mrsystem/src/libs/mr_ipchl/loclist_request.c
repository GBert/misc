#include <mr_ipc.h>
#include "mr_ipchl.h"

void MrIpcHlLoclistRequest(MrIpcCmdType *Cmd, unsigned long Uid,
                           int SenderSock, int ReceiverSock)
{
   MrIpcInit(Cmd);
   MrIpcSetSenderSocket(Cmd, SenderSock);
   MrIpcSetReceiverSocket(Cmd, ReceiverSock);
   MrIpcSetCanResponse(Cmd, 0);
   MrIpcCalcHash(Cmd, Uid);
   MrIpcSetCanCommand(Cmd, CS2_CMD_CONFIG_QUERY);
   MrIpcSetCanPrio(Cmd, CS2_PRIO_2);
   MrIpcCmdSetReqestLoclist(Cmd);
}
