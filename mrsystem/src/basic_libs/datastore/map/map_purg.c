#include <stddef.h>
#include <stdlib.h>
#include <avl.h>
#include "map.h"

void MapPurge(Map *Wurzel)
{
   if (Wurzel != NULL)
   {
      AvlPurge(Wurzel->MapDaten);
   }
}
