#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2EncMfxVerify6(MrCs2CanDataType *CanMsg, unsigned long Uid,
                        unsigned int Sid)
{
   SetLongToByteArray((char *)CanMsg->Data, Uid);
   SetIntToByteArray((char *)&(CanMsg->Data[4]), Sid);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_MFX_VERIFY);
   MrCs2SetDlc(CanMsg, 6);
}

void MrCs2EncMfxVerify7(MrCs2CanDataType *CanMsg, unsigned long Uid,
                        unsigned int Sid, int Ask)
{
   SetLongToByteArray((char *)CanMsg->Data, Uid);
   SetIntToByteArray((char *)&(CanMsg->Data[4]), Sid);
   CanMsg->Data[6] = Ask;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_MFX_VERIFY);
   MrCs2SetDlc(CanMsg, 7);
}
