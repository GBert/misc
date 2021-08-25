#include <stddef.h>
#include <boolean.h>
#include <liste.h>
#include "stack.h"

BOOL StackPush(Stack *Wurzel, StackDataType Daten)
{
   return(ListeAhead(Wurzel->StackDaten, (ListeKeyType)NULL, (ListeDataType)Daten));
}
