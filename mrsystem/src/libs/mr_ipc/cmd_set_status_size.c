#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdSetStatusPos(MrIpcCmdType *Data, unsigned long Addr,
                          unsigned int Index, unsigned int NumPackets)
{
   if (Data != (MrIpcCmdType *)NULL)
   {
      MrIpcSetCommand(Data, MrIpcCmdStatusSize);
      MrIpcSetIntLp1(Data, Addr);
      MrIpcSetIntIp1(Data, Index);
      MrIpcSetIntIp2(Data, NumPackets);
   }
}
