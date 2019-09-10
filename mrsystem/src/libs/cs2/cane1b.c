#include <bytestream.h>
#include "cs2.h"

void Cs2EncCanBootldr(unsigned char *CanData, char *Bytes)
{  int i;

   for (i = 0; i < 8; i++)
      CanData[i] = (unsigned char)(Bytes[i]);
}
