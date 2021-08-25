#ifndef LISTE_H
#define LISTE_H

#include <boolean.h>
#include <compare.h>

typedef void *ListeKeyType;
typedef void *ListeDataType;
typedef void (*ListeKeyDelCbFkt)(ListeKeyType Key);
typedef void (*ListeDataDelCbFkt)(ListeDataType Daten);
typedef struct liste_element {
   ListeKeyType Key;
   ListeDataType Data;
   struct liste_element *Next;
} ListeElement, *ListeKnoten;
typedef struct {
   ListeKnoten Daten;
   ListeKnoten Aktuell;
   CmpFkt Compare;
   ListeKeyDelCbFkt DestroyKey;
   ListeDataDelCbFkt DestroyDaten;
} Liste;

Liste *ListeCreate(void);
void ListeDestroy(Liste *Wurzel);
void ListeInit(Liste *Wurzel, CmpFkt Cmp, ListeKeyDelCbFkt DestroyKeyCb, ListeDataDelCbFkt DestroyDatenCb);
BOOL ListeIsEmpty(Liste *Wurzel);
BOOL ListeInsert(Liste *Wurzel, ListeKeyType Key, ListeDataType Daten);
BOOL ListeAhead(Liste *Wurzel, ListeKeyType Key, ListeDataType Daten);
BOOL ListeAppend(Liste *Wurzel, ListeKeyType Key, ListeDataType Daten);
void ListeDelete(Liste *Wurzel, ListeKeyType Key);
void ListeDelFirst(Liste *Wurzel);
ListeKnoten ListeFinde(Liste *Wurzel, ListeKeyType Key);
ListeKnoten ListeFirst(Liste *Wurzel);
ListeKnoten ListeNext(Liste *Wurzel);

#endif
