#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2EncSysDecoderTimeing(MrCs2CanDataType *CanMsg, unsigned long Uid,
                               int Time)
{
   SetLongToByteArray(CanMsg->Data, Uid);
   SetIntToByteArray(&(CanMsg->Data[5]), Time);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_SYSTEM);
   MrCs2SetSystemSubcmd(CanMsg, MR_CS2_SUBCMD_SYSTEM_DECODER_TIMEING);
   MrCs2SetDlc(CanMsg, 7);
}
