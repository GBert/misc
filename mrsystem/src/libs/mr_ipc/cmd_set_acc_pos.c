#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdSetAccPos(MrIpcCmdType *Data, unsigned long Addr,
                       PositionType Position, unsigned int Current)
{
   MrIpcSetCommand(Data, MrIpcCmdAccSwitch);
   MrIpcSetIntLp1(Data, Addr);
   MrIpcSetIntIp1(Data, Position);
   MrIpcSetIntIp2(Data, Current);
}
