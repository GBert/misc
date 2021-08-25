#include <bytestream.h>
#include "cs2.h"

void Cs2EncS88Event4(unsigned char *CanData, int Device, int Contact)
{
   SetIntToByteArray(CanData, Device);
   SetIntToByteArray(&(CanData[2]), Contact);
}

void Cs2EncS88Event5(unsigned char *CanData, int Device, int Contact,
                     int Param)
{
   SetIntToByteArray(CanData, Device);
   SetIntToByteArray(&(CanData[2]), Contact);
   CanData[4] = (unsigned char)Param;
}

void Cs2EncS88Event8(unsigned char *CanData, int Device, int Contact,
                     int StateOld, int StateNew, int Time)
{
   SetIntToByteArray(CanData, Device);
   SetIntToByteArray(&(CanData[2]), Contact);
   CanData[4] = (unsigned char)StateOld;
   CanData[5] = (unsigned char)StateNew;
   SetIntToByteArray(&(CanData[6]), Time);
}
