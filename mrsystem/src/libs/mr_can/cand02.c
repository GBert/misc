#include <string.h>
#include <bitmask.h>
#include <bytestream.h>
#include "mr_can.h"

void MrCs2DecBind(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                  unsigned int *Sid)
{
   *Uid = GetLongFromByteArray((char *)CanMsg->Data);
   *Sid = GetIntFromByteArray((char *)&(CanMsg->Data[4]));
}
