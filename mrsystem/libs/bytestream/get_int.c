#include <bitmask.h>
#include "bytestream.h"

unsigned int GetIntFromByteArray(char *Data)
{
   return ((unsigned int)Data[0] << SHIFT_1_BYTE) |
          ((unsigned int)Data[1] << SHIFT_0_BYTE);
}
