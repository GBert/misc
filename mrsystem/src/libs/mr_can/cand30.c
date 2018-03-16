#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecAutomatic6(MrCs2CanDataType *CanMsg, int *DeviceId, int *Func,
                        int *Status, int *Param)
{
   *DeviceId = GetIntFromByteArray((char *)CanMsg->Data);
   *Func = GetIntFromByteArray((char *)&(CanMsg->Data[2]));
   *Status = CanMsg->Data[4];
   *Param = CanMsg->Data[5];
}

void MrCs2DecAutomatic8(MrCs2CanDataType *CanMsg, int *DeviceId, int *Func,
                        unsigned long *LocId)
{
   *DeviceId = GetIntFromByteArray((char *)CanMsg->Data);
   *Func = GetIntFromByteArray((char *)&(CanMsg->Data[2]));
   *LocId = GetLongFromByteArray((char *)&(CanMsg->Data[4]));
}
