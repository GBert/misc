#include <bytestream.h>
#include "cs2.h"

void Cs2DecSpeed4(unsigned char *CanData, unsigned long *LocId)
{
   *LocId = GetLongFromByteArray(CanData);
}

void Cs2DecSpeed6(unsigned char *CanData, unsigned long *LocId, int *Speed)
{
   *LocId = GetLongFromByteArray(CanData);
   *Speed = GetIntFromByteArray(&(CanData[4]));
}
