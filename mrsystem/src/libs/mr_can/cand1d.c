#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecStatus5(MrCs2CanDataType *CanMsg, unsigned long *Uid, int *Index)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *Index = CanMsg->Data[4];
}

void MrCs2DecStatus6(MrCs2CanDataType *CanMsg, unsigned long *Uid, int *Index,
                     int *NumPackets)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *Index = CanMsg->Data[4];
   *NumPackets = CanMsg->Data[5];
}
