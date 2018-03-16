#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2EncPing0(MrCs2CanDataType *CanMsg)
{
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_PING);
   MrCs2SetDlc(CanMsg, 0);
}

void MrCs2EncPing8(MrCs2CanDataType *CanMsg, unsigned long Uid,
                   int SwVersion, int DeviceId)
{
   SetLongToByteArray((char *)CanMsg->Data, Uid);
   SetIntToByteArray((char *)&(CanMsg->Data[4]), SwVersion);
   SetIntToByteArray((char *)&(CanMsg->Data[6]), DeviceId);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_PING);
   MrCs2SetDlc(CanMsg, 8);
}
