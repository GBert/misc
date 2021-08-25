#include "dliste.h"

DlisteKnoten DlisteFirst(DlisteIterator *Iter)
{
   Iter->Aktuell = Iter->Wurzel->DatenAnfang;
   return(Iter->Aktuell);
}
