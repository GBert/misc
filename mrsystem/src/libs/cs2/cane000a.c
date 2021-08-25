#include <bytestream.h>
#include "cs2.h"

void Cs2EncSysOverload(unsigned char *CanData, unsigned long Uid,
                       int Channel)
{
   SetLongToByteArray(CanData, Uid);
   CanData[5] = (unsigned char)Channel;
}
