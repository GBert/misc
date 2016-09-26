#include <stddef.h>
#include <stdlib.h>
#include <boolean.h>
#include <compare.h>
#include "dliste.h"

void DlisteDestroy(Dliste *Wurzel)
{  DlisteKnoten NextPtr, DelPtr;

   if (Wurzel != NULL)
   {
      DelPtr = Wurzel->DatenAnfang;
      while (DelPtr != NULL)
      {
         NextPtr = DelPtr->Next;
         if ((Wurzel->DestroyKey != NULL) && (DelPtr->Key != NULL))
         {
            Wurzel->DestroyKey(DelPtr->Key);
         }
         if ((Wurzel->DestroyDaten != NULL) && (DelPtr->Data != NULL))
         {
            Wurzel->DestroyDaten(DelPtr->Data);
         }
         free(DelPtr);
         DelPtr = NextPtr;
      }
      free(Wurzel);
   }
}
