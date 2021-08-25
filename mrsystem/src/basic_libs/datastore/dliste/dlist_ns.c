#include <stddef.h>
#include <stdlib.h>
#include <boolean.h>
#include <compare.h>
#include "dliste.h"

BOOL DlisteInsert(Dliste *Wurzel, DlisteKeyType Key, DlisteDataType Daten)
{  DlisteKnoten WorkPtr, NewElement;
   BOOL fertig;

   /* neues Listenelement erzeugen */
   NewElement = (DlisteKnoten)malloc(sizeof(DlisteElement));
   if (NewElement != NULL)
   {
      NewElement->Key = Key;
      NewElement->Data = Daten;
      /* Listenelement einfuegen */
      if (Wurzel->DatenAnfang == NULL)
      {
         /* Liste ist leer -> als erstes Element einfuegen */
         NewElement->Prev = NULL;
         NewElement->Next = NULL;
         Wurzel->DatenAnfang = NewElement;
         Wurzel->DatenEnde = Wurzel->DatenAnfang;
      }
      else
      {
         /* die einzufuegende Stelle suchen */
         if (SMALLER(Wurzel->Compare(NewElement->Key, Wurzel->DatenAnfang->Key))
             || EQUAL(Wurzel->Compare(NewElement->Key, Wurzel->DatenAnfang->Key)))
         {
            /* als erstes Element einfuegen, da kleinster Schluessel */
            Wurzel->DatenAnfang->Prev = NewElement;
            NewElement->Prev = NULL;
            NewElement->Next = Wurzel->DatenAnfang;
            Wurzel->DatenAnfang = NewElement;
         }
         else 
         {
            /* Liste durchlaufen */
            WorkPtr = Wurzel->DatenAnfang;
            fertig = FALSE;
            while ((WorkPtr->Next != NULL) && !fertig)
            {
               if (GREATER(Wurzel->Compare(WorkPtr->Next->Key, NewElement->Key)))
               {
                  /* Der Nachfolger der aktuellen Position hat
                     einen groesseren Schluessel -> einfuegen */
                  NewElement->Next = WorkPtr->Next;
                  WorkPtr->Next->Prev = NewElement;
                  NewElement->Prev = WorkPtr;
                  WorkPtr->Next = NewElement;
                  fertig = 1;
               }
               else
                  /* noch nicht gefunden -> Liste weiter durchsuchen */
                  WorkPtr = WorkPtr->Next;
            }
            if (!fertig)
            {
               /* Es wurde in der Liste keine Stelle zum Einfuegen
                  gefunden -> am Ende anhaengen */
               NewElement->Prev = WorkPtr;
               NewElement->Next = NULL;
               WorkPtr->Next = NewElement;
               Wurzel->DatenEnde = NewElement;
            }
         }
      }
      return(TRUE);
   }
   else
   {
      return(FALSE);
   }
}
