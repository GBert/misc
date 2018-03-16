#include <stdlib.h>
#include <stddef.h>
#include "mr_ipc.h"

void MrIpcDestroy(MrIpcCmdType *Data)
{
   if (Data != (MrIpcCmdType *)NULL)
   {
      free(Data);
   }
}
