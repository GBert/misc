#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2EncWriteConfig(MrCs2CanDataType *CanMsg, unsigned long LocId,
                         int CvIndex, int Value, int CtrlRslt)
{
   SetLongToByteArray(CanMsg->Data, LocId);
   SetIntToByteArray(&(CanMsg->Data[4]), CvIndex);
   CanMsg->Data[6] = (unsigned char)Value;
   CanMsg->Data[7] = (unsigned char)CtrlRslt;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_WRITE_CONFIG);
   MrCs2SetDlc(CanMsg, 8);
}
