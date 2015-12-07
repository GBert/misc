#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2EncAutomatic6(MrCs2CanDataType *CanMsg, int DeviceId, int Func,
                        int Status, int Param)
{
   SetIntToByteArray((char *)CanMsg->Data, DeviceId);
   SetIntToByteArray((char *)&(CanMsg->Data[2]), Func);
   CanMsg->Data[4] = Status;
   CanMsg->Data[5] = Param;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_AUTOMATIC);
   MrCs2SetDlc(CanMsg, 6);
}

void MrCs2EncAutomatic8(MrCs2CanDataType *CanMsg, int DeviceId, int Func,
                        unsigned long LocId)
{
   SetIntToByteArray((char *)CanMsg->Data, DeviceId);
   SetIntToByteArray((char *)&(CanMsg->Data[2]), Func);
   SetLongToByteArray((char *)&(CanMsg->Data[4]), LocId);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_AUTOMATIC);
   MrCs2SetDlc(CanMsg, 8);
}
