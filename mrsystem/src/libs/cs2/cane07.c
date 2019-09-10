#include <bytestream.h>
#include "cs2.h"

void Cs2EncReadConfig6(unsigned char *CanData, unsigned long LocId,
                       int CvIndex)
{
   SetLongToByteArray(CanData, LocId);
   SetIntToByteArray(&(CanData[4]), CvIndex);
}

void Cs2EncReadConfig7(unsigned char *CanData, unsigned long LocId,
                       int CvIndex, int Param)
{
   SetLongToByteArray(CanData, LocId);
   SetIntToByteArray(&(CanData[4]), CvIndex);
}
