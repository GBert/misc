#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecAccSwitch6(MrCs2CanDataType *CanMsg, unsigned long *LocId,
                        int *Position, int *Current)
{
   *LocId = GetLongFromByteArray((char *)CanMsg->Data);
   *Position = CanMsg->Data[4];
   *Current = CanMsg->Data[5];
}

void MrCs2DecAccSwitch8(MrCs2CanDataType *CanMsg, unsigned long *LocId,
                        int *Position, int *Current, int *TimeFkt)
{
   *LocId = GetLongFromByteArray((char *)CanMsg->Data);
   *Position = CanMsg->Data[4];
   *Current = CanMsg->Data[5];
   *TimeFkt = GetIntFromByteArray((char *)&(CanMsg->Data[6]));
}
