#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include "mr_cs2ms2.h"

void MrEthCs2Send(int Socket, char *Data)
{
   send(Socket, Data, MR_CS2_UDP_LENGTH, 0);
}
