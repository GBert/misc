#include <bytestream.h>
#include "cs2.h"

void Cs2DecSysStatus6(unsigned char *CanData, unsigned long *Uid,
                      int *Channel)
{
   *Uid = GetLongFromByteArray(CanData);
   *Channel = (int)(CanData[5]);
}

void Cs2DecSysStatus7(unsigned char *CanData, unsigned long *Uid,
                      int *Channel, int *Set)
{
   *Uid = GetLongFromByteArray(CanData);
   *Channel = (int)(CanData[5]);
   *Set = (int)(CanData[6]);
}

void Cs2DecSysStatus8(unsigned char *CanData, unsigned long *Uid,
                      int *Channel, int *Value)
{
   *Uid = GetLongFromByteArray(CanData);
   *Channel = (int)(CanData[5]);
   *Value = GetIntFromByteArray(&(CanData[6]));
}
