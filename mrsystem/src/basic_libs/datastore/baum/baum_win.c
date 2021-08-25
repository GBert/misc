#include <stddef.h>
#include "baum.h"

static void WalkInorder(BaumKnoten Zweig, BaumWalkCbFkt Cb, void *PrivData)
{
   if (Zweig != NULL)
   {
      WalkInorder(Zweig->Left, Cb, PrivData);
      Cb(PrivData, Zweig);
      WalkInorder(Zweig->Right, Cb, PrivData);
   }
}

void BaumWalkInorder(Baum *Wurzel, BaumWalkCbFkt Cb, void *PrivData)
{
   WalkInorder(Wurzel->Daten, Cb, PrivData);
}
