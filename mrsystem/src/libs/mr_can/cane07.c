#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2EncReadConfig6(MrCs2CanDataType *CanMsg, unsigned long LocId,
                         int CvIndex)
{
   SetLongToByteArray(CanMsg->Data, LocId);
   SetIntToByteArray(&(CanMsg->Data[4]), CvIndex);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_READ_CONFIG);
   MrCs2SetDlc(CanMsg, 6);
}

void MrCs2EncReadConfig7(MrCs2CanDataType *CanMsg, unsigned long LocId,
                         int CvIndex, int Param)
{
   SetLongToByteArray(CanMsg->Data, LocId);
   SetIntToByteArray(&(CanMsg->Data[4]), CvIndex);
   CanMsg->Data[6] = (unsigned char)Param;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_READ_CONFIG);
   MrCs2SetDlc(CanMsg, 7);
}
