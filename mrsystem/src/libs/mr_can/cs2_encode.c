#include <string.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <bitmask.h>
#include "mr_can.h"

void MrCs2Encode(MrCs2CanDataType *CanMsg, CanFrameStruct *CanFrame)
{  unsigned CanHash, i;

   CanHash = (MrCs2GetHash(CanMsg) & ~MR_CS2_MASK_HASH_MAGIC) | MR_CS2_HASH_MAGIC;
   MrCs2SetId(CanMsg,
              MrCs2EncodeId(CanHash, MrCs2GetResponse(CanMsg),
                            MrCs2GetCommand(CanMsg), MrCs2GetPrio(CanMsg)));
   CanFrame->CanId = MrCs2GetId(CanMsg);
   CanFrame->CanId &= CAN_EFF_MASK;
   CanFrame->CanId |= CAN_EFF_FLAG;
   CanFrame->CanDlc = MrCs2GetDlc(CanMsg);
   for (i = 0; i < 8; i++)
      CanFrame->CanData[i] = CanMsg->Data[i];
}
