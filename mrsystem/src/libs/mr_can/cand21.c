#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecCfgdatStream6(MrCs2CanDataType *CanMsg, unsigned long *Length,
                           int *Crc)
{
   *Length = GetLongFromByteArray(CanMsg->Data);
   *Crc = GetIntFromByteArray(&(CanMsg->Data[4]));
}

void MrCs2DecCfgdatStream7(MrCs2CanDataType *CanMsg, unsigned long *Length,
                           int *Crc)
{
   *Length = GetLongFromByteArray(CanMsg->Data);
   *Crc = GetIntFromByteArray(&(CanMsg->Data[4]));
}

void MrCs2DecCfgdatStream8(MrCs2CanDataType *CanMsg, char *Bytes)
{  int i;

   for (i = 0; i < 8; i++)
      Bytes[i] = (char)(CanMsg->Data[i]);
}
