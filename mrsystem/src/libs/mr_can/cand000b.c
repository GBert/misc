#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecSysStatus6(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                        int *Channel)
{
   *Uid = GetLongFromByteArray(CanMsg->Data);
   *Channel = (int)(CanMsg->Data[5]);
}

void MrCs2DecSysStatus7(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                        int *Channel, int *Set)
{
   *Uid = GetLongFromByteArray(CanMsg->Data);
   *Channel = (int)(CanMsg->Data[5]);
   *Set = (int)(CanMsg->Data[6]);
}

void MrCs2DecSysStatus8(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                        int *Channel, int *Value)
{
   *Uid = GetLongFromByteArray(CanMsg->Data);
   *Channel = (int)(CanMsg->Data[5]);
   *Value = GetIntFromByteArray(&(CanMsg->Data[6]));
}
