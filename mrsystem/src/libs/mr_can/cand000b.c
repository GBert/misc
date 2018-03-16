#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecSysStatus6(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                        int *Channel)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *Channel = CanMsg->Data[5];
}

void MrCs2DecSysStatus7(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                        int *Channel, int *Set)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *Channel = CanMsg->Data[5];
   *Set = CanMsg->Data[6];
}

void MrCs2DecSysStatus8(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                        int *Channel, int *Value)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *Channel = CanMsg->Data[5];
   *Value = GetIntFromByteArray((char *)&(CanMsg->Data[6]));
}
