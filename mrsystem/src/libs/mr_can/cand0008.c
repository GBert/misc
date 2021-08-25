#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecSysTrackProtocoll(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                               int *Proto)
{
   *Uid = GetLongFromByteArray(CanMsg->Data);
   *Proto = (int)(CanMsg->Data[5]);
}
