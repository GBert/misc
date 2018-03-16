#ifndef BAUM_H
#define BAUM_H

#include <boolean.h>
#include <compare.h>

typedef void *BaumKeyType;
typedef void *BaumDataType;
typedef void (*BaumKeyDelCbFkt)(BaumKeyType Key);
typedef void (*BaumDataDelCbFkt)(BaumDataType Daten);
typedef struct baum_knoten {
   BaumKeyType Key;
   BaumDataType Daten;
   struct baum_knoten *Left,*Right;
} BaumElement, *BaumKnoten;
typedef struct {
   BaumKnoten Daten;
   CmpFkt Compare;
   BaumKeyDelCbFkt DestroyKey;
   BaumDataDelCbFkt DestroyDaten;
} Baum;
typedef void (*BaumWalkCbFkt)(void *PrivData, BaumKnoten Element);

Baum *BaumCreate(void);
void BaumDestroy(Baum *Wurzel);
void BaumInit(Baum *Wurzel, CmpFkt Cmp, BaumKeyDelCbFkt DestroyKeyCb, BaumDataDelCbFkt DestroyDatenCb);
BOOL BaumInsert(Baum *Wurzel, BaumKeyType Key, BaumDataType Daten);
void BaumDelete(Baum *Wurzel, BaumKeyType Key);
BaumKnoten BaumFinde(Baum *Wurzel, BaumKeyType Key);
void BaumWalkPraeorder(Baum *Wurzel, BaumWalkCbFkt Cb, void *PrivData);
void BaumWalkPostorder(Baum *Wurzel, BaumWalkCbFkt Cb, void *PrivData);
void BaumWalkInorder(Baum *Wurzel, BaumWalkCbFkt Cb, void *PrivData);

#endif
