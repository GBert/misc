#include <string.h>
#include <strings.h>
#include "mr_can.h"

void MrEthCs2Encode(char *UdpFrame, MrCs2CanDataType *CanMsg)
{  unsigned CanHash;

   CanHash = (MrCs2GetHash(CanMsg) & ~MR_CS2_MASK_HASH_MAGIC) | MR_CS2_HASH_MAGIC;
   MrCs2SetId(CanMsg,
              MrCs2EncodeId(CanHash, MrCs2GetResponse(CanMsg),
                            MrCs2GetCommand(CanMsg), MrCs2GetPrio(CanMsg)));
   SetLongToByteArray((unsigned char *)UdpFrame, MrCs2GetId(CanMsg));
   UdpFrame[4] = MrCs2GetDlc(CanMsg);
   memcpy(&UdpFrame[5], CanMsg->Data, MR_CS2_NUM_CAN_BYTES);
}
