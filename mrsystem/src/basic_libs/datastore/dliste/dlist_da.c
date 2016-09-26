#include <stddef.h>
#include <stdlib.h>
#include "dliste.h"

void DlisteDelAkt(Dliste *Wurzel, DlisteKnoten Aktuell)
{
   if (Aktuell->Prev == NULL)
   {
      /* das erste Element ist das aktuelle */
      Wurzel->DatenAnfang = Aktuell->Next;
   }
   else
   {
      /* Nachfolger des Vorgaengers ist eigener Nachfolger */
      Aktuell->Prev->Next = Aktuell->Next;
   }
   if (Aktuell->Next == NULL)
   {
      /* das letzte Element ist das aktuelle */
      Wurzel->DatenEnde = Aktuell->Prev;
   }
   else
   {
      /* Vorgaenger des Nachfolgers ist eigener Vorgaenger */
      Aktuell->Next->Prev = Aktuell->Prev;
   }
   if ((Wurzel->DestroyKey != NULL) && (Aktuell->Key != NULL))
   {
      Wurzel->DestroyKey(Aktuell->Key);
   }
   if ((Wurzel->DestroyDaten != NULL) && (Aktuell->Data != NULL))
   {
      Wurzel->DestroyDaten(Aktuell->Data);
   }
   free(Aktuell);
}
