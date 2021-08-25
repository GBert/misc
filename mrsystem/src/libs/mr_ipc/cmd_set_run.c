#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdSetRun(MrIpcCmdType *Data, SwitchType Switch)
{
   MrIpcSetCommand(Data, MrIpcCmdRun);
   MrIpcSetIntIp1(Data, Switch);
}
