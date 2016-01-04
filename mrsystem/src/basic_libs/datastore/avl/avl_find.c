#include <stddef.h>
#include <stdlib.h>
#include <compare.h>
#include "avl.h"

#ifdef REKURSIV
/* rekursive Loesung */
static AvlKnoten Finde(AvlBaum *Wurzel, AvlKnoten tree, AvlKeyType Key)
{
   if (SMALLER(Wurzel->Compare(Key, tree->key)))
   {
      if (tree->Link[0] != NULL)
      {
         return(Finde(Wurzel, tree->Link[0], Key));
      }
      else
      {
         return(NULL);
      }
   }
   else if (GREATER(Wurzel->Compare(Key, tree->key)))
   {
      if (tree->Link[1] != NULL)
      {
         return(Finde(Wurzel, tree->Link[1], Key));
      }
      else
      {
         return(NULL);
      }
   }
   else
   {
      return(tree);
   }
}
#endif

AvlKnoten AvlFinde(AvlBaum *Wurzel, AvlKeyType Key)
{
#ifdef REKURSIV
   /* rekursive Loesung */
   return(Finde(Wurzel, Wurzel->Daten, Key));
#else
   AvlKnoten PresentPtr;

   /* iterativ, schneller als rekursiv */
   PresentPtr = Wurzel->Daten;
   while ((PresentPtr != NULL) && (!EQUAL(Wurzel->Compare(PresentPtr->Key, Key))))
   {
      if (SMALLER(Wurzel->Compare(Key, PresentPtr->Key)))
         PresentPtr = PresentPtr->Link[0];
      else if (GREATER(Wurzel->Compare(Key, PresentPtr->Key)))
         PresentPtr = PresentPtr->Link[1];
   }
   return(PresentPtr);
#endif
}
