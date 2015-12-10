#include <stddef.h>
#include <stdlib.h>
#include <compare.h>
#include "dliste.h"

void DlisteDelete(Dliste *Wurzel, DlisteKeyType Key)
{  DlisteKnoten WorkPtr;

   /* Liste durchsuchen */
   WorkPtr = Wurzel->DatenAnfang;
   while ((WorkPtr != NULL) && !EQUAL(Wurzel->Compare(WorkPtr->Key, Key)))
   {
      WorkPtr = WorkPtr->Next;
   }
   if (WorkPtr != NULL)
   {
      /* Eintrag wurde gefunden */
      DlisteDelAkt(Wurzel, WorkPtr);
   }
}
