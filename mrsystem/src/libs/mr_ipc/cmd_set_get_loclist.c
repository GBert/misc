#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdSetReqestLoclist(MrIpcCmdType *Data)
{
   MrIpcSetCommand(Data, MrIpcCmdRequestLokListe);
}
