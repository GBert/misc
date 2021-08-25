#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecSpeed4(MrCs2CanDataType *CanMsg, unsigned long *LocId)
{
   *LocId = GetLongFromByteArray(CanMsg->Data);
}

void MrCs2DecSpeed6(MrCs2CanDataType *CanMsg, unsigned long *LocId, int *Speed)
{
   *LocId = GetLongFromByteArray(CanMsg->Data);
   *Speed = GetIntFromByteArray(&(CanMsg->Data[4]));
}
