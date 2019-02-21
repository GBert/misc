#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdSetMesswertRequest(MrIpcCmdType *Data, unsigned long Addr,
                                unsigned int Kanalnummer)
{
   if (Data != (MrIpcCmdType *)NULL)
   {
      MrIpcSetCommand(Data, MrIpcCmdRequestMesswert);
      MrIpcSetIntLp1(Data, Addr);
      MrIpcSetIntIp1(Data, Kanalnummer);
   }
}
