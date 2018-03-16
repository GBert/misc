#include <stddef.h>
#include <stdlib.h>
#include <boolean.h>
#include <dliste.h>
#include "menge.h"

Menge *MengeCreate(void)
{  Menge *NewMenge;

   NewMenge = (Menge *)malloc(sizeof(Menge));
   if (NewMenge != NULL)
   {
      NewMenge->MengeDaten = DlisteCreate();
      if (NewMenge->MengeDaten == NULL)
      {
         free(NewMenge);
         NewMenge = (Menge *)NULL;
      }
   }
   return(NewMenge);
}
