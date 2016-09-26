#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdGetReqestLocname(MrIpcCmdType *Data, unsigned *StartIdx, unsigned *EndIdx)
{
   if (Data != (MrIpcCmdType *)NULL)
   {
      *StartIdx = MrIpcGetIntIp1(Data);
      *EndIdx = MrIpcGetIntIp2(Data);
   }
}
