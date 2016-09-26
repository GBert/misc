#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecCfgdatStream6(MrCs2CanDataType *CanMsg, unsigned long *Length,
                           int *Crc)
{
   *Length = GetLongFromByteArray((char *)CanMsg->Data);
   *Crc = GetIntFromByteArray((char *)&(CanMsg->Data[4]));
}

void MrCs2DecCfgdatStream7(MrCs2CanDataType *CanMsg, unsigned long *Length,
                           int *Crc)
{
   *Length = GetLongFromByteArray((char *)CanMsg->Data);
   *Crc = GetIntFromByteArray((char *)&(CanMsg->Data[4]));
}

void MrCs2DecCfgdatStream8(MrCs2CanDataType *CanMsg, char *Bytes)
{  int i;

   for (i = 0; i < 8; i++)
      Bytes[i] = CanMsg->Data[i];
}
