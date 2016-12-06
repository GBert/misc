#include <stddef.h>
#include <stdlib.h>
#include <boolean.h>
#include <dliste.h>
#include "menge.h"

MengeIterator *MengeCreateIter(void)
{  MengeIterator *NewIter;

   NewIter = (MengeIterator *)malloc(sizeof(MengeIterator));
   if (NewIter != (MengeIterator *)NULL)
   {
      NewIter->Wurzel = (Menge *)NULL;
   }
   return(NewIter);
}
