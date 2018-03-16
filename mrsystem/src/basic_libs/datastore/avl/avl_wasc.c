#include <stddef.h>
#include "avl.h"

static void WalkAscend(AvlKnoten Zweig, AvlWalkCbFkt Cb, void *PrivData)
{
   if (Zweig != NULL)
   {
      WalkAscend(Zweig->Link[0], Cb, PrivData);
      Cb(PrivData, Zweig);
      WalkAscend(Zweig->Link[1], Cb, PrivData);
   }
}

void AvlWalkAscend(AvlBaum *Wurzel, AvlWalkCbFkt Cb, void *PrivData)
{
   WalkAscend(Wurzel->Daten, Cb, PrivData);
}
