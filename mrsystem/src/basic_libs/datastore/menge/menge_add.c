#include <stddef.h>
#include <boolean.h>
#include <dliste.h>
#include "menge.h"

BOOL MengeAdd(Menge *Wurzel, MengeDataType Daten)
{
   return(DlisteAhead(Wurzel->MengeDaten,
                      (DlisteKeyType)NULL, (DlisteDataType)Daten));
}
