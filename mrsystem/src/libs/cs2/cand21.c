#include <bytestream.h>
#include "cs2.h"

void Cs2DecCfgdatStream6(unsigned char *CanData, unsigned long *Length,
                         int *Crc)
{
   *Length = GetLongFromByteArray(CanData);
   *Crc = GetIntFromByteArray(&(CanData[4]));
}

void Cs2DecCfgdatStream7(unsigned char *CanData, unsigned long *Length,
                         int *Crc)
{
   *Length = GetLongFromByteArray(CanData);
   *Crc = GetIntFromByteArray(&(CanData[4]));
}

void Cs2DecCfgdatStream8(unsigned char *CanData, char *Bytes)
{  int i;

   for (i = 0; i < 8; i++)
      Bytes[i] = (char)(CanData[i]);
}
