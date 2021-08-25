#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2EncSysMfxFastRead(MrCs2CanDataType *CanMsg, unsigned long Uid,
                            int MfxSid)
{
   SetLongToByteArray(CanMsg->Data, Uid);
   CanMsg->Data[5] = (unsigned char)MfxSid;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_SYSTEM);
   MrCs2SetSystemSubcmd(CanMsg, MR_CS2_SUBCMD_SYSTEM_MFX_FAST_READ);
   MrCs2SetDlc(CanMsg, 6);
}
