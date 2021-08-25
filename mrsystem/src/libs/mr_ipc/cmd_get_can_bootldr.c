#include <stddef.h>
#include <string.h>
#include "mr_ipc.h"

void MrIpcCmdGetCanBootldr(MrIpcCmdType *Data, unsigned *Dlc,
                           char *CanData)
{
   *Dlc = (unsigned int)MrIpcGetRawDlc(Data);
   memcpy(CanData, MrIpcGetRawData(Data), MR_CS2_NUM_CAN_BYTES);
}
