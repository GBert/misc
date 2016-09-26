#include <stddef.h>
#include <stdlib.h>
#include <dliste.h>
#include "queue.h"

void QueueDestroy(Queue *Wurzel)
{
   DlisteDestroy(Wurzel->QueueDaten);
   free(Wurzel);
}
