#include <stddef.h>
#include "dliste.h"

DlisteKnoten DlistePrev(DlisteIterator *Iter)
{
   if (Iter->Aktuell != NULL)
   {
      Iter->Aktuell = Iter->Aktuell->Prev;
   }
   return(Iter->Aktuell);
}
