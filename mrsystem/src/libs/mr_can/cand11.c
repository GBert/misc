#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecS88Event4(MrCs2CanDataType *CanMsg, int *Device, int *Contact)
{
   *Device = GetIntFromByteArray((char *)CanMsg->Data);
   *Contact = GetIntFromByteArray((char *)&(CanMsg->Data[2]));
}

void MrCs2DecS88Event5(MrCs2CanDataType *CanMsg, int *Device, int *Contact,
                       int *Param)
{
   *Device = GetIntFromByteArray((char *)CanMsg->Data);
   *Contact = GetIntFromByteArray((char *)&(CanMsg->Data[2]));
   *Param = CanMsg->Data[4];
}

void MrCs2DecS88Event8(MrCs2CanDataType *CanMsg, int *Device, int *Contact,
                       int *StateOld, int *StateNew, int *Time)
{
   *Device = GetIntFromByteArray((char *)CanMsg->Data);
   *Contact = GetIntFromByteArray((char *)&(CanMsg->Data[2]));
   *StateOld = CanMsg->Data[4];
   *StateNew = CanMsg->Data[5];
   *Time = GetIntFromByteArray((char *)&(CanMsg->Data[6]));
}
