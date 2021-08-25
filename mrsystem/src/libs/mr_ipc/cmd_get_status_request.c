#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdGetStatusRequest(MrIpcCmdType *Data, unsigned long *Addr,
                              unsigned int *Index)
{
   if (Data != (MrIpcCmdType *)NULL)
   {
      *Addr = MrIpcGetIntLp1(Data);
      *Index = MrIpcGetIntIp1(Data);
   }
}
