#include <stddef.h>
#include <stdlib.h>
#include "avl.h"

static void DeleteKnoten(void *PrivData, AvlKnoten Element)
{  AvlBaum *Wurzel;

   Wurzel = (AvlBaum *)PrivData;
   if ((Wurzel->DestroyKey != NULL) && (Element->Key != NULL))
   {
      Wurzel->DestroyKey(Element->Key);
   }
   if ((Wurzel->DestroyDaten != NULL) && (Element->Daten != NULL))
   {
      Wurzel->DestroyDaten(Element->Daten);
   }
   free(Element);
}

static void WalkPostorder(AvlKnoten Zweig, AvlWalkCbFkt Cb, void *PrivData)
{
   if (Zweig != NULL)
   {
      WalkPostorder(Zweig->Link[0], Cb, PrivData);
      WalkPostorder(Zweig->Link[1], Cb, PrivData);
      Cb(PrivData, Zweig);
   }
}

void AvlDestroy(AvlBaum *Wurzel)
{
   if (Wurzel != NULL)
   {
      WalkPostorder(Wurzel->Daten, DeleteKnoten, (void *)Wurzel);
      free(Wurzel);
   }
}
