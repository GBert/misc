#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecConfigQuery(MrCs2CanDataType *CanMsg, char *FileName)
{  int i;

   for (i = 0; i < 8; i++)
      FileName[i] = (char)(CanMsg->Data[i]);
   FileName[8] = '\0';
}
