#include <stddef.h>
#include <stdlib.h>
#include <avl.h>
#include "map.h"

void MapDestroy(Map *Wurzel)
{
   if (Wurzel != NULL)
   {
      AvlDestroy(Wurzel->MapDaten);
      free(Wurzel);
   }
}
