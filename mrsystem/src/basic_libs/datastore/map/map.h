#ifndef MAP_H
#define MAP_H

#include <boolean.h>
#include <avl.h>

typedef void *MapKeyType;
typedef void *MapDataType;
typedef void (*MapKeyDelCbFkt)(MapKeyType Key);
typedef void (*MapDataDelCbFkt)(MapDataType Daten);
typedef void (*MapWalkCbFkt)(void *PrivData, MapKeyType Key, MapDataType Daten);
typedef struct {
   AvlBaum *MapDaten;
   MapKeyDelCbFkt DestroyKey;
   MapDataDelCbFkt DestroyDaten;
} Map;

Map *MapCreate(void);
void MapDestroy(Map *Wurzel);
void MapInit(Map *Wurzel, CmpFkt Cmp, MapKeyDelCbFkt DestroyKeyCb, MapDataDelCbFkt DestroyDatenCb);
BOOL MapSet(Map *Wurzel, MapKeyType Key, MapDataType Daten);
void MapDel(Map *Wurzel, MapKeyType Key);
void MapPurge(Map *Wurzel);
MapDataType MapGet(Map *Wurzel, MapKeyType Key);
void MapWalkAscend(Map *Wurzel, MapWalkCbFkt Cb, void *PrivData);

#endif
