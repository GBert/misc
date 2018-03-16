#include <stddef.h>
#include <compare.h>
#include "baum.h"

static BaumKnoten Finde(Baum *Wurzel, BaumKnoten Zweig, BaumKeyType Key)
{
   if (Zweig == NULL)
   {
      return(NULL);
   }
   else if (SMALLER(Wurzel->Compare(Key, Zweig->Key)))
   {
      return(Finde(Wurzel, Zweig->Left, Key));
   }
   else if (GREATER(Wurzel->Compare(Key, Zweig->Key)))
   {
      return(Finde(Wurzel, Zweig->Right, Key));
   }
   else
   {
      return(Zweig);
   }
}

BaumKnoten BaumFinde(Baum *Wurzel, BaumKeyType Key)
{
   if (Wurzel->Daten != NULL)
   {
      return(Finde(Wurzel, Wurzel->Daten, Key));
   }
   else
   {
      return(NULL);
   }
}
