#include <bytestream.h>
#include "cs2.h"

void Cs2DecCanBootldr(unsigned char *CanData, char *Bytes)
{  int i;

   for (i = 0; i < 8; i++)
      Bytes[i] = (char)(CanData[i]);
}
