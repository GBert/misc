#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2EncDiscovery0(MrCs2CanDataType *CanMsg)
{
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_DISCOVERY);
   MrCs2SetDlc(CanMsg, 0);
}

void MrCs2EncDiscovery1(MrCs2CanDataType *CanMsg, int Proto)
{
   CanMsg->Data[0] = Proto;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_DISCOVERY);
   MrCs2SetDlc(CanMsg, 1);
}

void MrCs2EncDiscovery5(MrCs2CanDataType *CanMsg, unsigned long Uid,
                        int Range)
{
   SetLongToByteArray(CanMsg->Data, Uid);
   CanMsg->Data[4] = (unsigned char)Range;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_DISCOVERY);
   MrCs2SetDlc(CanMsg, 5);
}

void MrCs2EncDiscovery6(MrCs2CanDataType *CanMsg, unsigned long Uid,
                        int Range, int Ask)
{
   SetLongToByteArray(CanMsg->Data, Uid);
   CanMsg->Data[4] = (unsigned char)Range;
   CanMsg->Data[5] = (unsigned char)Ask;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_DISCOVERY);
   MrCs2SetDlc(CanMsg, 6);
}
