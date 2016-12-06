#include "liste.h"

void ListeInit(Liste *Wurzel, CmpFkt Cmp, ListeKeyDelCbFkt DestroyKeyCb, ListeDataDelCbFkt DestroyDatenCb)
{
   Wurzel->Compare = Cmp;
   Wurzel->DestroyKey = DestroyKeyCb;
   Wurzel->DestroyDaten = DestroyDatenCb;
}
