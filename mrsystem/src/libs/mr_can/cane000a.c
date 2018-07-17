#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2EncSysOverload(MrCs2CanDataType *CanMsg, unsigned long Uid,
                         int Channel)
{
   SetLongToByteArray(CanMsg->Data, Uid);
   CanMsg->Data[5] = (unsigned char)Channel;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_SYSTEM);
   MrCs2SetSystemSubcmd(CanMsg, MR_CS2_SUBCMD_SYSTEM_OVERLOAD);
   MrCs2SetDlc(CanMsg, 6);
}
