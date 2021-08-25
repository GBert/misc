#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecStatus5(MrCs2CanDataType *CanMsg, unsigned long *Uid, int *Index)
{
   *Uid = GetLongFromByteArray(CanMsg->Data);
   *Index = (int)(CanMsg->Data[4]);
}

void MrCs2DecStatus6(MrCs2CanDataType *CanMsg, unsigned long *Uid, int *Index,
                     int *NumPackets)
{
   *Uid = GetLongFromByteArray(CanMsg->Data);
   *Index = (int)(CanMsg->Data[4]);
   *NumPackets = (int)(CanMsg->Data[5]);
}

void MrCs2DecStatus8(MrCs2CanDataType *CanMsg, char *Bytes)
{  int i;

   for (i = 0; i < 8; i++)
      Bytes[i] = (char)(CanMsg->Data[i]);
}
