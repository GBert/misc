#include <stddef.h>
#include <stdlib.h>
#include <dliste.h>
#include "menge.h"

void MengeDestroyIterator(MengeIterator *Iter)
{
   if (Iter != NULL)
   {
      free(Iter);
   }
}
