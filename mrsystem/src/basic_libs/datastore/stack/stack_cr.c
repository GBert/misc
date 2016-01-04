#include <stddef.h>
#include <stdlib.h>
#include <liste.h>
#include "stack.h"

Stack *StackCreate(void)
{  Stack *NewStack;

   NewStack = (Stack *)malloc(sizeof(Stack));
   if (NewStack != NULL)
   {
      NewStack->StackDaten = ListeCreate();
      if (NewStack->StackDaten == NULL)
      {
         free(NewStack);
         NewStack = NULL;
      }
   }
   return(NewStack);
}
