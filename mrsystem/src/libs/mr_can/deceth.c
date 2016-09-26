#include <string.h>
#include <strings.h>
#include "mr_can.h"

void MrEthCs2Decode(MrCs2CanDataType *CanMsg, char *UdpFrame)
{  unsigned CanHash, Response, Command, Prio, i;

   MrCs2SetId(CanMsg,
              ((unsigned long)UdpFrame[0] << 24) |
              ((unsigned long)UdpFrame[1] << 16) |
              ((unsigned long)UdpFrame[2] <<  8) |
              ((unsigned long)UdpFrame[3] <<  0));
   MrCs2SetDlc(CanMsg, UdpFrame[4]);
   for (i = 0; i < 8; i++)
      CanMsg->Data[i] = UdpFrame[5 + i];
   MrCs2DecodeId(MrCs2GetId(CanMsg), &CanHash, &Response, &Command, &Prio);
   MrCs2SetHash(CanMsg, CanHash & ~MR_CS2_MASK_HASH_MAGIC);
   MrCs2SetResponse(CanMsg, Response);
   MrCs2SetCommand(CanMsg, Command);
   MrCs2SetPrio(CanMsg, Prio);
   MrCs2SetIsCs2(CanMsg, MrCs2IsCs2Msg(CanHash, MrCs2GetCommand(CanMsg)));
}
