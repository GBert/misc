#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2EncBind(MrCs2CanDataType *CanMsg, unsigned long Uid,
                  unsigned int Sid)
{
   SetLongToByteArray(CanMsg->Data, Uid);
   SetIntToByteArray(&(CanMsg->Data[4]), Sid);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_BIND);
   MrCs2SetDlc(CanMsg, 6);
}
