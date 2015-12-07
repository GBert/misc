#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2EncFunction5(MrCs2CanDataType *CanMsg, unsigned long LocId,
                       int Function)
{
   SetLongToByteArray((char *)CanMsg->Data, LocId);
   CanMsg->Data[4] = Function;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_FUNCTION);
   MrCs2SetDlc(CanMsg, 5);
}

void MrCs2EncFunction6(MrCs2CanDataType *CanMsg, unsigned long LocId,
                       int Function, int Value)
{
   SetLongToByteArray((char *)CanMsg->Data, LocId);
   CanMsg->Data[4] = Function;
   CanMsg->Data[5] = Value;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_FUNCTION);
   MrCs2SetDlc(CanMsg, 6);
}

void MrCs2EncFunction8(MrCs2CanDataType *CanMsg, unsigned long LocId,
                       int Function, int Value, int FktVal)
{
   SetLongToByteArray((char *)CanMsg->Data, LocId);
   CanMsg->Data[4] = Function;
   CanMsg->Data[5] = Value;
   SetIntToByteArray((char *)&(CanMsg->Data[6]), FktVal);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_FUNCTION);
   MrCs2SetDlc(CanMsg, 8);
}
