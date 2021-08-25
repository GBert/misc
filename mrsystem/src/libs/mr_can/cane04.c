#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2EncSpeed4(MrCs2CanDataType *CanMsg, unsigned long LocId)
{
   SetLongToByteArray(CanMsg->Data, LocId);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_SPEED);
   MrCs2SetDlc(CanMsg, 4);
}

void MrCs2EncSpeed6(MrCs2CanDataType *CanMsg, unsigned long LocId, int Speed)
{
   SetLongToByteArray(CanMsg->Data, LocId);
   SetIntToByteArray(&(CanMsg->Data[4]), Speed);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_SPEED);
   MrCs2SetDlc(CanMsg, 6);
}
