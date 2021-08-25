#include <boolean.h>
#include <dliste.h>
#include "queue.h"

BOOL QueueIsEmpty(Queue *Wurzel)
{
   return(DlisteIsEmpty(Wurzel->QueueDaten));
}
