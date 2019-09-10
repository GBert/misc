#include <bytestream.h>
#include "cs2.h"

void Cs2DecDirection4(unsigned char *CanData, unsigned long *LocId)
{
   *LocId = GetLongFromByteArray(CanData);
}

void Cs2DecDirection5(unsigned char *CanData, unsigned long *LocId,
                      int *Direction)
{
   *LocId = GetLongFromByteArray(CanData);
   *Direction = (int)(CanData[4]);
}
