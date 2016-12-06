#include <stddef.h>
#include <compare.h>
#include <dliste.h>
#include "menge.h"

void MengeRemove(Menge *Wurzel, MengeDataType Daten)
{  DlisteKnoten WorkPtr;
   DlisteIterator Iter;

   DlisteInitIterator(&Iter, Wurzel->MengeDaten);
   WorkPtr = DlisteFirst(&Iter);
   while ((WorkPtr != NULL) &&
          !EQUAL(Wurzel->Compare((MengeDataType)WorkPtr->Data, Daten)))
   {
      WorkPtr = DlisteNext(&Iter);
   }
   if (WorkPtr != NULL)
   {
      DlisteDelAkt(Wurzel->MengeDaten, WorkPtr);
   }
}
