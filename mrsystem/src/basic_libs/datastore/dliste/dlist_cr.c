#include <stddef.h>
#include <stdlib.h>
#include <compare.h>
#include "dliste.h"

Dliste *DlisteCreate(void)
{  Dliste *Wurzel;

   Wurzel = (Dliste *)malloc(sizeof(Dliste));
   if (Wurzel != NULL)
   {
      Wurzel->DatenAnfang = NULL;
      Wurzel->DatenEnde = NULL;
      Wurzel->Compare = NULL;
      Wurzel->DestroyKey = NULL;
      Wurzel->DestroyDaten = NULL;
   }
   return(Wurzel);
}
