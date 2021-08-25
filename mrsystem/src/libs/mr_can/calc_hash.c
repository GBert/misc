#include "mr_can.h"

unsigned MrCs2CalcHash(unsigned long Uid)
{  unsigned CanHash;

   CanHash = (Uid & 0x0000ffff) ^ (Uid >> 16); /* xor uid low and high word */
   CanHash = htons(CanHash);                   /* swap bytes if needed */
   CanHash &= ~MR_CS2_MASK_HASH_MAGIC;         /* mask bits 7-9 */
   CanHash |= MR_CS2_HASH_MAGIC;               /* set bits 8-9 */
   return(CanHash);
}
