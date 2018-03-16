#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2EncS88Poll5(MrCs2CanDataType *CanMsg, unsigned long Uid,
                      int NumModuls)
{
   SetLongToByteArray((char *)CanMsg->Data, Uid);
   CanMsg->Data[4] = NumModuls;
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_S88_POLL);
   MrCs2SetDlc(CanMsg, 5);
}

void MrCs2EncS88Poll7(MrCs2CanDataType *CanMsg, unsigned long Uid,
                      int Modul, int State)
{
   SetLongToByteArray((char *)CanMsg->Data, Uid);
   CanMsg->Data[4] = Modul;
   SetIntToByteArray((char *)&(CanMsg->Data[5]), State);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_S88_POLL);
   MrCs2SetDlc(CanMsg, 7);
}
