#include <stddef.h>
#include <stdlib.h>
#include <boolean.h>
#include <compare.h>
#include "avl.h"

BOOL AvlInsert(AvlBaum *Wurzel, AvlKeyType Key, AvlDataType Daten)
{
   /* Uses Knuth's Algorithm 6.2.3A (balanced tree search and
      insertion), but caches results of comparisons. In empirical
      tests this eliminates about 25% of the comparisons seen under
      random insertions. */

   /* A1. */
   AvlKnoten t, s, p, q, r, this;

   this = malloc(sizeof(AvlElement));
   if (this != NULL)
   {
      this->Key = Key;
      this->Daten = Daten;
      this->Bal = 0;
      this->Link[0] = NULL;
      this->Link[1] = NULL;
      if (Wurzel->Daten == NULL)
         Wurzel->Daten = this;
      else
      {
         t = Wurzel->Daten;
         s = p = Wurzel->Daten;
         for (;;)
         {
            /* A2. */
            /* compare, included in  A3, Michael Bernstein */
            /* A3. */
            if (SMALLER(Wurzel->Compare(Key, p->Key)))
            {
               p->cache = 0;
               q = p->Link[0];
               if (q == NULL)
               {
                  p->Link[0] = q = this;
                  break;
               }
            }
            /* A4. */
            else if (GREATER(Wurzel->Compare(Key, p->Key)))
            {
               p->cache = 1;
               q = p->Link[1];
               if (q == NULL)
               {
                  p->Link[1] = q = this;
                  break;
               }
            }
            /* A3, A4. */
            if (q->Bal != 0)
            {
               t = p;
               s = q;
            }
            p = q;
         }
         /* A5. */
         this->Link[0] = NULL;
         this->Link[1] = NULL;
         /* A6. */
         r = p = s->Link[s->cache];
         while (p != q)
         {
            p->Bal = p->cache * 2 - 1;
            p = p->Link[p->cache];
         }
         /* A7. */
         if (s->cache == 0)
         {
            /* a = -1. */
            if (s->Bal == 0)
            {
               s->Bal = -1;
               return(TRUE);
            }
            else if (s->Bal == 1)
            {
               s->Bal = 0;
               return(TRUE);
            }
            if (r->Bal == -1)
            {
               /* A8. */
               p = r;
               s->Link[0] = r->Link[1];
               r->Link[1] = s;
               s->Bal = r->Bal = 0;
            }
            else
            {
               /* A9. */
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
            }
         }
         else
         {
            /* a == +1. */
            if (s->Bal == 0)
            {
               s->Bal = 1;
               return(TRUE);
            }
            else if (s->Bal == -1)
            {
               s->Bal = 0;
               return(TRUE);
            }
            if (r->Bal == 1)
            {
               /* A8. */
               p = r;
               s->Link[1] = r->Link[0];
               r->Link[0] = s;
               s->Bal = r->Bal = 0;
            }
            else
            {
               /* A9. */
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
            }
         }
         /* A10. */
         /*if (t != Wurzel->Daten && s == t->Left)*/
         if (s == t->Link[1])
            t->Link[1] = p;
         else if (s == t->Link[0])
            t->Link[0] = p;
         else
            Wurzel->Daten = p;
      }
      return(TRUE);
   }
   else
   {
      return(FALSE);
   }
}
