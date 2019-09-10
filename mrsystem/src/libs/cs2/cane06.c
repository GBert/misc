#include <bytestream.h>
#include "cs2.h"

void Cs2EncFunction5(unsigned char *CanData, unsigned long LocId,
                     int Function)
{
   SetLongToByteArray(CanData, LocId);
   CanData[4] = (unsigned char)Function;
}

void Cs2EncFunction6(unsigned char *CanData, unsigned long LocId,
                     int Function, int Value)
{
   SetLongToByteArray(CanData, LocId);
   CanData[4] = (unsigned char)Function;
   CanData[5] = (unsigned char)Value;
}

void Cs2EncFunction8(unsigned char *CanData, unsigned long LocId,
                     int Function, int Value, int FktVal)
{
   SetLongToByteArray(CanData, LocId);
   CanData[4] = (unsigned char)Function;
   CanData[5] = (unsigned char)Value;
   SetIntToByteArray(&(CanData[6]), FktVal);
}
