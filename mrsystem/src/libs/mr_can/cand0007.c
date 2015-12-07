#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecSysMfxFastRead(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                            int *MfxSid)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *MfxSid = GetIntFromByteArray((char *)&(CanMsg->Data[5]));
}
