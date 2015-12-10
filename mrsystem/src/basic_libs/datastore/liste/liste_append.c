#include <stddef.h>
#include <stdlib.h>
#include <boolean.h>
#include <compare.h>
#include "liste.h"

BOOL ListeAppend(Liste *Wurzel, ListeKeyType Key, ListeDataType Daten)
{  ListeKnoten WorkPtr, NewElement;

   /* neues Listenelement erzeugen */
   NewElement = malloc(sizeof(ListeElement));
   if (NewElement != NULL)
   {
      NewElement->Key = Key;
      NewElement->Data = Daten;
      /* Listenelement einfuegen */
      if (Wurzel->Daten == NULL)
      {
         /* Liste ist leer -> als erstes Element einfuegen */
         NewElement->Next = NULL;
         Wurzel->Daten = NewElement;
      }
      else
      {
         /* Liste bis zum Ende durchlaufen */
         WorkPtr = Wurzel->Daten;
         while (WorkPtr->Next != NULL)
            WorkPtr = WorkPtr->Next;
         /* am Ende anhaengen */
         NewElement->Next = NULL;
         WorkPtr->Next = NewElement;
      }
      return(TRUE);
   }
   else
      return(FALSE);
}
