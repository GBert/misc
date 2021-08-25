#include <bytestream.h>
#include "cs2.h"

void Cs2EncSysResetMfxCounter(unsigned char *CanData, unsigned long Uid,
                              int Counter)
{
   SetLongToByteArray(CanData, Uid);
   SetIntToByteArray(&(CanData[5]), Counter);
}
