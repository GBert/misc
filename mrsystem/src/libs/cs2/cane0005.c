#include <bytestream.h>
#include "cs2.h"

void Cs2EncSysDataProtocoll(unsigned char *CanData, unsigned long Uid,
                            int Proto)
{
   SetLongToByteArray(CanData, Uid);
}
