#include <string.h>
#include <bytestream.h>
#include "cs2.h"

void Cs2EncDebugMessage(unsigned char *CanData, char *Message)
{  int i;

   for (i = 0; i < 8; i++)
      CanData[i] = (unsigned char)0;
   for (i = 0; i < strlen(Message); i++)
      CanData[i] = (unsigned char)(Message[i]);
}
