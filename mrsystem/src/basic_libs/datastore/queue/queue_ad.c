#include <stddef.h>
#include <boolean.h>
#include <dliste.h>
#include "queue.h"

BOOL QueueAdd(Queue *Wurzel, QueueDataType Daten)
{
   return(DlisteAhead(Wurzel->QueueDaten, (DlisteKeyType)NULL, (DlisteDataType)Daten));
}
