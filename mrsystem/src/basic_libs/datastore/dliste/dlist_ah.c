#include <stddef.h>
#include <stdlib.h>
#include <boolean.h>
#include "dliste.h"

BOOL DlisteAhead(Dliste *Wurzel, DlisteKeyType Key, DlisteDataType Daten)
{  DlisteKnoten NewElement;

   /* neues Listenelement erzeugen */
   NewElement = (DlisteKnoten)malloc(sizeof(DlisteElement));
   if (NewElement != NULL)
   {
      NewElement->Key = Key;
      NewElement->Data = Daten;
      NewElement->Next = Wurzel->DatenAnfang;
      /* Listenelement als erstes Element einfuegen */
      if (Wurzel->DatenEnde == NULL)
      {
         Wurzel->DatenEnde = NewElement;
      }
      else
      {
         Wurzel->DatenAnfang->Prev = NewElement;
      }
      Wurzel->DatenAnfang = NewElement;
      NewElement->Prev = NULL;
      return(TRUE);
   }
   else
      return(FALSE);
}
