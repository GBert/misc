#include <stddef.h>
#include "mr_ipc.h"

void MrIpcInit(MrIpcCmdType *Data)
{
   if (Data != (MrIpcCmdType *)NULL)
   {
      MrIpcSetCommand(Data, MrIpcCmdNull);
   }
}
