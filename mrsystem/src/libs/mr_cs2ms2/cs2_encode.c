#include <string.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <bitmask.h>
#include <cs2.h>
#include "mr_cs2ms2.h"

void MrCs2Encode(MrCs2CanDataType *CanMsg, struct can_frame *CanFrame)
{  unsigned CanHash;

   CanHash = (MrCs2GetHash(CanMsg) & ~CS2_MASK_HASH_MAGIC) | CS2_HASH_MAGIC;
   MrCs2SetId(CanMsg,
              Cs2EncodeId(CanHash, MrCs2GetResponse(CanMsg),
                          MrCs2GetCommand(CanMsg), MrCs2GetPrio(CanMsg)));
   CanFrame->can_id = MrCs2GetId(CanMsg);
   CanFrame->can_id &= CAN_EFF_MASK;
   CanFrame->can_id |= CAN_EFF_FLAG;
   CanFrame->can_dlc = MrCs2GetDlc(CanMsg);
   memcpy(CanFrame->data, CanMsg->Data, MR_CS2_NUM_CAN_BYTES);
}
