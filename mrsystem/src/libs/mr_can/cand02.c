#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecBind(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                  unsigned int *Sid)
{
   *Uid = GetLongFromByteArray(CanMsg->Data);
   *Sid = GetIntFromByteArray(&(CanMsg->Data[4]));
}
