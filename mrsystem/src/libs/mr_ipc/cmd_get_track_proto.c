#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdGetTrackProto(MrIpcCmdType *Data, unsigned *Protokoll)
{
   if (Data != (MrIpcCmdType *)NULL)
   {
      *Protokoll = MrIpcGetIntIp1(Data);
   }
}
