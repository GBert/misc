#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecDiscovery0(MrCs2CanDataType *CanMsg)
{
}

void MrCs2DecDiscovery1(MrCs2CanDataType *CanMsg, int *Proto)
{
   *Proto = (int)(CanMsg->Data[0]);
}

void MrCs2DecDiscovery5(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                        int *Range)
{
   *Uid = GetLongFromByteArray(CanMsg->Data);
   *Range = (int)(CanMsg->Data[4]);
}

void MrCs2DecDiscovery6(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                        int *Range, int *Ask)
{
   *Uid = GetLongFromByteArray(CanMsg->Data);
   *Range = (int)(CanMsg->Data[4]);
   *Ask = (int)(CanMsg->Data[5]);
}
