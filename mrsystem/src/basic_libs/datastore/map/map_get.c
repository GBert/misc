#include <stddef.h>
#include <avl.h>
#include "map.h"

MapDataType MapGet(Map *Wurzel, MapKeyType Key)
{  AvlKnoten PresentData;

   PresentData = AvlFinde(Wurzel->MapDaten, (AvlKeyType)Key);
   if (PresentData != NULL)
   {
      return((MapDataType)PresentData->Daten);
   }
   else
   {
      return((MapDataType)NULL);
   }
}
