#include "avl.h"

void AvlInit(AvlBaum *Wurzel, CmpFkt Cmp, AvlKeyDelCbFkt DestroyKeyCb, AvlDataDelCbFkt DestroyDatenCb)
{
   Wurzel->Compare = Cmp;
   Wurzel->DestroyKey = DestroyKeyCb;
   Wurzel->DestroyDaten = DestroyDatenCb;
}
