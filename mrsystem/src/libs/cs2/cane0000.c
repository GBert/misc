#include <bytestream.h>
#include "cs2.h"

void Cs2EncSysStop(unsigned char *CanData, unsigned long Uid)
{
   SetLongToByteArray(CanData, Uid);
}
