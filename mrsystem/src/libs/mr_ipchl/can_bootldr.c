#include <cs2.h>
#include <mr_ipc.h>
#include "mr_ipchl.h"

void MrIpcHlCanBootldr(MrIpcCmdType *Cmd, unsigned long Uid,
                       unsigned long Length, unsigned char *Data)
{
   MrIpcInit(Cmd);
   MrIpcSetSenderSocket(Cmd, MR_IPC_SOCKET_ALL);
   MrIpcSetReceiverSocket(Cmd, MR_IPC_SOCKET_ALL);
   MrIpcSetCanResponse(Cmd, 0);
   MrIpcCalcHash(Cmd, Uid);
   MrIpcSetCanCommand(Cmd, CS2_CMD_BOOTLDR_CAN);
   MrIpcSetCanPrio(Cmd, CS2_PRIO_2);
   MrIpcCmdSetCanBootldr(Cmd, Length, Data);
}
