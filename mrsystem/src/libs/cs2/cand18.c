#include <bytestream.h>
#include "cs2.h"

void Cs2DecPing0(unsigned char *CanData)
{
}

void Cs2DecPing8(unsigned char *CanData, unsigned long *Uid,
                 int *SwVersion, int *DeviceId)
{
   *Uid = GetLongFromByteArray(CanData);
   *SwVersion = GetIntFromByteArray(&(CanData[4]));
   *DeviceId = GetIntFromByteArray(&(CanData[6]));
}
