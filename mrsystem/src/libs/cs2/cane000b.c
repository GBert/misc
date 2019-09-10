#include <bytestream.h>
#include "cs2.h"

void Cs2EncSysStatus6(unsigned char *CanData, unsigned long Uid,
                      int Channel)
{
   SetLongToByteArray(CanData, Uid);
   CanData[5] = (unsigned char)Channel;
}

void Cs2EncSysStatus7(unsigned char *CanData, unsigned long Uid,
                      int Channel, int Set)
{
   SetLongToByteArray(CanData, Uid);
   CanData[5] = (unsigned char)Channel;
   CanData[6] = (unsigned char)Set;
}

void Cs2EncSysStatus8(unsigned char *CanData, unsigned long Uid,
                      int Channel, int Value)
{
   SetLongToByteArray(CanData, Uid);
   CanData[5] = (unsigned char)Channel;
   SetIntToByteArray(&(CanData[6]), Value);
}
