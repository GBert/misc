#include <bytestream.h>
#include "cs2.h"

void Cs2DecDebugMessage(unsigned char *CanData, char *Message)
{  int i;

   for (i = 0; i < 8; i++)
      Message[i] = (char)(CanData[i]);
   Message[8] = '\0';
}
