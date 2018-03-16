#include <stddef.h>
#include <stdlib.h>
#include <dliste.h>
#include "queue.h"

QueueDataType QueueGet(Queue *Wurzel)
{  DlisteKnoten LastNode;
   DlisteIterator Iter;
   QueueDataType Daten;

   DlisteInitIterator(&Iter, Wurzel->QueueDaten);
   LastNode = DlisteLast(&Iter);
   if (LastNode != NULL)
   {
      Daten = (QueueDataType)LastNode->Data;
      LastNode->Data = (DlisteDataType)NULL;
      DlisteDelAkt(Wurzel->QueueDaten, LastNode);
   }
   else
   {
      Daten = (QueueDataType)NULL;
   }
   return(Daten);
}
