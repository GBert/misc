#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdGetQuery(MrIpcCmdType *Data, char *Name)
{  int i;

   if (Data != (MrIpcCmdType *)NULL)
   {
      for (i = 0; i < MR_CS2_NUM_CAN_BYTES; i++)
         Name[i] = MrIpcGetRawDataI(Data,i);
      Name[MR_CS2_NUM_CAN_BYTES] = '\0';
   }
   else
   {
      Name[0] = '\0';
   }
}
