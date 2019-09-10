#include <bytestream.h>
#include "cs2.h"

void Cs2EncWriteConfig(unsigned char *CanData, unsigned long LocId,
                       int CvIndex, int Value, int CtrlRslt)
{
   SetLongToByteArray(CanData, LocId);
   SetIntToByteArray(&(CanData[4]), CvIndex);
   CanData[6] = (unsigned char)Value;
   CanData[7] = (unsigned char)CtrlRslt;
}
