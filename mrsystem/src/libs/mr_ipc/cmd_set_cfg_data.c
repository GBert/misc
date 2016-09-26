#include <stddef.h>
#include <string.h>
#include "mr_ipc.h"

void MrIpcCmdSetCfgData(MrIpcCmdType *Data, char *Buf)
{
   MrIpcSetCommand(Data, MrIpcCmdCfgData);
   memcpy(MrIpcGetRawData(Data), Buf, MR_CS2_NUM_CAN_BYTES);
}
