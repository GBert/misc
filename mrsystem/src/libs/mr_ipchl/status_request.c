#include <cs2.h>
#include <mr_ipc.h>
#include "mr_ipchl.h"

void MrIpcHlStatusRequest(MrIpcCmdType *Cmd, unsigned long Uid,
                          unsigned long DstUid, unsigned int Index)
{
   MrIpcInit(Cmd);
   MrIpcSetSenderSocket(Cmd, MR_IPC_SOCKET_ALL);
   MrIpcSetReceiverSocket(Cmd, MR_IPC_SOCKET_ALL);
   MrIpcSetCanResponse(Cmd, 0);
   MrIpcCalcHash(Cmd, Uid);
   MrIpcSetCanCommand(Cmd, CS2_CMD_STATUS);
   MrIpcSetCanPrio(Cmd, CS2_PRIO_2);
   MrIpcCmdSetStatusRequest(Cmd, DstUid, Index);
}
