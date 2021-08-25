#include <boolean.h>
#include "cs2.h"

BOOL Cs2IsCs2Msg(unsigned Hash, unsigned Command)
{
   return(((Hash & CS2_MASK_HASH_MAGIC) == CS2_HASH_MAGIC) ||
          (Hash == 0 && Command == CS2_CMD_CFGDAT_STREAM));
}
