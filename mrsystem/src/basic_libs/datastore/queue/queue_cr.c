#include <stddef.h>
#include <stdlib.h>
#include <dliste.h>
#include "queue.h"

Queue *QueueCreate(void)
{  Queue *NewQueue;

   NewQueue = (Queue *)malloc(sizeof(Queue));
   if (NewQueue != NULL)
   {
      NewQueue->QueueDaten = DlisteCreate();
      if (NewQueue->QueueDaten == NULL)
      {
         free(NewQueue);
         NewQueue = NULL;
      }
   }
   return(NewQueue);
}
