#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdSetLocomotiveSpeed(MrIpcCmdType *Data, unsigned long Addr, unsigned Speed)
{
   MrIpcSetCommand(Data, MrIpcCmdLocomotiveSpeed);
   MrIpcSetIntLp1(Data, Addr);
   MrIpcSetIntIp1(Data, Speed);
}
