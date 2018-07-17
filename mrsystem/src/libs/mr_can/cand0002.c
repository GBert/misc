#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecSysHalt(MrCs2CanDataType *CanMsg, unsigned long *Uid)
{
   *Uid = GetLongFromByteArray(CanMsg->Data);
}
