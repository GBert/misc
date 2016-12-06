#ifndef DLISTE_H
#define DLISTE_H

#include <boolean.h>
#include <compare.h>

typedef void * DlisteKeyType;
typedef void * DlisteDataType;
typedef void (*DlisteKeyDelCbFkt)(DlisteKeyType Key);
typedef void (*DlisteDataDelCbFkt)(DlisteDataType Daten);
typedef struct dliste_element {
   DlisteKeyType Key;
   DlisteDataType Data;
   struct dliste_element *Prev;
   struct dliste_element *Next;
} DlisteElement, *DlisteKnoten;
typedef struct {
   DlisteKnoten DatenAnfang;
   DlisteKnoten DatenEnde;
   CmpFkt Compare;
   DlisteKeyDelCbFkt DestroyKey;
   DlisteDataDelCbFkt DestroyDaten;
} Dliste;
typedef struct {
   DlisteKnoten Aktuell;
   Dliste *Wurzel;
} DlisteIterator;

Dliste *DlisteCreate(void);
void DlisteDestroy(Dliste *Wurzel);
void DlisteInit(Dliste *Wurzel, CmpFkt Cmp, DlisteKeyDelCbFkt DestroyKeyCb, DlisteDataDelCbFkt DestroyDatenCb);
BOOL DlisteIsEmpty(Dliste *Wurzel);
BOOL DlisteInsert(Dliste *Wurzel, DlisteKeyType Key, DlisteDataType Daten);
BOOL DlisteAhead(Dliste *Wurzel, DlisteKeyType Key, DlisteDataType Daten);
BOOL DlisteAppend(Dliste *Wurzel, DlisteKeyType Key, DlisteDataType Daten);
void DlisteDelete(Dliste *Wurzel, DlisteKeyType Key);
void DlisteDelFirst(Dliste *Wurzel);
void DlisteDelLast(Dliste *Wurzel);
void DlisteDelAkt(Dliste *Wurzel, DlisteKnoten Aktuell);
DlisteKnoten DlisteFinde(Dliste *Wurzel, DlisteKeyType Key);
DlisteIterator *DlisteCreateIterator(void);
void DlisteDestroyIterator(DlisteIterator *Iter);
void DlisteInitIterator(DlisteIterator *Iter, Dliste *Wurzel);
DlisteKnoten DlisteFirst(DlisteIterator *Iter);
DlisteKnoten DlisteNext(DlisteIterator *Iter);
DlisteKnoten DlisteLast(DlisteIterator *Iter);
DlisteKnoten DlistePrev(DlisteIterator *Iter);
DlisteKnoten DlisteRemove(DlisteIterator *Iter);

#endif
