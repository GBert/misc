#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2EncS88Event4(MrCs2CanDataType *CanMsg, int Device, int Contact)
{
   SetIntToByteArray((char *)CanMsg->Data, Device);
   SetIntToByteArray((char *)&(CanMsg->Data[2]), Contact);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_S88_EVENT);
   MrCs2SetDlc(CanMsg, 4);
}

void MrCs2EncS88Event5(MrCs2CanDataType *CanMsg, int Device, int Contact,
                       int Param)
{
   SetIntToByteArray((char *)CanMsg->Data, Device);
   SetIntToByteArray((char *)&(CanMsg->Data[2]), Contact);
   CanMsg->Data[4] = Param;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_S88_EVENT);
   MrCs2SetDlc(CanMsg, 5);
}

void MrCs2EncS88Event8(MrCs2CanDataType *CanMsg, int Device, int Contact,
                       int StateOld, int StateNew, int Time)
{
   SetIntToByteArray((char *)CanMsg->Data, Device);
   SetIntToByteArray((char *)&(CanMsg->Data[2]), Contact);
   CanMsg->Data[4] = StateOld;
   CanMsg->Data[5] = StateNew;
   SetIntToByteArray((char *)&(CanMsg->Data[6]), Time);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_S88_EVENT);
   MrCs2SetDlc(CanMsg, 8);
}
