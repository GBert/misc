#include <linux/can.h>
#include <boolean.h>
#include "m_cs2ms2.h"

BOOL MrCs2IsCs2Msg(unsigned Hash, unsigned Command)
{
   return(((Hash & MR_CS2_MASK_HASH_MAGIC) == MR_CS2_HASH_MAGIC) ||
          (Hash == 0 && Command == MR_CS2_CMD_CFGDAT_STREAM));
}
