#include <mr_ipc.h>
#include "mr_ipchl.h"

void MrIpcHlLocinfoRequest(MrIpcCmdType *Cmd, unsigned long Uid,
                           int SenderSock, int ReceiverSocket,
                           char *LokName)
{
   MrIpcInit(Cmd);
   MrIpcSetSenderSocket(Cmd, SenderSock);
   MrIpcSetReceiverSocket(Cmd, ReceiverSocket);
   MrIpcSetCanResponse(Cmd, 0);
   MrIpcCalcHash(Cmd, Uid);
   MrIpcSetCanCommand(Cmd, CS2_CMD_CONFIG_QUERY);
   MrIpcSetCanPrio(Cmd, CS2_PRIO_2);
   MrIpcCmdSetReqestLocinfo(Cmd, LokName);
}
