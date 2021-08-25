#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2EncConfigQuery0(MrCs2CanDataType *CanMsg)
{
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_CONFIG_QUERY);
   MrCs2SetDlc(CanMsg, 0);
}

void MrCs2EncConfigQuery(MrCs2CanDataType *CanMsg, char *FileName)
{  int i;

   for (i = 0; i < 8; i++)
      CanMsg->Data[i] = (unsigned char)0;
   for (i = 0; i < strlen(FileName); i++)
      CanMsg->Data[i] = (unsigned char)(FileName[i]);
   MrCs2SetCommand(CanMsg, MR_CS2_CMD_CONFIG_QUERY);
   MrCs2SetDlc(CanMsg, 8);
}
