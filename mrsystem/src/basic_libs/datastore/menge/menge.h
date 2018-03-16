#ifndef MENGE_H
#define MENGE_H

#include <boolean.h>
#include <compare.h>
#include <dliste.h>

typedef void *MengeDataType;
typedef void (*MengeDelCbFkt)(MengeDataType Daten);
typedef struct {
   Dliste *MengeDaten;
   CmpFkt Compare;
} Menge;
typedef struct {
   DlisteIterator Iter;
   Menge *Wurzel;
} MengeIterator;

Menge *MengeCreate(void);
void MengeDestroy(Menge *Wurzel);
void MengeInit(Menge *Wurzel, CmpFkt Cmp, MengeDelCbFkt DestroyDatenCb);
BOOL MengeAdd(Menge *Wurzel, MengeDataType Daten);
void MengeRemove(Menge *Wurzel, MengeDataType Daten);
MengeIterator *MengeCreateIter(void);
void MengeDestroyIterator(MengeIterator *Iter);
void MengeInitIterator(MengeIterator *Iter, Menge *Wurzel);
MengeDataType MengeFirst(MengeIterator *Iter);
MengeDataType MengeNext(MengeIterator *Iter);
MengeDataType MengeIterRemove(MengeIterator *Iter);

#endif
