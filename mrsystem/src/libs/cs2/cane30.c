#include <bytestream.h>
#include "cs2.h"

void Cs2EncAutomatic6(unsigned char *CanData, int DeviceId, int Func,
                      int Status, int Param)
{
   SetIntToByteArray(CanData, DeviceId);
   SetIntToByteArray(&(CanData[2]), Func);
   CanData[4] = (unsigned char)Status;
   CanData[5] = (unsigned char)Param;
}

void Cs2EncAutomatic8(unsigned char *CanData, int DeviceId, int Func,
                      unsigned long LocId)
{
   SetIntToByteArray(CanData, DeviceId);
   SetIntToByteArray(&(CanData[2]), Func);
   SetLongToByteArray(&(CanData[4]), LocId);
}
