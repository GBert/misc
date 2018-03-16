#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2EncSysStatus6(MrCs2CanDataType *CanMsg, unsigned long Uid,
                        int Channel)
{
   SetLongToByteArray((char *)CanMsg->Data, Uid);
   CanMsg->Data[5] = Channel;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_SYSTEM);
   MrCs2SetSystemSubcmd(CanMsg, MR_CS2_SUBCMD_SYSTEM_STATUS);
   MrCs2SetDlc(CanMsg, 6);
}

void MrCs2EncSysStatus7(MrCs2CanDataType *CanMsg, unsigned long Uid,
                        int Channel, int Set)
{
   SetLongToByteArray((char *)CanMsg->Data, Uid);
   CanMsg->Data[5] = Channel;
   CanMsg->Data[6] = Set;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_SYSTEM);
   MrCs2SetSystemSubcmd(CanMsg, MR_CS2_SUBCMD_SYSTEM_STATUS);
   MrCs2SetDlc(CanMsg, 7);
}

void MrCs2EncSysStatus8(MrCs2CanDataType *CanMsg, unsigned long Uid,
                        int Channel, int Value)
{
   SetLongToByteArray((char *)CanMsg->Data, Uid);
   CanMsg->Data[5] = Channel;
   SetIntToByteArray((char *)&(CanMsg->Data[6]), Value);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_SYSTEM);
   MrCs2SetSystemSubcmd(CanMsg, MR_CS2_SUBCMD_SYSTEM_STATUS);
   MrCs2SetDlc(CanMsg, 8);
}
