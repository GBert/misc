#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdGetLocomotiveFkt(MrIpcCmdType *Data, unsigned long *Addr, unsigned *Function, SwitchType *Switch)
{
   if (Data != (MrIpcCmdType *)NULL)
   {
      *Addr = MrIpcGetIntLp1(Data);
      *Function = MrIpcGetIntIp1(Data);
      *Switch = MrIpcGetIntIp2(Data);
   }
}
