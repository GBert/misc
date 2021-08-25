#include <bytestream.h>
#include "cs2.h"

void Cs2DecS88Poll5(unsigned char *CanData, unsigned long *Uid,
                    int *NumModuls)
{
   *Uid = GetLongFromByteArray(CanData);
   *NumModuls = (int)(CanData[4]);
}

void Cs2DecS88Poll7(unsigned char *CanData, unsigned long *Uid,
                    int *Modul, int *State)
{
   *Uid = GetLongFromByteArray(CanData);
   *Modul = (int)(CanData[4]);
   *State = GetIntFromByteArray(&(CanData[5]));
}
