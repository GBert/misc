#include <bytestream.h>
#include "cs2.h"

void Cs2EncSysSystemId5(unsigned char *CanData, unsigned long Uid)
{
   SetLongToByteArray(CanData, Uid);
}

void Cs2EncSysSystemId7(unsigned char *CanData, unsigned long Uid,
                        int Id)
{
   SetLongToByteArray(CanData, Uid);
   SetIntToByteArray(&(CanData[5]), Id);
}
