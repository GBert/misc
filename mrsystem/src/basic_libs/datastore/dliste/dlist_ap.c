#include <stddef.h>
#include <stdlib.h>
#include <boolean.h>
#include <compare.h>
#include "dliste.h"

BOOL DlisteAppend(Dliste *Wurzel, DlisteKeyType Key, DlisteDataType Daten)
{  DlisteKnoten NewElement;

   /* neues Listenelement erzeugen */
   NewElement = malloc(sizeof(DlisteElement));
   if (NewElement != NULL)
   {
      NewElement->Key = Key;
      NewElement->Data = Daten;
      /* Listenelement einfuegen */
      if (Wurzel->DatenEnde == NULL)
      {
         /* Liste ist leer -> als erstes Element einfuegen */
         NewElement->Next = NULL;
         NewElement->Prev = NULL;
         Wurzel->DatenAnfang = NewElement;
         Wurzel->DatenEnde = NewElement;
      }
      else
      {
         /* am Ende anhaengen */
         NewElement->Next = NULL;
         NewElement->Prev = Wurzel->DatenEnde;
         Wurzel->DatenEnde->Next = NewElement;
         Wurzel->DatenEnde = NewElement;
      }
      return(TRUE);
   }
   else
   {
      return(FALSE);
   }
}
