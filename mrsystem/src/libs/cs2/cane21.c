#include <bytestream.h>
#include "cs2.h"

#define CFG_HDR_ZLIB_RESERVED 0x7B

void Cs2EncCfgdatStream6(unsigned char *CanData, unsigned long Length,
                         int Crc)
{
   SetLongToByteArray(CanData, Length);
   SetIntToByteArray(&(CanData[4]), Crc);
}

void Cs2EncCfgdatStream7(unsigned char *CanData, unsigned long Length,
                         int Crc)
{
   SetLongToByteArray(CanData, Length);
   SetIntToByteArray(&(CanData[4]), Crc);
   CanData[6] = CFG_HDR_ZLIB_RESERVED;
}

void Cs2EncCfgdatStream8(unsigned char *CanData, char *Bytes)
{  int i;

   for (i = 0; i < 8; i++)
      CanData[i] = (unsigned char)(Bytes[i]);
}
