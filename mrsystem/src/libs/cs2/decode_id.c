#include "cs2.h"

void Cs2DecodeId(unsigned long CanId, unsigned *Hash, unsigned *Response,
                 unsigned *Command, unsigned *Prio)
{
   *Hash     = (CanId & CS2_MASK_HASH)    >> CS2_SHIFT_HASH;
   *Response = (CanId & CS2_MASK_RESP)    >> CS2_SHIFT_RESP;
   *Command  = (CanId & CS2_MASK_COMMAND) >> CS2_SHIFT_COMMAND;
   *Prio     = (CanId & CS2_MASK_PRIO)    >> CS2_SHIFT_PRIO;
}
