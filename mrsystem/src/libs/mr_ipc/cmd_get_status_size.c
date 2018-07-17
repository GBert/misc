#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdGetStatusPos(MrIpcCmdType *Data, unsigned long *Addr,
                          unsigned int *Index, unsigned int *NumPackets)
{
   if (Data != (MrIpcCmdType *)NULL)
   {
      *Addr = MrIpcGetIntLp1(Data);
      *Index = MrIpcGetIntIp1(Data);
      *NumPackets = MrIpcGetIntIp2(Data);
   }
}
