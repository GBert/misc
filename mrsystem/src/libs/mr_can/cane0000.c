#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2EncSysStop(MrCs2CanDataType *CanMsg, unsigned long Uid)
{
   SetLongToByteArray(CanMsg->Data, Uid);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_SYSTEM);
   MrCs2SetSystemSubcmd(CanMsg, MR_CS2_SUBCMD_SYSTEM_STOP);
   MrCs2SetDlc(CanMsg, 5);
}
