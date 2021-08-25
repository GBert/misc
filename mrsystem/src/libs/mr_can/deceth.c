#include <string.h>
#include <strings.h>
#include "mr_can.h"

void MrEthCs2Decode(MrCs2CanDataType *CanMsg, char *UdpFrame)
{  unsigned CanHash, Response, Command, Prio;

   MrCs2SetId(CanMsg, GetLongFromByteArray((unsigned char *)UdpFrame));
   MrCs2SetDlc(CanMsg, UdpFrame[4]);
   memcpy(CanMsg->Data, &UdpFrame[5], MR_CS2_NUM_CAN_BYTES);
   MrCs2DecodeId(MrCs2GetId(CanMsg), &CanHash, &Response, &Command, &Prio);
   MrCs2SetHash(CanMsg, CanHash & ~MR_CS2_MASK_HASH_MAGIC);
   MrCs2SetResponse(CanMsg, Response);
   MrCs2SetCommand(CanMsg, Command);
   MrCs2SetPrio(CanMsg, Prio);
   MrCs2SetIsCs2(CanMsg, MrCs2IsCs2Msg(CanHash, MrCs2GetCommand(CanMsg)));
}
