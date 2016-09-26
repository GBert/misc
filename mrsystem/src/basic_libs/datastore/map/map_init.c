#include <stddef.h>
#include <stdlib.h>
#include <avl.h>
#include "map.h"

void MapInit(Map *Wurzel, CmpFkt Cmp, MapKeyDelCbFkt DestroyKeyCb, MapDataDelCbFkt DestroyDatenCb)
{
   Wurzel->DestroyKey = DestroyKeyCb;
   Wurzel->DestroyDaten = DestroyDatenCb;
   AvlInit(Wurzel->MapDaten, Cmp,
           (AvlKeyDelCbFkt)DestroyKeyCb,
           (AvlDataDelCbFkt)DestroyDatenCb);
}
