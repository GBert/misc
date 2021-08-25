#include <bytestream.h>
#include "cs2.h"

void Cs2EncSysMfxFastRead(unsigned char *CanData, unsigned long Uid,
                          int MfxSid)
{
   SetLongToByteArray(CanData, Uid);
   CanData[5] = (unsigned char)MfxSid;
}
