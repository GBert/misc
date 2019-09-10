#include <arpa/inet.h>
#include "cs2.h"

unsigned short Cs2CalcHash(unsigned long Uid)
{  unsigned short CanHash;

   CanHash = (Uid & 0x0000ffff) ^ (Uid >> 16); /* xor uid low and high word */
   CanHash = htons(CanHash);                   /* swap bytes if needed */
   CanHash &= ~CS2_MASK_HASH_MAGIC;         /* mask bits 7-9 */
   CanHash |= CS2_HASH_MAGIC;               /* set bits 8-9 */
   return(CanHash);
}
