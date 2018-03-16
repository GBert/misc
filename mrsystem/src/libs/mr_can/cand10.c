#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecS88Poll5(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                      int *NumModuls)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *NumModuls = CanMsg->Data[4];
}

void MrCs2DecS88Poll7(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                      int *Modul, int *State)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *Modul = CanMsg->Data[4];
   *State = GetIntFromByteArray((char *)&(CanMsg->Data[5]));
}
