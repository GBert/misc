#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdGetSetConfig(MrIpcCmdType *Data, unsigned long *Addr,
                          unsigned int *Kanalnummer, unsigned int *Value)
{
   if (Data != (MrIpcCmdType *)NULL)
   {
      *Addr = MrIpcGetIntLp1(Data);
      *Kanalnummer = MrIpcGetIntIp1(Data);
      *Value = MrIpcGetIntIp2(Data);
   }
}
