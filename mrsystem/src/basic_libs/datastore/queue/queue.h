#ifndef QUEUE_H
#define QUEUE_H

#include <boolean.h>
#include <dliste.h>

typedef void *QueueDataType;
typedef void (*QueueDelCbFkt)(QueueDataType Data);
typedef struct {
   Dliste *QueueDaten;
} Queue;

Queue *QueueCreate(void);
void QueueDestroy(Queue *Wurzel);
void QueueInit(Queue *Wurzel, QueueDelCbFkt Cb);
BOOL QueueIsEmpty(Queue *Wurzel);
BOOL QueueAdd(Queue *Wurzel, QueueDataType Daten);
QueueDataType QueueGet(Queue *Wurzel);

#endif
