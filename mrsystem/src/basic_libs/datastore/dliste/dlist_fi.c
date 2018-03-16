#include <stddef.h>
#include <boolean.h>
#include <compare.h>
#include "dliste.h"

DlisteKnoten DlisteFinde(Dliste *Wurzel, DlisteKeyType Key)
{  DlisteKnoten WorkPtr;
   BOOL gefunden;

   if (Wurzel->DatenAnfang == NULL)
   {
      /* die Liste ist leer */
      return(NULL);
   }
   else
   {
      gefunden = FALSE;
      WorkPtr = Wurzel->DatenAnfang;
      while ((WorkPtr != NULL) && !gefunden)
      {
         /* solange kein Listenende und noch nicht gefunden */
         if (EQUAL(Wurzel->Compare(WorkPtr->Key, Key)))
         {
            /* gefunden */
            gefunden = TRUE;
         }
         else
         {
            /* noch nicht gefunden -> weitersuchen */
            WorkPtr = WorkPtr->Next;
         }
      }
      return(WorkPtr);
   }
}
