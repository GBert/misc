#include <stddef.h>
#include <stdlib.h>
#include <boolean.h>
#include <avl.h>
#include "map.h"

Map *MapCreate(void)
{  Map *NewMap;

   NewMap = (Map *)malloc(sizeof(Map));
   if (NewMap != NULL)
   {
      NewMap->MapDaten = AvlCreate();
      if (NewMap->MapDaten == NULL)
      {
         free(NewMap);
         NewMap = (Map *)NULL;
      }
      else
      {
         NewMap->DestroyKey = NULL;
         NewMap->DestroyDaten = NULL;
      }
   }
   return(NewMap);
}
