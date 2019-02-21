#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdGetMesswertRequest(MrIpcCmdType *Data, unsigned long *Addr,
                                unsigned int *Kanalnummer)
{
   if (Data != (MrIpcCmdType *)NULL)
   {
      *Addr = MrIpcGetIntLp1(Data);
      *Kanalnummer = MrIpcGetIntIp1(Data);
   }
}
