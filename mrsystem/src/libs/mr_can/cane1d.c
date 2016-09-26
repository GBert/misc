#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2EncStatus5(MrCs2CanDataType *CanMsg, unsigned long Uid, int Index)
{
   SetLongToByteArray((char *)CanMsg->Data, Uid);
   CanMsg->Data[4] = Index;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_STATUS);
   MrCs2SetDlc(CanMsg, 5);
}

void MrCs2EncStatus6(MrCs2CanDataType *CanMsg, unsigned long Uid, int Index,
                     int NumPackets)
{
   SetLongToByteArray((char *)CanMsg->Data, Uid);
   CanMsg->Data[4] = Index;
   CanMsg->Data[5] = NumPackets;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_STATUS);
   MrCs2SetDlc(CanMsg, 6);
}
