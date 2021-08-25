#include "mr_can.h"

unsigned MrCs2CalcHashFromFolgenummer(unsigned Folgenummer)
{  unsigned CanHash;

   CanHash = ((Folgenummer & MR_CS2_FOLGENUMMER_HIGH) << MR_CS2_SHIFT_HASH_MSB) |
             (Folgenummer & MR_CS2_FOLGENUMMER_LOW);
   CanHash &= ~MR_CS2_MASK_HASH_MAGIC;         /* mask bits 7-9 */
   CanHash |= MR_CS2_HASH_MAGIC;               /* set bits 8-9 */
   return(CanHash);
}
