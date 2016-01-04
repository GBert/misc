#include <avl.h>
#include "map.h"

typedef struct {
   MapWalkCbFkt WalkCb;
   void *PrivData;
} WalkStruct;


static void AvlWalkAscendCb(void *PrivData, AvlKnoten Element)
{
   ((WalkStruct *)PrivData)->WalkCb(((WalkStruct *)PrivData)->PrivData,
                                    Element->Key, Element->Daten);
}

void MapWalkAscend(Map *Wurzel, MapWalkCbFkt Cb, void *PrivData)
{  WalkStruct CbData;

   CbData.WalkCb = Cb;
   CbData.PrivData = PrivData;
   AvlWalkAscend(Wurzel->MapDaten, AvlWalkAscendCb, &CbData);
}
