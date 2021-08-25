#include <boolean.h>
#include <liste.h>
#include "stack.h"

BOOL StackIsEmpty(Stack *Wurzel)
{
   return(ListeIsEmpty(Wurzel->StackDaten));
}
