#include <stddef.h>
#include <stdlib.h>
#include <compare.h>
#include "dliste.h"

void DlisteDelLast(Dliste *Wurzel)
{
   if (Wurzel->DatenEnde != NULL)
   {
      /* Die Liste enthaelt Elemente */
      DlisteDelAkt(Wurzel, Wurzel->DatenEnde);
   }
}
