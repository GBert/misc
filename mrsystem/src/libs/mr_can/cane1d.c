#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2EncStatus5(MrCs2CanDataType *CanMsg, unsigned long Uid, int Index)
{
   SetLongToByteArray(CanMsg->Data, Uid);
   CanMsg->Data[4] = (unsigned char)Index;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_STATUS);
   MrCs2SetDlc(CanMsg, 5);
}

void MrCs2EncStatus6(MrCs2CanDataType *CanMsg, unsigned long Uid, int Index,
                     int NumPackets)
{
   SetLongToByteArray(CanMsg->Data, Uid);
   CanMsg->Data[4] = (unsigned char)Index;
   CanMsg->Data[5] = (unsigned char)NumPackets;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_STATUS);
   MrCs2SetDlc(CanMsg, 6);
}

void MrCs2EncStatus8(MrCs2CanDataType *CanMsg, char *Bytes)
{  int i;

   for (i = 0; i < 8; i++)
      CanMsg->Data[i] = (unsigned char)(Bytes[i]);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_STATUS);
   MrCs2SetDlc(CanMsg, 8);
}
