#include "cs2.h"

unsigned long Cs2EncodeId(unsigned short Hash, unsigned Response,
                          unsigned Command, unsigned Prio)
{
   return(((unsigned long)Hash     << CS2_SHIFT_HASH) |
          ((unsigned long)Response << CS2_SHIFT_RESP) |
          ((unsigned long)Command  << CS2_SHIFT_COMMAND) |
          ((unsigned long)Prio     << CS2_SHIFT_PRIO));
}
