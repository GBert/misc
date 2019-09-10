#include <string.h>
#include <bytestream.h>
#include "cs2.h"

void Cs2EncConfigQuery0(unsigned char *CanData)
{
}

void Cs2EncConfigQuery(unsigned char *CanData, char *FileName)
{  int i;

   for (i = 0; i < 8; i++)
      CanData[i] = (unsigned char)0;
   for (i = 0; i < strlen(FileName); i++)
      CanData[i] = (unsigned char)(FileName[i]);
}
