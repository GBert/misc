#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdSetSetConfig(MrIpcCmdType *Data, unsigned long Addr,
                          unsigned int Kanalnummer, unsigned int Value)
{
   if (Data != (MrIpcCmdType *)NULL)
   {
      MrIpcSetCommand(Data, MrIpcCmdSetConfig);
      MrIpcSetIntLp1(Data, Addr);
      MrIpcSetIntIp1(Data, Kanalnummer);
      MrIpcSetIntIp2(Data, Value);
   }
}
