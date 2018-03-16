#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdSetMember(MrIpcCmdType *Data, unsigned long Addr, unsigned Version, unsigned Type)
{
   MrIpcSetCommand(Data, MrIpcCmdMember);
   MrIpcSetIntLp1(Data, Addr);
   MrIpcSetIntIp1(Data, Version);
   MrIpcSetIntIp2(Data, Type);
}
