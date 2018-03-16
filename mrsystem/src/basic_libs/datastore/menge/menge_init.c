#include <stddef.h>
#include <stdlib.h>
#include <compare.h>
#include <dliste.h>
#include "menge.h"

void MengeInit(Menge *Wurzel, CmpFkt Cmp, MengeDelCbFkt DestroyDatenCb)
{
   DlisteInit(Wurzel->MengeDaten, (CmpFkt)NULL,
              (DlisteKeyDelCbFkt)NULL,
              (DlisteDataDelCbFkt)DestroyDatenCb);
   Wurzel->Compare = Cmp;
}
