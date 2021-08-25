#include <bytestream.h>
#include "cs2.h"

void Cs2EncSysDecoderTimeing(unsigned char *CanData, unsigned long Uid,
                             int Time)
{
   SetLongToByteArray(CanData, Uid);
}
