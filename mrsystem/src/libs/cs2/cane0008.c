#include <bytestream.h>
#include "cs2.h"

void Cs2EncSysTrackProtocoll(unsigned char *CanData, unsigned long Uid,
                             int Proto)
{
   SetLongToByteArray(CanData, Uid);
   CanData[5] = (unsigned char)Proto;
}
