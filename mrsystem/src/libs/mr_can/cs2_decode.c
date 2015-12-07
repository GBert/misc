#include <string.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <bitmask.h>
#include "mr_can.h"

void MrCs2Decode(MrCs2CanDataType *CanMsg, struct can_frame *CanFrame)
{  unsigned CanHash, Response, Command, Prio, i;

   MrCs2SetId(CanMsg, CanFrame->can_id);
   MrCs2SetDlc(CanMsg, CanFrame->can_dlc);
   for (i = 0; i < 8; i++)
      CanMsg->Data[i] = CanFrame->data[i];
   MrCs2DecodeId(MrCs2GetId(CanMsg), &CanHash, &Response, &Command, &Prio);
   MrCs2SetHash(CanMsg, CanHash & ~MR_CS2_MASK_HASH_MAGIC);
   MrCs2SetResponse(CanMsg, Response);
   MrCs2SetCommand(CanMsg, Command);
   MrCs2SetPrio(CanMsg, Prio);
   MrCs2SetIsCs2(CanMsg, MrCs2IsCs2Msg(CanHash, MrCs2GetCommand(CanMsg)));
}
