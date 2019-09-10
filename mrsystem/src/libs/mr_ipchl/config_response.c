#include <cs2.h>
#include <mr_ipc.h>
#include "mr_ipchl.h"

void MrIpcHlConfigResponse(MrIpcCmdType *Cmd, unsigned long Uid, char *CfgFile)
{
   MrIpcInit(Cmd);
   MrIpcSetSenderSocket(Cmd, MR_IPC_SOCKET_ALL);
   MrIpcSetReceiverSocket(Cmd, MR_IPC_SOCKET_ALL);
   MrIpcSetCanResponse(Cmd, 0);
   MrIpcCalcHash(Cmd, Uid);
   MrIpcSetCanCommand(Cmd, CS2_CMD_CONFIG_QUERY);
   MrIpcSetCanPrio(Cmd, CS2_PRIO_2);
   MrIpcCmdSetQuery(Cmd, CS2_CFG_DLC, CfgFile);
}
