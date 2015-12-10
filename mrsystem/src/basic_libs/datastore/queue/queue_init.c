#include <stddef.h>
#include <dliste.h>
#include "queue.h"

void QueueInit(Queue *Wurzel, QueueDelCbFkt Cb)
{
   DlisteInit(Wurzel->QueueDaten,
              (CmpFkt)NULL,
              (DlisteKeyDelCbFkt)NULL,
              (DlisteDataDelCbFkt)Cb);
}
