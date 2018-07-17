#ifndef CAN_IO_H
#define CAN_IO_H

#include <boolean.h>
#include <mr_can.h>

#define IOFKT_INVALID_FD -1

typedef struct {
   void *private;
   int (*GetFd)(void *private);
   BOOL (*Open)(void *private);
   void (*Close)(void *private);
   BOOL (*Read)(void *private, int fd, BOOL PendingData,
                MrCs2CanDataType *CanMsg);
   BOOL (*Write)(void *private, int ReceiverSocket, MrCs2CanDataType *CanMsg);
} IoFktStruct;

#endif
