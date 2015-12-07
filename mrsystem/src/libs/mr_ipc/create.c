#include <stdlib.h>
#include "mr_ipc.h"

MrIpcCmdType *RpiIpcCreate(void)
{  MrIpcCmdType *NewIpcCmd;

   NewIpcCmd = (MrIpcCmdType *)malloc(sizeof(MrIpcCmdType));
   return NewIpcCmd;
}
