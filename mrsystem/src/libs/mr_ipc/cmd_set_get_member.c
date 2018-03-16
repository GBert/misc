#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdSetRequestMember(MrIpcCmdType *Data)
{
   MrIpcSetCommand(Data, MrIpcCmdRequestMember);
   MrIpcSetIntLp1(Data, 0l);
   MrIpcSetIntIp1(Data, 0);
   MrIpcSetIntIp2(Data, 0);
}
