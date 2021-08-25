#include <bytestream.h>
#include "cs2.h"

void Cs2DecSysDecoderTimeing(unsigned char *CanData, unsigned long *Uid,
                             int *Time)
{
   *Uid = GetLongFromByteArray(CanData);
   *Time = GetIntFromByteArray(&(CanData[5]));
}
