#include <stddef.h>
#include <stdlib.h>
#include <dliste.h>
#include "menge.h"

void MengeDestroy(Menge *Wurzel)
{
   if (Wurzel != NULL)
   {
      DlisteDestroy(Wurzel->MengeDaten);
      free(Wurzel);
   }
}
