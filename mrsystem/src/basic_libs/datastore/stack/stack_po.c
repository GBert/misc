#include <stddef.h>
#include <stdlib.h>
#include <liste.h>
#include "stack.h"

StackDataType StackPop(Stack *Wurzel)
{  ListeKnoten FirstNode;
   StackDataType Daten;

   FirstNode = ListeFirst(Wurzel->StackDaten);
   if (FirstNode != (ListeKnoten)NULL)
   {
      Daten = (StackDataType)FirstNode->Data;
      FirstNode->Data = (ListeDataType)NULL;
      ListeDelFirst(Wurzel->StackDaten);
   }
   else
   {
      Daten = (StackDataType)NULL;
   }
   return(Daten);
}
