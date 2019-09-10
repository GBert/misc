#include <bytestream.h>
#include "cs2.h"

void Cs2DecReadConfig6(unsigned char *CanData, unsigned long *LocId,
                       int *CvIndex)
{
   *LocId = GetLongFromByteArray(CanData);
   *CvIndex = GetIntFromByteArray(&(CanData[4]));
}

void Cs2DecReadConfig7(unsigned char *CanData, unsigned long *LocId,
                       int *CvIndex, int *Param)
{
   *LocId = GetLongFromByteArray(CanData);
   *CvIndex = GetIntFromByteArray(&(CanData[4]));
   *Param = (int)(CanData[6]);
}
