#include <stddef.h>
#include <string.h>
#include "mr_ipc.h"

void MrIpcCmdGetReqestLocinfo(MrIpcCmdType *Data, char *Locname)
{
   if (Data != (MrIpcCmdType *)NULL)
   {
      strcpy(Locname, (char *)MrIpcGetRawData(Data));
   }
}
