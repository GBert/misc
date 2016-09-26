#include <stddef.h>
#include <stdlib.h>
#include <compare.h>
#include "avl.h"

AvlBaum *AvlCreate(void)
{  AvlBaum *Wurzel;

   Wurzel = (AvlBaum *)malloc(sizeof(AvlBaum));
   if (Wurzel != NULL)
   {
      Wurzel->Daten = NULL;
      Wurzel->Compare = NULL;
      Wurzel->DestroyKey = NULL;
      Wurzel->DestroyDaten = NULL;
   }
   return(Wurzel);
}
