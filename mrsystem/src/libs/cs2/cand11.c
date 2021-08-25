#include <bytestream.h>
#include "cs2.h"

void Cs2DecS88Event4(unsigned char *CanData, int *Device, int *Contact)
{
   *Device = GetIntFromByteArray(CanData);
   *Contact = GetIntFromByteArray(&(CanData[2]));
}

void Cs2DecS88Event5(unsigned char *CanData, int *Device, int *Contact,
                     int *Param)
{
   *Device = GetIntFromByteArray(CanData);
   *Contact = GetIntFromByteArray(&(CanData[2]));
   *Param = (int)(CanData[4]);
}

void Cs2DecS88Event8(unsigned char *CanData, int *Device, int *Contact,
                     int *StateOld, int *StateNew, int *Time)
{
   *Device = GetIntFromByteArray(CanData);
   *Contact = GetIntFromByteArray(&(CanData[2]));
   *StateOld = (int)(CanData[4]);
   *StateNew = (int)(CanData[5]);
   *Time = GetIntFromByteArray(&(CanData[6]));
}
