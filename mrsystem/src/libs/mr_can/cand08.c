#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecWriteConfig(MrCs2CanDataType *CanMsg, unsigned long *LocId,
                         int *CvIndex, int *Value, int *CtrlRslt)
{
   *LocId = GetLongFromByteArray((char *)CanMsg->Data);
   *CvIndex = GetIntFromByteArray((char *)&(CanMsg->Data[4]));
   *Value = CanMsg->Data[6];
   *CtrlRslt = CanMsg->Data[7];
}
