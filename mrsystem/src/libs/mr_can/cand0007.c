#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecSysMfxFastRead(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                            int *MfxSid)
{
   *Uid = GetLongFromByteArray(CanMsg->Data);
   *MfxSid = GetIntFromByteArray(&(CanMsg->Data[5]));
}
