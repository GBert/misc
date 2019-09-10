#include <bytestream.h>
#include "cs2.h"

void Cs2EncSysEmergencyHalt(unsigned char *CanData, unsigned long Uid)
{
   SetLongToByteArray(CanData, Uid);
}
