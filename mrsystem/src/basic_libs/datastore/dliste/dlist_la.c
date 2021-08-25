#include "dliste.h"

DlisteKnoten DlisteLast(DlisteIterator *Iter)
{
   Iter->Aktuell = Iter->Wurzel->DatenEnde;
   return(Iter->Aktuell);
}
