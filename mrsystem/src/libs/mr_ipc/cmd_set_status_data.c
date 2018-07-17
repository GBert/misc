#include <stddef.h>
#include <string.h>
#include "mr_ipc.h"

void MrIpcCmdSetStatusData(MrIpcCmdType *Data, unsigned char *Buf)
{
   if (Data != (MrIpcCmdType *)NULL)
   {
      MrIpcSetCommand(Data, MrIpcCmdStatusData);
      memcpy(MrIpcGetRawData(Data), Buf, MR_CS2_NUM_CAN_BYTES);
   }
}
