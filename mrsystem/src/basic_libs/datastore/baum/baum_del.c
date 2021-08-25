#include <stddef.h>
#include <stdlib.h>
#include <compare.h>
#include "baum.h"

static BaumKnoten PutOut(BaumKnoten *Old)
{  BaumKnoten CopyOld, DaddyMaxLeft, MaxLeft;

   CopyOld = *Old;
   if (CopyOld->Right == NULL)
      *Old = CopyOld->Left;
   else if (CopyOld->Left == NULL)
      *Old = CopyOld->Right;
   else
   {
      DaddyMaxLeft = CopyOld->Right;
      if (DaddyMaxLeft->Left == NULL)
      {
         DaddyMaxLeft->Left = CopyOld->Left;
         *Old = DaddyMaxLeft;
      }
      else
      {
         MaxLeft = DaddyMaxLeft->Left;
         while (MaxLeft->Left != NULL)
         {
            DaddyMaxLeft = MaxLeft;
            MaxLeft = DaddyMaxLeft->Left;
         }
         MaxLeft->Left = CopyOld->Left;
         DaddyMaxLeft->Left = MaxLeft->Right;
         MaxLeft->Right = CopyOld->Right;
         *Old = MaxLeft;
      }
   }
   return(CopyOld);
}

static BaumKnoten Delete(Baum *Wurzel, BaumKnoten *Zweig, BaumKeyType Key)
{
   if (*Zweig != NULL)
   {
      if (SMALLER(Wurzel->Compare(Key, (*Zweig)->Key)))
         return(Delete(Wurzel, &((*Zweig)->Left), Key));
      else if (GREATER(Wurzel->Compare(Key, (*Zweig)->Key)))
         return(Delete(Wurzel, &((*Zweig)->Right), Key));
      else
         return(PutOut(Zweig));
   }
   else
   {
      return(NULL);
   }
}

void BaumDelete(Baum *Wurzel, BaumKeyType Key)
{  BaumKnoten ReturnPtr;

   if (Wurzel->Daten != NULL)
   {
      ReturnPtr = Delete(Wurzel, &(Wurzel->Daten), Key);
      if (ReturnPtr != NULL)
      {
         if ((Wurzel->DestroyKey != NULL) && (ReturnPtr->Key != NULL))
         {
            Wurzel->DestroyKey(ReturnPtr->Key);
         }
         if ((Wurzel->DestroyDaten != NULL) && (ReturnPtr->Daten != NULL))
         {
            Wurzel->DestroyDaten(ReturnPtr->Daten);
         }
         free(ReturnPtr);
      }
   }
}
