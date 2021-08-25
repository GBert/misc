#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

#define CFG_HDR_ZLIB_RESERVED 0x7B

void MrCs2EncCfgdatStream6(MrCs2CanDataType *CanMsg, unsigned long Length,
                           int Crc)
{
   SetLongToByteArray(CanMsg->Data, Length);
   SetIntToByteArray(&(CanMsg->Data[4]), Crc);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_CFGDAT_STREAM);
   MrCs2SetDlc(CanMsg, 6);
}

void MrCs2EncCfgdatStream7(MrCs2CanDataType *CanMsg, unsigned long Length,
                           int Crc)
{
   SetLongToByteArray(CanMsg->Data, Length);
   SetIntToByteArray(&(CanMsg->Data[4]), Crc);
   CanMsg->Data[6] = CFG_HDR_ZLIB_RESERVED;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_CFGDAT_STREAM);
   MrCs2SetDlc(CanMsg, 7);
}

void MrCs2EncCfgdatStream8(MrCs2CanDataType *CanMsg, char *Bytes)
{  int i;

   for (i = 0; i < 8; i++)
      CanMsg->Data[i] = (unsigned char)(Bytes[i]);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_CFGDAT_STREAM);
   MrCs2SetDlc(CanMsg, 8);
}
