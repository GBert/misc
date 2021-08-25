#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdSetCfgHeader(MrIpcCmdType *Data, unsigned long Length,
                          unsigned Crc)
{
   MrIpcSetCommand(Data, MrIpcCmdCfgHeader);
   MrIpcSetIntLp1(Data, Length);
   MrIpcSetIntIp1(Data, Crc);
}
