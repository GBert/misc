#include <stddef.h>
#include <stdlib.h>
#include <compare.h>
#include "dliste.h"

void DlisteDelFirst(Dliste *Wurzel)
{  DlisteKnoten ReturnPtr;

   if (Wurzel->DatenAnfang != NULL)
   {
      /* Die Liste enthaelt Elemente */
      DlisteDelAkt(Wurzel, Wurzel->DatenAnfang);
   }
}
