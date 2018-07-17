#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecSysStop(MrCs2CanDataType *CanMsg, unsigned long *Uid)
{
   *Uid = GetLongFromByteArray(CanMsg->Data);
}
