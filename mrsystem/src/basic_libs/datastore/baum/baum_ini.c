#include "baum.h"

void BaumInit(Baum *Wurzel, CmpFkt Cmp, BaumKeyDelCbFkt DestroyKeyCb, BaumDataDelCbFkt DestroyDatenCb)
{
   Wurzel->Compare = Cmp;
   Wurzel->DestroyKey = DestroyKeyCb;
   Wurzel->DestroyDaten = DestroyDatenCb;
}
