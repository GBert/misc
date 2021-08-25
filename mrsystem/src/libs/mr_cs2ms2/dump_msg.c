#include <stdlib.h>
#include <stdio.h>
#include "mr_cs2ms2.h"

void MrCs2DumpCanMsg(MrCs2CanDataType *CanMsg, char *Info)
{  int i;

   printf("%s: id 0x%lx, dlc %d\n    ",
          Info, MrCs2GetId(CanMsg), MrCs2GetDlc(CanMsg));
   for (i = 0; i < 8; i++)
      printf("0x%02x ", CanMsg->Data[i]);
   printf("\n    hash 0x%x resp 0x%x cmd 0x%x prio 0x%x\n",
          MrCs2GetHash(CanMsg), MrCs2GetResponse(CanMsg),
          MrCs2GetCommand(CanMsg), MrCs2GetPrio(CanMsg));
}
