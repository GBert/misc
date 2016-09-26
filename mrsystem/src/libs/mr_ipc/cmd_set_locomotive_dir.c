#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdSetLocomotiveDir(MrIpcCmdType *Data, unsigned long Addr, DirectionType Direction)
{
   MrIpcSetCommand(Data, MrIpcCmdLocomotiveDirection);
   MrIpcSetIntLp1(Data, Addr);
   MrIpcSetIntLp1(Data, Direction);
}
