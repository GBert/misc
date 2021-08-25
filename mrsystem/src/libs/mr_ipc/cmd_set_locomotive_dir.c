#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdSetLocomotiveDir(MrIpcCmdType *Data, unsigned long Addr, DirectionType Direction)
{
   MrIpcSetCommand(Data, MrIpcCmdLocomotiveDirection);
   MrIpcSetIntLp1(Data, Addr);
   MrIpcSetIntIp1(Data, Direction);
}
