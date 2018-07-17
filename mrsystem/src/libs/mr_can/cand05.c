#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecDirection4(MrCs2CanDataType *CanMsg, unsigned long *LocId)
{
   *LocId = GetLongFromByteArray(CanMsg->Data);
}

void MrCs2DecDirection5(MrCs2CanDataType *CanMsg, unsigned long *LocId,
                        int *Direction)
{
   *LocId = GetLongFromByteArray(CanMsg->Data);
   *Direction = (int)(CanMsg->Data[4]);
}
