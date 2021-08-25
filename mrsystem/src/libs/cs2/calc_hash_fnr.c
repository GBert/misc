#include "cs2.h"

unsigned short Cs2CalcHashFromFolgenummer(unsigned int Folgenummer)
{  unsigned short CanHash;

   CanHash = ((Folgenummer & CS2_FOLGENUMMER_HIGH) << CS2_SHIFT_HASH_MSB) |
             (Folgenummer & CS2_FOLGENUMMER_LOW);
   CanHash &= ~CS2_MASK_HASH_MAGIC;         /* mask bits 7-9 */
   CanHash |= CS2_HASH_MAGIC;               /* set bits 8-9 */
   return(CanHash);
}
