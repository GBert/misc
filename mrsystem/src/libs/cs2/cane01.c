#include <bytestream.h>
#include "cs2.h"

void Cs2EncDiscovery0(unsigned char *CanData)
{
}

void Cs2EncDiscovery1(unsigned char *CanData, int Proto)
{
   CanData[0] = Proto;
}

void Cs2EncDiscovery5(unsigned char *CanData, unsigned long Uid,
                      int Range)
{
   SetLongToByteArray(CanData, Uid);
   CanData[4] = (unsigned char)Range;
}

void Cs2EncDiscovery6(unsigned char *CanData, unsigned long Uid,
                      int Range, int Ask)
{
   SetLongToByteArray(CanData, Uid);
   CanData[4] = (unsigned char)Range;
   CanData[5] = (unsigned char)Ask;
}
