#include <bytestream.h>
#include "cs2.h"

void Cs2EncS88Poll5(unsigned char *CanData, unsigned long Uid,
                    int NumModuls)
{
   SetLongToByteArray(CanData, Uid);
   CanData[4] = (unsigned char)NumModuls;
}

void Cs2EncS88Poll7(unsigned char *CanData, unsigned long Uid,
                    int Modul, int State)
{
   SetLongToByteArray(CanData, Uid);
   CanData[4] = (unsigned char)Modul;
   SetIntToByteArray(&(CanData[5]), State);
}
