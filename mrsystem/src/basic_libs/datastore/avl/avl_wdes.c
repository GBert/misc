#include <stddef.h>
#include "avl.h"

static void WalkDescend(AvlKnoten Zweig, AvlWalkCbFkt Cb, void *PrivData)
{
   if (Zweig != NULL)
   {
      WalkDescend(Zweig->Link[1], Cb, PrivData);
      Cb(PrivData, Zweig);
      WalkDescend(Zweig->Link[0], Cb, PrivData);
   }
}

void AvlWalkDescend(AvlBaum *Wurzel, AvlWalkCbFkt Cb, void *PrivData)
{
   WalkDescend(Wurzel->Daten, Cb, PrivData);
}
