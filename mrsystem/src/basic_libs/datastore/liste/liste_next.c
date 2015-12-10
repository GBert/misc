#include <stddef.h>
#include <boolean.h>
#include <compare.h>
#include "liste.h"

ListeKnoten ListeNext(Liste *Wurzel)
{
   if (Wurzel->Aktuell == NULL)
      /* alle Elemente wurde schon durchgegangen */
      return(NULL);
   else
   {
      Wurzel->Aktuell = Wurzel->Aktuell->Next;
      return(Wurzel->Aktuell);
   }
}
