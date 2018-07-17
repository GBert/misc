#include "m_cs2ms2.h"

unsigned long MrCs2EncodeId(unsigned short Hash, unsigned Response,
                            unsigned Command, unsigned Prio)
{
   return(((unsigned long)Hash     << MR_CS2_SHIFT_HASH) |
          ((unsigned long)Response << MR_CS2_SHIFT_RESP) |
          ((unsigned long)Command  << MR_CS2_SHIFT_COMMAND) |
          ((unsigned long)Prio     << MR_CS2_SHIFT_PRIO));
}
