#include <stddef.h>
#include <stdlib.h>
#include <boolean.h>
#include <compare.h>
#include "liste.h"

Liste *ListeCreate(void)
{  Liste *Wurzel;

   Wurzel = (Liste *)malloc(sizeof(Liste));
   if (Wurzel != NULL)
   {
      Wurzel->Daten = NULL;
      Wurzel->Aktuell = NULL;
      Wurzel->Compare = NULL;
      Wurzel->DestroyKey = NULL;
      Wurzel->DestroyDaten = NULL;
   }
   return(Wurzel);
}
