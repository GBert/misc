#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdSetTrackProto(MrIpcCmdType *Data, int Protokoll)
{
   MrIpcSetCommand(Data, MrIpcCmdTrackProto);
   MrIpcSetIntIp1(Data, Protokoll);
}
