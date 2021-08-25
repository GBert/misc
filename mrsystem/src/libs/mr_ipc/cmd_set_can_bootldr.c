#include <stddef.h>
#include <string.h>
#include "mr_ipc.h"

void MrIpcCmdSetCanBootldr(MrIpcCmdType *Data, unsigned Dlc,
                           unsigned char *CanData)
{
   MrIpcSetCommand(Data, MrIpcCmdCanBootldrGeb);
   MrIpcSetRawDlc(Data, Dlc);
   memcpy(MrIpcGetRawData(Data), CanData, MR_CS2_NUM_CAN_BYTES);
}
