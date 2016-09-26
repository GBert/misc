#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecSysOverload(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                         int *Channel)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *Channel = CanMsg->Data[5];
}
