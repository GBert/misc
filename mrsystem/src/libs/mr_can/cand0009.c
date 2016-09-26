#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecSysResetMfxCounter(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                                int *Counter)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *Counter = GetIntFromByteArray((char *)&(CanMsg->Data[5]));
}
