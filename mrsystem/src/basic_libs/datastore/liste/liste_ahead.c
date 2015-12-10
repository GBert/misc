#include <stddef.h>
#include <stdlib.h>
#include <boolean.h>
#include <compare.h>
#include "liste.h"

BOOL ListeAhead(Liste *Wurzel, ListeKeyType Key, ListeDataType Daten)
{  ListeKnoten NewElement;

   /* neues Listenelement erzeugen */
   NewElement = malloc(sizeof(ListeElement));
   if (NewElement != NULL)
   {
      NewElement->Key = Key;
      NewElement->Data = Daten;
      /* Listenelement als erstes Element einfuegen */
      NewElement->Next = Wurzel->Daten;
      Wurzel->Daten = NewElement;
      return(TRUE);
   }
   else
      return(FALSE);
}
