#include "m_cs2ms2.h"

void MrCs2DecodeId(unsigned long CanId, unsigned *Hash, unsigned *Response,
                   unsigned *Command, unsigned *Prio)
{
   *Hash     = (CanId & MR_CS2_MASK_HASH)    >> MR_CS2_SHIFT_HASH;
   *Response = (CanId & MR_CS2_MASK_RESP)    >> MR_CS2_SHIFT_RESP;
   *Command  = (CanId & MR_CS2_MASK_COMMAND) >> MR_CS2_SHIFT_COMMAND;
   *Prio     = (CanId & MR_CS2_MASK_PRIO)    >> MR_CS2_SHIFT_PRIO;
}
