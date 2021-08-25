#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdSetStatusResponse(MrIpcCmdType *Data, unsigned long Addr,
                               unsigned int Index)
{
   if (Data != (MrIpcCmdType *)NULL)
   {
      MrIpcSetCommand(Data, MrIpcCmdStatusResponse);
      MrIpcSetIntLp1(Data, Addr);
      MrIpcSetIntIp1(Data, Index);
   }
}
