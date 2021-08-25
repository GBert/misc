#include <bytestream.h>
#include "cs2.h"

void Cs2DecAutomatic6(unsigned char *CanData, int *DeviceId, int *Func,
                      int *Status, int *Param)
{
   *DeviceId = GetIntFromByteArray(CanData);
   *Func = GetIntFromByteArray(&(CanData[2]));
   *Status = CanData[4];
   *Param = CanData[5];
}

void Cs2DecAutomatic8(unsigned char *CanData, int *DeviceId, int *Func,
                      unsigned long *LocId)
{
   *DeviceId = GetIntFromByteArray(CanData);
   *Func = GetIntFromByteArray(&(CanData[2]));
   *LocId = GetLongFromByteArray(&(CanData[4]));
}
