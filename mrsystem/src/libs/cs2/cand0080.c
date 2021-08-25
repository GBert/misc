#include <bytestream.h>
#include "cs2.h"

void Cs2DecSysSystemReset(unsigned char *CanData, unsigned long *Uid,
                          int *ResetTarget)
{
   *Uid = GetLongFromByteArray(CanData);
   *ResetTarget = (int)(CanData[5]);
}
