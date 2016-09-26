#include <stddef.h>
#include <string.h>
#include "mr_ipc.h"

void MrIpcCmdSetReqestLocinfo(MrIpcCmdType *Data, char *Locname)
{
   MrIpcSetCommand(Data, MrIpcCmdRequestLocInfo);
   strcpy((char *)MrIpcGetRawData(Data), Locname);
}
