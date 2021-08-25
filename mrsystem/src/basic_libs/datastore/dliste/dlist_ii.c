#include <stddef.h>
#include <stdlib.h>
#include <compare.h>
#include "dliste.h"

void DlisteInitIterator(DlisteIterator *Iter, Dliste *Wurzel)
{
   if (Iter != NULL)
   {
      Iter->Aktuell = NULL;
      Iter->Wurzel = Wurzel;
   }
}
