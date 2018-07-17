#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecSysResetMfxCounter(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                                int *Counter)
{
   *Uid = GetLongFromByteArray(CanMsg->Data);
   *Counter = GetIntFromByteArray(&(CanMsg->Data[5]));
}
