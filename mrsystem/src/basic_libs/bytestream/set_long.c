#include <bitmask.h>
#include "bytestream.h"

void SetLongToByteArray(char *Data, unsigned long Value)
{
   Data[0] = (Value >> SHIFT_3_BYTE) & MASK_LOW_BYTE;
   Data[1] = (Value >> SHIFT_2_BYTE) & MASK_LOW_BYTE;
   Data[2] = (Value >> SHIFT_1_BYTE) & MASK_LOW_BYTE;
   Data[3] = (Value >> SHIFT_0_BYTE) & MASK_LOW_BYTE;
}
