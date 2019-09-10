#include <bytestream.h>
#include "cs2.h"

void Cs2DecMfxVerify6(unsigned char *CanData, unsigned long *Uid,
                      unsigned int *Sid)
{
   *Uid = GetLongFromByteArray(CanData);
   *Sid = GetIntFromByteArray(&(CanData[4]));
}

void Cs2DecMfxVerify7(unsigned char *CanData, unsigned long *Uid,
                      unsigned int *Sid, int *Ask)
{
   *Uid = GetLongFromByteArray(CanData);
   *Sid = GetIntFromByteArray(&(CanData[4]));
   *Ask = (int)(CanData[6]);
}
