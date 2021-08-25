#include <bytestream.h>
#include "cs2.h"

void Cs2DecSysDataProtocoll(unsigned char *CanData, unsigned long *Uid,
                            int *Proto)
{
   *Uid = GetLongFromByteArray(CanData);
   *Proto = CanData[5];
}
