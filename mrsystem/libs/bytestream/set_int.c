#include <bitmask.h>
#include "bytestream.h"

void SetIntToByteArray(char *Data, unsigned int Value)
{
   Data[0] = (Value >> SHIFT_1_BYTE) & MASK_LOW_BYTE;
   Data[1] = (Value >> SHIFT_0_BYTE) & MASK_LOW_BYTE;
}
