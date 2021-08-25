#include <string.h>
#include <strings.h>
#include <bytestream.h>
#include <cs2.h>
#include "mr_cs2ms2.h"

void MrEthCs2Decode(MrCs2CanDataType *CanMsg, char *UdpFrame)
{  unsigned CanHash, Response, Command, Prio;

   MrCs2SetId(CanMsg, GetLongFromByteArray((unsigned char *)UdpFrame));
   MrCs2SetDlc(CanMsg, UdpFrame[4]);
   memcpy(CanMsg->Data, &UdpFrame[5], MR_CS2_NUM_CAN_BYTES);
   Cs2DecodeId(MrCs2GetId(CanMsg), &CanHash, &Response, &Command, &Prio);
   MrCs2SetHash(CanMsg, CanHash & ~CS2_MASK_HASH_MAGIC);
   MrCs2SetResponse(CanMsg, Response);
   MrCs2SetCommand(CanMsg, Command);
   MrCs2SetPrio(CanMsg, Prio);
   MrCs2SetIsCs2(CanMsg, Cs2IsCs2Msg(CanHash, MrCs2GetCommand(CanMsg)));
}
