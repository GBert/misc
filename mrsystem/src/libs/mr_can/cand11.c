#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecS88Event4(MrCs2CanDataType *CanMsg, int *Device, int *Contact)
{
   *Device = GetIntFromByteArray(CanMsg->Data);
   *Contact = GetIntFromByteArray(&(CanMsg->Data[2]));
}

void MrCs2DecS88Event5(MrCs2CanDataType *CanMsg, int *Device, int *Contact,
                       int *Param)
{
   *Device = GetIntFromByteArray(CanMsg->Data);
   *Contact = GetIntFromByteArray(&(CanMsg->Data[2]));
   *Param = (int)(CanMsg->Data[4]);
}

void MrCs2DecS88Event8(MrCs2CanDataType *CanMsg, int *Device, int *Contact,
                       int *StateOld, int *StateNew, int *Time)
{
   *Device = GetIntFromByteArray(CanMsg->Data);
   *Contact = GetIntFromByteArray(&(CanMsg->Data[2]));
   *StateOld = (int)(CanMsg->Data[4]);
   *StateNew = (int)(CanMsg->Data[5]);
   *Time = GetIntFromByteArray(&(CanMsg->Data[6]));
}
