#include <bytestream.h>
#include "cs2.h"

void Cs2DecConfigQuery(unsigned char *CanData, char *FileName)
{  int i;

   for (i = 0; i < 8; i++)
      FileName[i] = (char)(CanData[i]);
   FileName[8] = '\0';
}
