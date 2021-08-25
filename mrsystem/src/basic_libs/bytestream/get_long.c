#include <bitmask.h>
#include "bytestream.h"

unsigned long GetLongFromByteArray(char *Data)
{
   return ((unsigned long)Data[0] << SHIFT_3_BYTE) |
          ((unsigned long)Data[1] << SHIFT_2_BYTE) |
          ((unsigned long)Data[2] << SHIFT_1_BYTE) |
          ((unsigned long)Data[3] << SHIFT_0_BYTE);
}
