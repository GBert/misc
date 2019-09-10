#include <bytestream.h>
#include "cs2.h"

void Cs2DecSysMfxFastRead(unsigned char *CanData, unsigned long *Uid,
                          int *MfxSid)
{
   *Uid = GetLongFromByteArray(CanData);
   *MfxSid = GetIntFromByteArray(&(CanData[5]));
}
