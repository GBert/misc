#include <bytestream.h>
#include "cs2.h"

void Cs2DecBind(unsigned char *CanData, unsigned long *Uid,
                unsigned int *Sid)
{
   *Uid = GetLongFromByteArray(CanData);
   *Sid = GetIntFromByteArray(&(CanData[4]));
}
