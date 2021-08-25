#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2EncDirection4(MrCs2CanDataType *CanMsg, unsigned long LocId)
{
   SetLongToByteArray(CanMsg->Data, LocId);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_DIRECTION);
   MrCs2SetDlc(CanMsg, 4);
}

void MrCs2EncDirection5(MrCs2CanDataType *CanMsg, unsigned long LocId,
                        int Direction)
{
   SetLongToByteArray(CanMsg->Data, LocId);
   CanMsg->Data[4] = (unsigned char)Direction;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_DIRECTION);
   MrCs2SetDlc(CanMsg, 5);
}
