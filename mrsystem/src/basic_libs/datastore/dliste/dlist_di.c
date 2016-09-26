#include <stddef.h>
#include <stdlib.h>
#include <boolean.h>
#include <compare.h>
#include "dliste.h"

void DlisteDestroyIterator(DlisteIterator *Iter)
{
   if (Iter != NULL)
   {
      free(Iter);
   }
}
