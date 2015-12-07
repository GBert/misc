#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/can.h>
#include "mr_can.h"

void SendMagicStart60113Frame(int CanSocket, int Verbose)
{  struct can_frame CanFrame;

   CanFrame.can_id = 0x360301UL;
   /* use EFF */
   CanFrame.can_id &= CAN_EFF_MASK;
   CanFrame.can_id |= CAN_EFF_FLAG;
   CanFrame.can_dlc = 5;
   CanFrame.data[0] = 0;
   CanFrame.data[1] = 0;
   CanFrame.data[2] = 0;
   CanFrame.data[3] = 0;
   CanFrame.data[4] = 0x11;
   if (write(CanSocket, &CanFrame, sizeof(CanFrame)) != sizeof(CanFrame))
   {
      perror("CAN magic 60113 start write __");
   }
   else
   {
      if (Verbose)
         puts("CAN magic 60113 start write");
   }
}
