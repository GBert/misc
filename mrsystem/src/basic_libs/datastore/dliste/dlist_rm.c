#include <stddef.h>
#include "dliste.h"

DlisteKnoten DlisteRemove(DlisteIterator *Iter)
{
   if (Iter->Aktuell != NULL)
   {  DlisteKnoten Merk;

      Merk = Iter->Aktuell;
      Iter->Aktuell = Iter->Aktuell->Next;
      DlisteDelAkt(Iter->Wurzel, Merk);
   }
   return(Iter->Aktuell);
}
