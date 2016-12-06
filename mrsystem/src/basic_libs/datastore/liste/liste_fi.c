#include <stddef.h>
#include <boolean.h>
#include <compare.h>
#include "liste.h"

ListeKnoten ListeFinde(Liste *Wurzel, ListeKeyType Key)
{  ListeKnoten WorkPtr;
   BOOL gefunden;

   if (Wurzel->Daten == NULL)
      /* die Liste ist leer */
      return(NULL);
   else
   {
      gefunden = FALSE;
      WorkPtr = Wurzel->Daten;
      while ((WorkPtr != NULL) && !gefunden)
      {
         /* solange kein Listenende und nicht gefunden */
         if (Wurzel->Compare(WorkPtr->Key, Key) == 0)
            /* gefunden */
            gefunden = TRUE;
         else
            /* noch nicht gefunden -> weitersuchen */
            WorkPtr = WorkPtr->Next;
      }
      Wurzel->Aktuell = WorkPtr;
      return(WorkPtr);
   }
}
