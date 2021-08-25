#include <bytestream.h>
#include "cs2.h"

void Cs2DecWriteConfig(unsigned char *CanData, unsigned long *LocId,
                       int *CvIndex, int *Value, int *CtrlRslt)
{
   *LocId = GetLongFromByteArray(CanData);
   *CvIndex = GetIntFromByteArray(&(CanData[4]));
   *Value = (int)(CanData[6]);
   *CtrlRslt = (int)(CanData[7]);
}
