#include <bytestream.h>
#include "cs2.h"

void Cs2DecStatus5(unsigned char *CanData, unsigned long *Uid, int *Index)
{
   *Uid = GetLongFromByteArray(CanData);
   *Index = (int)(CanData[4]);
}

void Cs2DecStatus6(unsigned char *CanData, unsigned long *Uid, int *Index,
                   int *NumPackets)
{
   *Uid = GetLongFromByteArray(CanData);
   *Index = (int)(CanData[4]);
   *NumPackets = (int)(CanData[5]);
}

void Cs2DecStatus8(unsigned char *CanData, char *Bytes)
{  int i;

   for (i = 0; i < 8; i++)
      Bytes[i] = (char)(CanData[i]);
}
