#include <stddef.h>
#include "mr_ipc.h"

void MrIpcCmdSetQuery(MrIpcCmdType *Data, unsigned char Dlc, char *Name)
{  int i;

   MrIpcSetCommand(Data, MrIpcCmdRequestFile);
   MrIpcSetRawDlc(Data, Dlc);
   for (i = 0; i < MR_CS2_NUM_CAN_BYTES; i++)
      MrIpcSetRawDataI(Data, i, Name[i]);
}
