#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2EncSysSystemReset(MrCs2CanDataType *CanMsg, unsigned long Uid,
                            int ResetTarget)
{
   SetLongToByteArray(CanMsg->Data, Uid);
   CanMsg->Data[5] = (unsigned char)ResetTarget;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_SYSTEM);
   MrCs2SetSystemSubcmd(CanMsg, MR_CS2_SUBCMD_SYSTEM_SYSTEM_RESET);
   MrCs2SetDlc(CanMsg, 6);
}
