#include <bytestream.h>
#include "cs2.h"

void Cs2DecFunction5(unsigned char *CanData, unsigned long *LocId,
                     int *Function)
{
   *LocId = GetLongFromByteArray(CanData);
   *Function = (int)(CanData[4]);
}

void Cs2DecFunction6(unsigned char *CanData, unsigned long *LocId,
                     int *Function, int *Value)
{
   *LocId = GetLongFromByteArray(CanData);
   *Function = (int)(CanData[4]);
   *Value = (int)(CanData[5]);
}

void Cs2DecFunction8(unsigned char *CanData, unsigned long *LocId,
                     int *Function, int *Value, int *FktVal)
{
   *LocId = GetLongFromByteArray(CanData);
   *Function = (int)(CanData[4]);
   *Value = (int)(CanData[5]);
   *FktVal = GetIntFromByteArray(&(CanData[6]));
}
