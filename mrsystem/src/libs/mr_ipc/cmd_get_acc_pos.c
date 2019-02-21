#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdGetAccPos(MrIpcCmdType *Data, unsigned long *Addr,
                       PositionType *Position, unsigned int *Current)
{
   if (Data != (MrIpcCmdType *)NULL)
   {
      *Addr = MrIpcGetIntLp1(Data);
      *Position = MrIpcGetIntIp1(Data);
      *Current = MrIpcGetIntIp2(Data);
   }
}
