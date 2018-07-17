#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2EncCanBootldr(MrCs2CanDataType *CanMsg, char *Bytes)
{  int i;

   for (i = 0; i < 8; i++)
      CanMsg->Data[i] = (unsigned char)(Bytes[i]);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_BOOTLDR_CAN);
   MrCs2SetDlc(CanMsg, 8);
}
