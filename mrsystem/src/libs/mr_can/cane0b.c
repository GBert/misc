#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2EncAccSwitch6(MrCs2CanDataType *CanMsg, unsigned long LocId,
                        int Position, int Current)
{
   SetLongToByteArray(CanMsg->Data, LocId);
   CanMsg->Data[4] = (unsigned char)Position;
   CanMsg->Data[5] = (unsigned char)Current;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_ACC_SWITCH);
   MrCs2SetDlc(CanMsg, 6);
}

void MrCs2EncAccSwitch8(MrCs2CanDataType *CanMsg, unsigned long LocId,
                        int Position, int Current, int TimeFkt)
{
   SetLongToByteArray(CanMsg->Data, LocId);
   CanMsg->Data[4] = (unsigned char)Position;
   CanMsg->Data[5] = (unsigned char)Current;
   SetIntToByteArray(&(CanMsg->Data[6]), TimeFkt);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_ACC_SWITCH);
   MrCs2SetDlc(CanMsg, 8);
}
