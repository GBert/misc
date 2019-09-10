#include <bytestream.h>
#include "cs2.h"

void Cs2DecSysSystemId5(unsigned char *CanData, unsigned long *Uid)
{
   *Uid = GetLongFromByteArray(CanData);
}

void Cs2DecSysSystemId7(unsigned char *CanData, unsigned long *Uid,
                        int *Id)
{
   *Uid = GetLongFromByteArray(CanData);
   *Id = GetIntFromByteArray(&(CanData[5]));
}
