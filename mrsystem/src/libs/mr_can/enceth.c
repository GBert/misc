#include <string.h>
#include <strings.h>
#include "mr_can.h"

void MrEthCs2Encode(char *UdpFrame, MrCs2CanDataType *CanMsg)
{  unsigned CanHash;

   CanHash = (MrCs2GetHash(CanMsg) & ~MR_CS2_MASK_HASH_MAGIC) | MR_CS2_HASH_MAGIC;
   MrCs2SetId(CanMsg,
              MrCs2EncodeId(CanHash, MrCs2GetResponse(CanMsg),
                            MrCs2GetCommand(CanMsg), MrCs2GetPrio(CanMsg)));
   bzero(UdpFrame, MR_CS2_UDP_LENGTH);
   UdpFrame[0] = (MrCs2GetId(CanMsg) >> 24) & 0x000000FF;
   UdpFrame[1] = (MrCs2GetId(CanMsg) >> 16) & 0x000000FF;
   UdpFrame[2] = (MrCs2GetId(CanMsg) >>  8) & 0x000000FF;
   UdpFrame[3] = (MrCs2GetId(CanMsg) >>  0) & 0x000000FF;
   UdpFrame[4] = MrCs2GetDlc(CanMsg);
   memcpy(&UdpFrame[5], &CanMsg->Data, MrCs2GetDlc(CanMsg));
}
