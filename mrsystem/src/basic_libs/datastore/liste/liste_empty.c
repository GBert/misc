#include <stddef.h>
#include <boolean.h>
#include "liste.h"

BOOL ListeIsEmpty(Liste *Wurzel)
{
   return(Wurzel->Daten == NULL);
}
