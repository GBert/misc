#include <bytestream.h>
#include "cs2.h"

void Cs2DecSysOverload(unsigned char *CanData, unsigned long *Uid,
                       int *Channel)
{
   *Uid = GetLongFromByteArray(CanData);
   *Channel = (int)(CanData[5]);
}
