#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2EncWriteConfig(MrCs2CanDataType *CanMsg, unsigned long LocId,
                         int CvIndex, int Value, int CtrlRslt)
{
   SetLongToByteArray((char *)CanMsg->Data, LocId);
   SetIntToByteArray((char *)&(CanMsg->Data[4]), CvIndex);
   CanMsg->Data[6] = Value;
   CanMsg->Data[7] = CtrlRslt;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_WRITE_CONFIG);
   MrCs2SetDlc(CanMsg, 8);
}
