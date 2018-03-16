#include <stddef.h>
#include "baum.h"

static void WalkPraeorder(BaumKnoten Zweig, BaumWalkCbFkt Cb, void *PrivData)
{
   if (Zweig != NULL)
   {
      Cb(PrivData, Zweig);
      WalkPraeorder(Zweig->Left, Cb, PrivData);
      WalkPraeorder(Zweig->Right, Cb, PrivData);
   }
}

void BaumWalkPraeorder(Baum *Wurzel, BaumWalkCbFkt Cb, void *PrivData)
{
   WalkPraeorder(Wurzel->Daten, Cb, PrivData);
}
