#include <stddef.h>
#include <stdlib.h>
#include "baum.h"

static void DeleteKnoten(void *PrivData, BaumKnoten Element)
{  Baum *Wurzel;

   Wurzel = (Baum *)PrivData;
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

void BaumDestroy(Baum *Wurzel)
{
   if (Wurzel != NULL)
   {
      BaumWalkPostorder(Wurzel, DeleteKnoten, (void *)Wurzel);
      free(Wurzel);
   }
}
