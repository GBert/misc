#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2EncSysSystemId5(MrCs2CanDataType *CanMsg, unsigned long Uid)
{
   SetLongToByteArray(CanMsg->Data, Uid);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_SYSTEM);
   MrCs2SetSystemSubcmd(CanMsg, MR_CS2_SUBCMD_SYSTEM_SYSTEM_ID);
   MrCs2SetDlc(CanMsg, 5);
}

void MrCs2EncSysSystemId7(MrCs2CanDataType *CanMsg, unsigned long Uid,
                         int Id)
{
   SetLongToByteArray(CanMsg->Data, Uid);
   SetIntToByteArray(&(CanMsg->Data[5]), Id);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_SYSTEM);
   MrCs2SetSystemSubcmd(CanMsg, MR_CS2_SUBCMD_SYSTEM_SYSTEM_ID);
   MrCs2SetDlc(CanMsg, 7);
}
