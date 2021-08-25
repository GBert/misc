#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdGetLocomotiveSpeed(MrIpcCmdType *Data, unsigned long *Addr, unsigned *Speed)
{
   if (Data != (MrIpcCmdType *)NULL)
   {
      *Addr = MrIpcGetIntLp1(Data);
      *Speed = MrIpcGetIntIp1(Data);
   }
}
