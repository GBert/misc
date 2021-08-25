#include <bytestream.h>
#include "cs2.h"

void Cs2EncAccSwitch6(unsigned char *CanData, unsigned long LocId,
                      int Position, int Current)
{
   SetLongToByteArray(CanData, LocId);
   CanData[4] = (unsigned char)Position;
   CanData[5] = (unsigned char)Current;
}

void Cs2EncAccSwitch8(unsigned char *CanData, unsigned long LocId,
                      int Position, int Current, int TimeFkt)
{
   SetLongToByteArray(CanData, LocId);
   CanData[4] = (unsigned char)Position;
   CanData[5] = (unsigned char)Current;
   SetIntToByteArray(&(CanData[6]), TimeFkt);
}
