#include <bytestream.h>
#include "cs2.h"

void Cs2DecAccSwitch6(unsigned char *CanData, unsigned long *LocId,
                      int *Position, int *Current)
{
   *LocId = GetLongFromByteArray(CanData);
   *Position = (int)(CanData[4]);
   *Current = (int)(CanData[5]);
}

void Cs2DecAccSwitch8(unsigned char *CanData, unsigned long *LocId,
                      int *Position, int *Current, int *TimeFkt)
{
   *LocId = GetLongFromByteArray(CanData);
   *Position = (int)(CanData[4]);
   *Current = (int)(CanData[5]);
   *TimeFkt = GetIntFromByteArray(&(CanData[6]));
}
