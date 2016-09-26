#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecPing0(MrCs2CanDataType *CanMsg)
{
}

void MrCs2DecPing8(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                   int *SwVersion, int *DeviceId)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *SwVersion = GetIntFromByteArray((char *)&(CanMsg->Data[4]));
   *DeviceId = GetIntFromByteArray((char *)&(CanMsg->Data[6]));
}
