#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecSysSystemReset(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                            int *ResetTarget)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *ResetTarget = CanMsg->Data[5];
}
