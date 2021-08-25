#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdGetConfigResponse(MrIpcCmdType *Data, unsigned long *Addr,
                               unsigned int *Kanalnummer, unsigned int *Response)
{
   if (Data != (MrIpcCmdType *)NULL)
   {
      *Addr = MrIpcGetIntLp1(Data);
      *Kanalnummer = MrIpcGetIntIp1(Data);
      *Response = MrIpcGetIntIp2(Data);
   }
}
