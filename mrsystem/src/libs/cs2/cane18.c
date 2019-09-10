#include <bytestream.h>
#include "cs2.h"

void Cs2EncPing0(unsigned char *CanData)
{
}

void Cs2EncPing8(unsigned char *CanData, unsigned long Uid,
                 int SwVersion, int DeviceId)
{
   SetLongToByteArray(CanData, Uid);
   SetIntToByteArray(&(CanData[4]), SwVersion);
   SetIntToByteArray(&(CanData[6]), DeviceId);
}
