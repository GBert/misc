#include <bytestream.h>
#include "cs2.h"

void Cs2EncMfxVerify6(unsigned char *CanData, unsigned long Uid,
                      unsigned int Sid)
{
   SetLongToByteArray(CanData, Uid);
   SetIntToByteArray(&(CanData[4]), Sid);
}

void Cs2EncMfxVerify7(unsigned char *CanData, unsigned long Uid,
                      unsigned int Sid, int Ask)
{
   SetLongToByteArray(CanData, Uid);
   SetIntToByteArray(&(CanData[4]), Sid);
   CanData[6] = (unsigned char)Ask;
}
