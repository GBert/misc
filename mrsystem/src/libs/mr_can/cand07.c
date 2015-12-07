#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecReadConfig6(MrCs2CanDataType *CanMsg, unsigned long *LocId,
                         int *CvIndex)
{
   *LocId = GetLongFromByteArray((char *)CanMsg->Data);
   *CvIndex = GetIntFromByteArray((char *)&(CanMsg->Data[4]));
}

void MrCs2DecReadConfig7(MrCs2CanDataType *CanMsg, unsigned long *LocId,
                         int *CvIndex, int *Param)
{
   *LocId = GetLongFromByteArray((char *)CanMsg->Data);
   *CvIndex = GetIntFromByteArray((char *)&(CanMsg->Data[4]));
   *Param = CanMsg->Data[6];
}
