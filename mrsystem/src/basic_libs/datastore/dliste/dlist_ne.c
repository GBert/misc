#include <stddef.h>
#include "dliste.h"

DlisteKnoten DlisteNext(DlisteIterator *Iter)
{
   if (Iter->Aktuell != NULL)
   {
      Iter->Aktuell = Iter->Aktuell->Next;
   }
   return(Iter->Aktuell);
}
