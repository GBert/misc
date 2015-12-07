#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecDiscovery0(MrCs2CanDataType *CanMsg)
{
}

void MrCs2DecDiscovery1(MrCs2CanDataType *CanMsg, int *Proto)
{
   *Proto = CanMsg->Data[0];
}

void MrCs2DecDiscovery5(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                        int *Range)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *Range = CanMsg->Data[4];
}

void MrCs2DecDiscovery6(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                        int *Range, int *Ask)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *Range = CanMsg->Data[4];
   *Ask = CanMsg->Data[5];
}
