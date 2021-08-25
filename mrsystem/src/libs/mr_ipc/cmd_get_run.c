#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdGetRun(MrIpcCmdType *Data, SwitchType *Switch)
{
   if (Data != (MrIpcCmdType *)NULL)
   {
      *Switch = MrIpcGetIntIp1(Data);
   }
}
