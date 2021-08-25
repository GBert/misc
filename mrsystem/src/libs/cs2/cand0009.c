#include <bytestream.h>
#include "cs2.h"

void Cs2DecSysResetMfxCounter(unsigned char *CanData, unsigned long *Uid,
                              int *Counter)
{
   *Uid = GetLongFromByteArray(CanData);
   *Counter = GetIntFromByteArray(&(CanData[5]));
}
