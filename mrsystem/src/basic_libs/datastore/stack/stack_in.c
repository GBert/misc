#include <stddef.h>
#include <liste.h>
#include "stack.h"

void StackInit(Stack *Wurzel, StackDelCbFkt Cb)
{
   ListeInit(Wurzel->StackDaten,
             (CmpFkt)NULL,
             (ListeKeyDelCbFkt)NULL,
             (ListeDataDelCbFkt)Cb);
}
