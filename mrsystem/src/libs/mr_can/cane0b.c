#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2EncAccSwitch6(MrCs2CanDataType *CanMsg, unsigned long LocId,
                        int Position, int Current)
{
   SetLongToByteArray((char *)CanMsg->Data, LocId);
   CanMsg->Data[4] = Position;
   CanMsg->Data[5] = Current;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_ACC_SWITCH);
   MrCs2SetDlc(CanMsg, 6);
}

void MrCs2EncAccSwitch8(MrCs2CanDataType *CanMsg, unsigned long LocId,
                        int Position, int Current, int TimeFkt)
{
   SetLongToByteArray((char *)CanMsg->Data, LocId);
   CanMsg->Data[4] = Position;
   CanMsg->Data[5] = Current;
   SetIntToByteArray((char *)&(CanMsg->Data[6]), TimeFkt);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_ACC_SWITCH);
   MrCs2SetDlc(CanMsg, 8);
}
