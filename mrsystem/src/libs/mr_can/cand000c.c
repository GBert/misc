#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecSysSystemId5(MrCs2CanDataType *CanMsg, unsigned long *Uid)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
}

void MrCs2DecSysSystemId7(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                         int *Id)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *Id = GetIntFromByteArray((char *)&(CanMsg->Data[5]));
}
