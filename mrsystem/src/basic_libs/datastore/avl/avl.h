#ifndef AVL_H
#define AVL_H

#include <boolean.h>
#include <compare.h>

typedef void *AvlKeyType;
typedef void *AvlDataType;
typedef void (*AvlKeyDelCbFkt)(AvlKeyType Key);
typedef void (*AvlDataDelCbFkt)(AvlDataType Daten);
typedef struct avl_knoten {
   AvlKeyType Key;
   AvlKeyType Daten;
   short Bal;
   short cache; /* Used during insertion */
   struct avl_knoten *Link[2];
} AvlElement, *AvlKnoten;
typedef struct {
   AvlKnoten Daten;
   CmpFkt Compare;
   AvlKeyDelCbFkt DestroyKey;
   AvlDataDelCbFkt DestroyDaten;
} AvlBaum;
typedef void (*AvlWalkCbFkt)(void *PrivData, AvlKnoten Element);

AvlBaum *AvlCreate(void);
void AvlDestroy(AvlBaum *Wurzel);
void AvlInit(AvlBaum *Wurzel, CmpFkt Cmp, AvlKeyDelCbFkt DestroyKeyCb, AvlDataDelCbFkt DestroyDatenCb);
BOOL AvlInsert(AvlBaum *Wurzel, AvlKeyType key, AvlDataType Daten);
void AvlDelete(AvlBaum *Wurzel, AvlKeyType Key);
AvlKnoten AvlFinde(AvlBaum *Wurzel, AvlKeyType Key);
void AvlPurge(AvlBaum *Wurzel);
void AvlWalkAscend(AvlBaum *Wurzel, AvlWalkCbFkt Cb, void *PrivData);
void AvlWalkDescend(AvlBaum *Wurzel, AvlWalkCbFkt Cb, void *PrivData);

#endif
