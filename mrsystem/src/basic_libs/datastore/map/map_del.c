#include <stddef.h>
#include <stdlib.h>
#include <boolean.h>
#include <avl.h>
#include "map.h"

void MapDel(Map *Wurzel, MapKeyType Key)
{
   AvlDelete(Wurzel->MapDaten, (AvlKeyType)Key);
}
