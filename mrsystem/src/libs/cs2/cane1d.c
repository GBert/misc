#include <bytestream.h>
#include "cs2.h"

void Cs2EncStatus5(unsigned char *CanData, unsigned long Uid, int Index)
{
   SetLongToByteArray(CanData, Uid);
   CanData[4] = (unsigned char)Index;
}

void Cs2EncStatus6(unsigned char *CanData, unsigned long Uid, int Index,
                   int NumPackets)
{
   SetLongToByteArray(CanData, Uid);
   CanData[4] = (unsigned char)Index;
   CanData[5] = (unsigned char)NumPackets;
}

void Cs2EncStatus8(unsigned char *CanData, char *Bytes)
{  int i;

   for (i = 0; i < 8; i++)
      CanData[i] = (unsigned char)(Bytes[i]);
}
