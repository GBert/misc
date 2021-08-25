#include <bytestream.h>
#include "cs2.h"

void Cs2DecDiscovery0(unsigned char *CanData)
{
}

void Cs2DecDiscovery1(unsigned char *CanData, int *Proto)
{
   *Proto = (int)(CanData[0]);
}

void Cs2DecDiscovery5(unsigned char *CanData, unsigned long *Uid,
                      int *Range)
{
   *Uid = GetLongFromByteArray(CanData);
   *Range = (int)(CanData[4]);
}

void Cs2DecDiscovery6(unsigned char *CanData, unsigned long *Uid,
                      int *Range, int *Ask)
{
   *Uid = GetLongFromByteArray(CanData);
   *Range = (int)(CanData[4]);
   *Ask = (int)(CanData[5]);
}
