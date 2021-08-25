#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecSysDecoderTimeing(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                               int *Time)
{
   *Uid = GetLongFromByteArray(CanMsg->Data);
   *Time = GetIntFromByteArray(&(CanMsg->Data[5]));
}
