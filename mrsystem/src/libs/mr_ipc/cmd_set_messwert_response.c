#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdSetMesswertResponse(MrIpcCmdType *Data, unsigned long Addr,
                                 unsigned int Kanalnummer, unsigned int Response)
{
   if (Data != (MrIpcCmdType *)NULL)
   {
      MrIpcSetCommand(Data, MrIpcCmdMesswertResponse);
      MrIpcSetIntLp1(Data, Addr);
      MrIpcSetIntIp1(Data, Kanalnummer);
      MrIpcSetIntIp2(Data, Response);
   }
}
