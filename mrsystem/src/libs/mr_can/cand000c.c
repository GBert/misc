#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecSysSystemId5(MrCs2CanDataType *CanMsg, unsigned long *Uid)
{
   *Uid = GetLongFromByteArray(CanMsg->Data);
}

void MrCs2DecSysSystemId7(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                         int *Id)
{
   *Uid = GetLongFromByteArray(CanMsg->Data);
   *Id = GetIntFromByteArray(&(CanMsg->Data[5]));
}
