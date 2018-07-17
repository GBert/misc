#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecAccSwitch6(MrCs2CanDataType *CanMsg, unsigned long *LocId,
                        int *Position, int *Current)
{
   *LocId = GetLongFromByteArray(CanMsg->Data);
   *Position = (int)(CanMsg->Data[4]);
   *Current = (int)(CanMsg->Data[5]);
}

void MrCs2DecAccSwitch8(MrCs2CanDataType *CanMsg, unsigned long *LocId,
                        int *Position, int *Current, int *TimeFkt)
{
   *LocId = GetLongFromByteArray(CanMsg->Data);
   *Position = (int)(CanMsg->Data[4]);
   *Current = (int)(CanMsg->Data[5]);
   *TimeFkt = GetIntFromByteArray(&(CanMsg->Data[6]));
}
