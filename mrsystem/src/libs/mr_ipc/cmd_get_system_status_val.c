#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdGetSystemStatusVal(MrIpcCmdType *Data, unsigned long *Addr, unsigned int *Channel, unsigned int *Value)
{
   if (Data != (MrIpcCmdType *)NULL)
   {
      *Addr = MrIpcGetIntLp1(Data);
      *Channel = MrIpcGetIntIp1(Data);
      *Value = MrIpcGetIntIp2(Data);
   }
}
