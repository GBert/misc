#ifndef CAN_IO_H
#define CAN_IO_H

#include <boolean.h>
#include <mr_can.h>

typedef struct {
   void *private;
   int (*GetFd)(void *private);
   BOOL (*Open)(void *private);
   void (*Close)(void *private);
   int (*Read)(void *private, CanFrameStruct *CanFrame);
   int (*Write)(void *private, CanFrameStruct *CanFrame);
} IoFktStruct;

#endif
