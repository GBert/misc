#include <stddef.h>
#include <stdlib.h>
#include <boolean.h>
#include <avl.h>
#include "map.h"

BOOL MapSet(Map *Wurzel, MapKeyType Key, MapDataType Daten)
{
   AvlDelete(Wurzel->MapDaten, (AvlKeyType)Key);
   return(AvlInsert(Wurzel->MapDaten, (AvlKeyType)Key, (AvlDataType)Daten));
}
