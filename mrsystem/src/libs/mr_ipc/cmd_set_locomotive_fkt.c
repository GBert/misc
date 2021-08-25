#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdSetLocomotiveFkt(MrIpcCmdType *Data, unsigned long Addr, unsigned Function, SwitchType Switch)
{
   MrIpcSetCommand(Data, MrIpcCmdLocomotiveFunction);
   MrIpcSetIntLp1(Data, Addr);
   MrIpcSetIntIp1(Data, Function);
   MrIpcSetIntIp2(Data, Switch);
}
