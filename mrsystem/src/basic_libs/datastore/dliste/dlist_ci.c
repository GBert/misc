#include <stddef.h>
#include <stdlib.h>
#include <compare.h>
#include "dliste.h"

DlisteIterator *DlisteCreateIterator(void)
{  DlisteIterator *Iter;

   Iter = (DlisteIterator *)malloc(sizeof(DlisteIterator));
   if (Iter != NULL)
   {
      Iter->Aktuell = NULL;
      Iter->Wurzel = NULL;
   }
   return(Iter);
}
