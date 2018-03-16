#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdSetReqestLocname(MrIpcCmdType *Data, unsigned StartIdx, unsigned EndIdx)
{
   MrIpcSetCommand(Data, MrIpcCmdRequestLocName);
   MrIpcSetIntIp1(Data, StartIdx);
   MrIpcSetIntIp2(Data, EndIdx);
}
