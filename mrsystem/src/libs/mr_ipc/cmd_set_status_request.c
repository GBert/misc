#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdSetStatusRequest(MrIpcCmdType *Data, unsigned long Addr,
                              unsigned int Index)
{
   if (Data != (MrIpcCmdType *)NULL)
   {
      MrIpcSetCommand(Data, MrIpcCmdStatusRequest);
      MrIpcSetIntLp1(Data, Addr);
      MrIpcSetIntIp1(Data, Index);
   }
}
