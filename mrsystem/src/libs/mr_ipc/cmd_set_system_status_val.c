#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdSetSystemStatusVal(MrIpcCmdType *Data, unsigned long Addr, unsigned int Channel, unsigned int Value)
{
   MrIpcSetCommand(Data, MrIpcCmdSystemStatusVal);
   MrIpcSetIntLp1(Data, Addr);
   MrIpcSetIntIp1(Data, Channel);
   MrIpcSetIntIp2(Data, Value);
}
