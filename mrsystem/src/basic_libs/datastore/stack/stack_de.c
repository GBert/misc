#include <stddef.h>
#include <stdlib.h>
#include <liste.h>
#include "stack.h"

void StackDestroy(Stack *Wurzel)
{
   ListeDestroy(Wurzel->StackDaten);
   free(Wurzel);
}
