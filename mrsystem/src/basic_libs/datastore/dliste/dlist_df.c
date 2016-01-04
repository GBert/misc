#include <stddef.h>
#include <stdlib.h>
#include <compare.h>
#include "dliste.h"

void DlisteDelFirst(Dliste *Wurzel)
{
   if (Wurzel->DatenAnfang != NULL)
   {
      /* Die Liste enthaelt Elemente */
      DlisteDelAkt(Wurzel, Wurzel->DatenAnfang);
   }
}
