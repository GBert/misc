#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecFunction5(MrCs2CanDataType *CanMsg, unsigned long *LocId,
                       int *Function)
{
   *LocId = GetLongFromByteArray(CanMsg->Data);
   *Function = (int)(CanMsg->Data[4]);
}

void MrCs2DecFunction6(MrCs2CanDataType *CanMsg, unsigned long *LocId,
                       int *Function, int *Value)
{
   *LocId = GetLongFromByteArray(CanMsg->Data);
   *Function = (int)(CanMsg->Data[4]);
   *Value = (int)(CanMsg->Data[5]);
}

void MrCs2DecFunction8(MrCs2CanDataType *CanMsg, unsigned long *LocId,
                       int *Function, int *Value, int *FktVal)
{
   *LocId = GetLongFromByteArray(CanMsg->Data);
   *Function = (int)(CanMsg->Data[4]);
   *Value = (int)(CanMsg->Data[5]);
   *FktVal = GetIntFromByteArray(&(CanMsg->Data[6]));
}
