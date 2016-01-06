#include <stddef.h>
#include <string.h>
#include "mr_ipc.h"

void MrIpcCmdGetNull(MrIpcCmdType *Data, unsigned char *Dlc, char *CanData)
{
   *Dlc = MrIpcGetRawDlc(Data);
   memcpy(CanData, MrIpcGetRawData(Data), MR_CS2_NUM_CAN_BYTES);
}
