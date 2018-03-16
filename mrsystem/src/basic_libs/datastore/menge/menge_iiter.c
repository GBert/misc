#include <stddef.h>
#include <stdlib.h>
#include <compare.h>
#include <dliste.h>
#include "menge.h"

void MengeInitIterator(MengeIterator *Iter, Menge *Wurzel)
{
   DlisteInitIterator(&(Iter->Iter), Wurzel->MengeDaten);
   Iter->Wurzel = Wurzel;
}
