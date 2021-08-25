#include <stddef.h>
#include <string.h>
#include "mr_ipc.h"

void MrIpcCmdSetNull(MrIpcCmdType *Data, unsigned char Dlc,
                     unsigned char *CanData)
{
   MrIpcSetCommand(Data, MrIpcCmdNull);
   MrIpcSetRawDlc(Data, Dlc);
   memcpy(MrIpcGetRawData(Data), CanData, MR_CS2_NUM_CAN_BYTES);
}
