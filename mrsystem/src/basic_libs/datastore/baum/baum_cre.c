#include <stddef.h>
#include <stdlib.h>
#include <boolean.h>
#include <compare.h>
#include "baum.h"

Baum *BaumCreate(void)
{  Baum *Wurzel;

   Wurzel = (Baum *)malloc(sizeof(Baum));
   if (Wurzel != NULL)
   {
      Wurzel->Daten = NULL;
      Wurzel->Compare = NULL;
      Wurzel->DestroyKey = NULL;
      Wurzel->DestroyDaten = NULL;
   }
   return(Wurzel);
}
