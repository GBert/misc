#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdSetCfgZHeader(MrIpcCmdType *Data, unsigned long Length,
                           unsigned Crc)
{
   MrIpcSetCommand(Data, MrIpcCmdCfgZHeader);
   MrIpcSetIntLp1(Data, Length);
   MrIpcSetIntIp1(Data, Crc);
}
