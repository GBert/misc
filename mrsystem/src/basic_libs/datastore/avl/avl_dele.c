#include <stddef.h>
#include <stdlib.h>
#include "avl.h"

#define AVL_MAX_HEIGHT 32

void AvlDelete(AvlBaum *Wurzel, AvlKeyType Key)
{
   /* Uses my Algorithm D, which can be found at
      http://www.msu.edu/user/pfaffben/avl.  Algorithm D is based on
      Knuth's Algorithm 6.2.2D (Tree deletion) and 6.2.3A (Balanced
      tree search and insertion), as well as the notes on pages 465-466
      of Vol. 3. */

   /* D1. */
   AvlElement Dummy;
   AvlKnoten pa[AVL_MAX_HEIGHT]; /* Stack P: Nodes. */
   short a[AVL_MAX_HEIGHT];      /* Stack P: Bits. */
   int k;                        /* Stack P: Pointer. */
   AvlKnoten *q, p, r, s;
   int l;
   AvlKnoten ReturnValue;

   /* insert dummy entry because access to k-1 is used */
   /* and this can generate access to pa[0], but we dont */
   /* want to modify any node in the tree */
   a[0] = 0;
   pa[0] = &Dummy;
   Dummy.Key = NULL;
   Dummy.Link[0] = NULL;
   Dummy.Link[1] = NULL;
   p = Wurzel->Daten;
   k = 1;
   for (;;)
   {
      /* D2. */
      if (p == NULL)
         return;
      if (EQUAL(Wurzel->Compare(Key, p->Key)))
         break;
      /* D3, D4. */
      pa[k] = p;
      if (SMALLER(Wurzel->Compare(Key, p->Key)))
      {
         p = p->Link[0];
         a[k] = 0;
      }
      else if (GREATER(Wurzel->Compare(Key, p->Key)))
      {
         p = p->Link[1];
         a[k] = 1;
      }
      k++;
   }
   /* D5. */
   q = &(pa[k - 1]->Link[a[k - 1]]);
   if (p->Link[1] == NULL)
   {
      *q = p->Link[0];
      if (*q)
         (*q)->Bal = 0;
   }
   else
   {
      /* D6. */
      r = p->Link[1];
      if (r->Link[0] == NULL)
      {
         r->Link[0] = p->Link[0];
         *q = r;
         r->Bal = p->Bal;
         a[k] = 1;
         pa[k++] = r;
      }
      else
      {
         /* D7. */
         s = r->Link[0];
         l = k++;
         a[k] = 0;
         pa[k++] = r;
         /* D8. */
         while (s->Link[0] != NULL)
         {
            r = s;
            s = r->Link[0];
            a[k] = 0;
            pa[k++] = r;
         }
         /* D9. */
         a[l] = 1;
         pa[l] = s;
         s->Link[0] = p->Link[0];
         r->Link[0] = s->Link[1];
         s->Link[1] = p->Link[1];
         s->Bal = p->Bal;
         *q = s;
      }
   }
   if (p == Wurzel->Daten)
   {
      /* Michael Bernstein: delete tree, we had to set new tree */
      Wurzel->Daten = *q;
   }
   /* Michael Bernstein: else tree is unchanged */
   ReturnValue = p;
   /* D10. */
   while (--k)
   {
      s = pa[k];
      if (a[k] == 0)
      {
         /* D10. */
         if (s->Bal == -1)
         {
            s->Bal = 0;
            continue;
         }
         else if (s->Bal == 0)
         {
            s->Bal = 1;
            break;
         }
         r = s->Link[1];
         if (r->Bal == 0)
         {
            /* D11. */
            s->Link[1] = r->Link[0];
            r->Link[0] = s;
            r->Bal = -1;
            pa[k - 1]->Link[a[k - 1]] = r;
            if (Wurzel->Daten == s)
            {
               /* Michael Bernstein: if k == 1 we have to set new root into tree */
               Wurzel->Daten = r;
            }
            break;
         }
         else if (r->Bal == 1)
         {
            /* D12. */
            s->Link[1] = r->Link[0];
            r->Link[0] = s;
            s->Bal = r->Bal = 0;
            pa[k - 1]->Link[a[k - 1]] = r;
            if (Wurzel->Daten == s)
            {
               /* Michael Bernstein: if k == 1 we have to set new root into tree */
               Wurzel->Daten = r;
            }
         }
         else
         {
            /* D13. */
            p = r->Link[0];
            r->Link[0] = p->Link[1];
            p->Link[1] = r;
            s->Link[1] = p->Link[0];
            p->Link[0] = s;
            if (p->Bal == 1)
            {
               s->Bal = -1;
               r->Bal = 0;
            }
            else if (p->Bal == 0)
               s->Bal = r->Bal = 0;
            else
            {
               s->Bal = 0;
               r->Bal = 1;
            }
            p->Bal = 0;
            pa[k - 1]->Link[a[k - 1]] = p;
            if (Wurzel->Daten == s)
            {
               /* Michael Bernstein: if k == 1 we have to set new root into tree */
               Wurzel->Daten = p;
            }
         }
      }
      else
      {
         /* D10. */
         if (s->Bal == 1)
         {
            s->Bal = 0;
            continue;
         }
         else if (s->Bal == 0)
         {
            s->Bal = -1;
            break;
         }
         r = s->Link[0];
         if (r->Bal == 0)
         {
            /* D11. */
            s->Link[0] = r->Link[1];
            r->Link[1] = s;
            r->Bal = 1;
            pa[k - 1]->Link[a[k - 1]] = r;
            if (Wurzel->Daten == s)
            {
               /* Michael Bernstein: if k == 1 we have to set new root into tree */
               Wurzel->Daten = r;
            }
            break;
         }
         else if (r->Bal == -1)
         {
            /* D12. */
            s->Link[0] = r->Link[1];
            r->Link[1] = s;
            s->Bal = r->Bal = 0;
            pa[k - 1]->Link[a[k - 1]] = r;
            if (Wurzel->Daten == s)
            {
               /* Michael Bernstein: if k == 1 we have to set new root into tree */
               Wurzel->Daten = r;
            }
         }
         else if (r->Bal == 1)
         {
            /* D13. */
            p = r->Link[1];
            r->Link[1] = p->Link[0];
            p->Link[0] = r;
            s->Link[0] = p->Link[1];
            p->Link[1] = s;
            if (p->Bal == -1)
            {
               s->Bal = 1;
               r->Bal = 0;
            }
            else if (p->Bal == 0)
               s->Bal = r->Bal = 0;
            else
            {
               s->Bal = 0;
               r->Bal = -1;
            }
            p->Bal = 0;
            pa[k - 1]->Link[a[k - 1]] = p;
            if (Wurzel->Daten == s)
            {
               /* Michael Bernstein: if k == 1 we have to set new root into tree */
               Wurzel->Daten = p;
            }
         }
      }
   }
   if (ReturnValue != NULL)
   {
      if ((Wurzel->DestroyKey != NULL) && (ReturnValue->Key != NULL))
      {
         Wurzel->DestroyKey(ReturnValue->Key);
      }
      if ((Wurzel->DestroyDaten != NULL) && (ReturnValue->Daten != NULL))
      {
         Wurzel->DestroyDaten(ReturnValue->Daten);
      }
      free(ReturnValue);
   }
}
