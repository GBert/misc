#include <stddef.h>
#include <stdlib.h>
#include <boolean.h>
#include <compare.h>
#include "baum.h"

static BOOL Insert(Baum *Wurzel, BaumKnoten *Zweig, BaumKeyType Key, BaumDataType Daten)
{  BaumKnoten Element;

   if (*Zweig == NULL)
   {
      Element = (BaumKnoten)malloc(sizeof(BaumElement));
      if (Element != NULL)
      {
         Element->Key = Key;
         Element->Daten = Daten;
         Element->Right = NULL;
         Element->Left = NULL;
         *Zweig = Element;
         return(TRUE);
      }
      else
      {
         return(FALSE);
      }
   }
   else if (SMALLER(Wurzel->Compare(Key, (*Zweig)->Key)))
   {
      return(Insert(Wurzel, &((*Zweig)->Left), Key, Daten));
   }
   else
   {
      return(Insert(Wurzel, &((*Zweig)->Right), Key, Daten));
   }
}

BOOL BaumInsert(Baum *Wurzel, BaumKeyType Key, BaumDataType Daten)
{
   return(Insert(Wurzel, &(Wurzel->Daten), Key, Daten));
}
