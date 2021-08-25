#include <bytestream.h>
#include "cs2.h"

void Cs2EncBind(unsigned char *CanData, unsigned long Uid,
                unsigned int Sid)
{
   SetLongToByteArray(CanData, Uid);
   SetIntToByteArray(&(CanData[4]), Sid);
}
