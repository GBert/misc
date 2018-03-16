#include <stddef.h>
#include <stdlib.h>
#include <boolean.h>
#include <compare.h>
#include "liste.h"

void ListeDestroy(Liste *Wurzel)
{  ListeKnoten NextPtr, DelPtr;

   if (Wurzel->Daten != NULL)
   {
      DelPtr = Wurzel->Daten;
      while (DelPtr != NULL)
      {
         NextPtr = DelPtr->Next;
         if (Wurzel->DestroyKey != NULL)
         {
            Wurzel->DestroyKey(DelPtr->Key);
         }
         if (Wurzel->DestroyDaten != NULL)
         {
            Wurzel->DestroyDaten(DelPtr->Data);
         }
         free(DelPtr);
         DelPtr = NextPtr;
      }
      free(Wurzel);
   }
}
