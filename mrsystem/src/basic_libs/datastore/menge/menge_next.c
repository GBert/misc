#include <stddef.h>
#include <dliste.h>
#include "menge.h"

MengeDataType MengeNext(MengeIterator *Iter)
{  DlisteKnoten NextNode;

   NextNode = DlisteNext(&(Iter->Iter));
   if (NextNode != NULL)
   {
      return(NextNode->Data);
   }
   else
   {
      return(NULL);
   }
}
