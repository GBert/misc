#include <stddef.h>
#include <stdlib.h>
#include "liste.h"

void ListeDelFirst(Liste *Wurzel)
{  ListeKnoten ReturnPtr;

   if (Wurzel->Daten != NULL)
   {
      /* Die Liste enthaelt Elemente */
      ReturnPtr = Wurzel->Daten;
      Wurzel->Daten = Wurzel->Daten->Next;
      if ((Wurzel->DestroyKey != NULL) && (ReturnPtr->Key != NULL))
      {
         Wurzel->DestroyKey(ReturnPtr->Key);
      }
      if ((Wurzel->DestroyDaten != NULL) && (ReturnPtr->Data != NULL))
      {
         Wurzel->DestroyDaten(ReturnPtr->Data);
      }
      free(ReturnPtr);
   }
}
