#include <bytestream.h>
#include "cs2.h"

void Cs2EncDirection4(unsigned char *CanData, unsigned long LocId)
{
   SetLongToByteArray(CanData, LocId);
}

void Cs2EncDirection5(unsigned char *CanData, unsigned long LocId,
                      int Direction)
{
   SetLongToByteArray(CanData, LocId);
   CanData[4] = (unsigned char)Direction;
}
