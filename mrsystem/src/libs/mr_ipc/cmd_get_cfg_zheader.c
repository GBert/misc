#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdGetCfgZHeader(MrIpcCmdType *Data, unsigned long *Length,
                           unsigned *Crc)
{
   if (Data != (MrIpcCmdType *)NULL)
   {
      *Length = MrIpcGetIntLp1(Data);
      *Crc = MrIpcGetIntIp1(Data);
   }
}
