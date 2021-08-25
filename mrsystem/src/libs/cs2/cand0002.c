#include <bytestream.h>
#include "cs2.h"

void Cs2DecSysHalt(unsigned char *CanData, unsigned long *Uid)
{
   *Uid = GetLongFromByteArray(CanData);
}
