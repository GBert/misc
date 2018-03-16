#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdGetMember(MrIpcCmdType *Data, unsigned long *Addr, unsigned *Version, unsigned *Type)
{
   if (Data != (MrIpcCmdType *)NULL)
   {
      *Addr = MrIpcGetIntLp1(Data);
      *Version = MrIpcGetIntIp1(Data);
      *Type = MrIpcGetIntIp2(Data);
   }
}
