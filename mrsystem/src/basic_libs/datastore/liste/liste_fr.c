#include <boolean.h>
#include <compare.h>
#include "liste.h"

ListeKnoten ListeFirst(Liste *Wurzel)
{
   Wurzel->Aktuell = Wurzel->Daten;
   return(Wurzel->Aktuell);
}
