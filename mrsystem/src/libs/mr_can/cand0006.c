#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecSysDecoderTimeing(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                               int *Time)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *Time = GetIntFromByteArray((char *)&(CanMsg->Data[5]));
}
