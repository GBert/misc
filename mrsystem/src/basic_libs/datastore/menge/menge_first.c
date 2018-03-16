#include <stddef.h>
#include <dliste.h>
#include "menge.h"

MengeDataType MengeFirst(MengeIterator *Iter)
{  DlisteKnoten FirstNode;

   FirstNode = DlisteFirst(&(Iter->Iter));
   if (FirstNode != (DlisteKnoten)NULL)
   {
      return(FirstNode->Data);
   }
   else
   {
      return((MengeDataType)NULL);
   }
}
