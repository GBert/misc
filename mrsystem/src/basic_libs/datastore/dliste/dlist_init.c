#include "dliste.h"

void DlisteInit(Dliste *Wurzel, CmpFkt Cmp, DlisteKeyDelCbFkt DestroyKeyCb, DlisteDataDelCbFkt DestroyDatenCb)
{
   Wurzel->Compare = Cmp;
   Wurzel->DestroyKey = DestroyKeyCb;
   Wurzel->DestroyDaten = DestroyDatenCb;
}
