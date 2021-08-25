#include <bytestream.h>
#include "cs2.h"

void Cs2DecSysEmergencyHalt(unsigned char *CanData, unsigned long *Uid)
{
   *Uid = GetLongFromByteArray(CanData);
}
