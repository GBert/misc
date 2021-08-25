#include <stddef.h>
#include <dliste.h>
#include "menge.h"

MengeDataType MengeIterRemove(MengeIterator *Iter)
{  DlisteKnoten NextNode;

   NextNode = DlisteRemove(&(Iter->Iter));
   if (NextNode != NULL)
   {
      return(NextNode->Data);
   }
   else
   {
      return(NULL);
   }
}
