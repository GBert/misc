#include <bytestream.h>
#include "cs2.h"

void Cs2DecSysTrackProtocoll(unsigned char *CanData, unsigned long *Uid,
                             int *Proto)
{
   *Uid = GetLongFromByteArray(CanData);
   *Proto = (int)(CanData[5]);
}
