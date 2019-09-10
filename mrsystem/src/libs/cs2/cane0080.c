#include <bytestream.h>
#include "cs2.h"

void Cs2EncSysSystemReset(unsigned char *CanData, unsigned long Uid,
                          int ResetTarget)
{
   SetLongToByteArray(CanData, Uid);
   CanData[5] = (unsigned char)ResetTarget;
}
