#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecWriteConfig(MrCs2CanDataType *CanMsg, unsigned long *LocId,
                         int *CvIndex, int *Value, int *CtrlRslt)
{
   *LocId = GetLongFromByteArray(CanMsg->Data);
   *CvIndex = GetIntFromByteArray(&(CanMsg->Data[4]));
   *Value = (int)(CanMsg->Data[6]);
   *CtrlRslt = (int)(CanMsg->Data[7]);
}
