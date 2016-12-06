#include <stddef.h>
#include <stdlib.h>
#include <compare.h>
#include "liste.h"

void ListeDelete(Liste *Wurzel, ListeKeyType Key)
{  ListeKnoten WorkPtr, MerkPtr, ReturnPtr;

   ReturnPtr = NULL;
   if (Wurzel->Daten != NULL)
   {
      /* Die Liste enthaelt Elemente */
      if (EQUAL(Wurzel->Compare(Wurzel->Daten->Key, Key)))
      {
         /* Das erste element ist das gesuchte */
         WorkPtr = Wurzel->Daten;
         Wurzel->Daten = Wurzel->Daten->Next;
         ReturnPtr = WorkPtr;
      }
      else
      {
         /* Liste durchsuchen */
         WorkPtr = Wurzel->Daten;
         while ((WorkPtr->Next != NULL) && (ReturnPtr == NULL))
         {
            /* solange kein Listenende und nicht gefunden */
            if (Wurzel->Compare(WorkPtr->Next->Key, Key) == 0)
            {
               /* Nachfolder des aktuellen Elements ist das gesuchte */
               MerkPtr = WorkPtr->Next;
               WorkPtr->Next = MerkPtr->Next;
               ReturnPtr = MerkPtr;
            }
            else
               /* noch nicht gefunden -> weitersuchen */
               WorkPtr = WorkPtr->Next;
         }
      }
      if (ReturnPtr != NULL)
      {
         if (Wurzel->DestroyKey != NULL)
         {
            Wurzel->DestroyKey(ReturnPtr->Key);
         }
         if (Wurzel->DestroyDaten != NULL)
         {
            Wurzel->DestroyDaten(ReturnPtr->Data);
         }
         free(ReturnPtr);
      }
   }
}
