#include <bytestream.h>
#include "cs2.h"

void Cs2EncSpeed4(unsigned char *CanData, unsigned long LocId)
{
   SetLongToByteArray(CanData, LocId);
}

void Cs2EncSpeed6(unsigned char *CanData, unsigned long LocId, int Speed)
{
   SetLongToByteArray(CanData, LocId);
   SetIntToByteArray(&(CanData[4]), Speed);
}
