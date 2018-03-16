#include <stddef.h>
#include "baum.h"

static void WalkPostorder(BaumKnoten Zweig, BaumWalkCbFkt Cb, void *PrivData)
{
   if (Zweig != NULL)
   {
      WalkPostorder(Zweig->Left, Cb, PrivData);
      WalkPostorder(Zweig->Right, Cb, PrivData);
      Cb(Zweig, PrivData);
   }
}

void BaumWalkPostorder(Baum *Wurzel, BaumWalkCbFkt Cb, void *PrivData)
{
   WalkPostorder(Wurzel->Daten, Cb, PrivData);
}
