#include <stddef.h>
#include <stdlib.h>
#include <boolean.h>
#include <compare.h>
#include "liste.h"

BOOL ListeInsert(Liste *Wurzel, ListeKeyType Key, ListeDataType Daten)
{  ListeKnoten WorkPtr, NewElement;
   BOOL fertig;

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
         /* die einzufuegende Stelle suchen */
         if (Wurzel->Compare(NewElement->Key, Wurzel->Daten->Key) <= 0)
         {
            /* als erstes Element einfuegen, da kleinstes Element */
            NewElement->Next = Wurzel->Daten;
            Wurzel->Daten = NewElement;
         }
         else
         {
            /* Liste durchsuchen */
            WorkPtr = Wurzel->Daten;
            fertig = FALSE;
            while ((WorkPtr->Next != NULL) && !fertig)
            {
               if (Wurzel->Compare(WorkPtr->Next->Key, NewElement->Key) > 0)
               {
                  /* Der Nachfolger der aktuellen Position hat
                     einen groesseren Schlussel -> einfuegen */
                  NewElement->Next = WorkPtr->Next;
                  WorkPtr->Next = NewElement;
                  fertig = TRUE;
               }
               else
                  /* noch nicht gefunden -> Liste weiter durchsuchen */
                  WorkPtr = WorkPtr->Next;
            }
            if (fertig)
            {
               /* Es wurde in der Liste keine Stelle zum Einfuegen
                  gefunden -> am Ende anhaengen */
               NewElement->Next = NULL;
               WorkPtr->Next = NewElement;
            }
         }
      }
      return(TRUE);
   }
   else
      return(FALSE);
}
