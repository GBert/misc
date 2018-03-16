#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdGetLocomotiveDir(MrIpcCmdType *Data, unsigned long *Addr, DirectionType *Direction)
{
   if (Data != (MrIpcCmdType *)NULL)
   {
      *Addr = MrIpcGetIntLp1(Data);
      *Direction = MrIpcGetIntIp1(Data);
   }
}
