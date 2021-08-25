#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2EncFunction5(MrCs2CanDataType *CanMsg, unsigned long LocId,
                       int Function)
{
   SetLongToByteArray(CanMsg->Data, LocId);
   CanMsg->Data[4] = (unsigned char)Function;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_FUNCTION);
   MrCs2SetDlc(CanMsg, 5);
}

void MrCs2EncFunction6(MrCs2CanDataType *CanMsg, unsigned long LocId,
                       int Function, int Value)
{
   SetLongToByteArray(CanMsg->Data, LocId);
   CanMsg->Data[4] = (unsigned char)Function;
   CanMsg->Data[5] = (unsigned char)Value;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_FUNCTION);
   MrCs2SetDlc(CanMsg, 6);
}

void MrCs2EncFunction8(MrCs2CanDataType *CanMsg, unsigned long LocId,
                       int Function, int Value, int FktVal)
{
   SetLongToByteArray(CanMsg->Data, LocId);
   CanMsg->Data[4] = (unsigned char)Function;
   CanMsg->Data[5] = (unsigned char)Value;
   SetIntToByteArray(&(CanMsg->Data[6]), FktVal);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_FUNCTION);
   MrCs2SetDlc(CanMsg, 8);
}
