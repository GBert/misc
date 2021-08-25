#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2EncSysResetMfxCounter(MrCs2CanDataType *CanMsg, unsigned long Uid,
                                int Counter)
{
   SetLongToByteArray(CanMsg->Data, Uid);
   SetIntToByteArray(&(CanMsg->Data[5]), Counter);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_SYSTEM);
   MrCs2SetSystemSubcmd(CanMsg, MR_CS2_SUBCMD_SYSTEM_RESET_MFX_COUNTER);
   MrCs2SetDlc(CanMsg, 7);
}
