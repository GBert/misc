#include <stddef.h>
#include <stdlib.h>
#include <boolean.h>
#include "dliste.h"

BOOL DlisteIsEmpty(Dliste *Wurzel)
{
   return(Wurzel->DatenAnfang == NULL);
}
