#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecS88Poll5(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                      int *NumModuls)
{
   *Uid = GetLongFromByteArray(CanMsg->Data);
   *NumModuls = (int)(CanMsg->Data[4]);
}

void MrCs2DecS88Poll7(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                      int *Modul, int *State)
{
   *Uid = GetLongFromByteArray(CanMsg->Data);
   *Modul = (int)(CanMsg->Data[4]);
   *State = GetIntFromByteArray(&(CanMsg->Data[5]));
}
