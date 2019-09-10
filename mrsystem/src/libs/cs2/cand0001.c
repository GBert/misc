#include <bytestream.h>
#include "cs2.h"

void Cs2DecSysGo(unsigned char *CanData, unsigned long *Uid)
{
   *Uid = GetLongFromByteArray(CanData);
}
