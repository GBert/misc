#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "mr_ipc.h"

void time_stamp(void)
{
   struct timeval tv;
   struct tm *tm;

   gettimeofday(&tv, NULL);
   tm = localtime(&tv.tv_sec);

   printf("%02d:%02d:%02d.%03d  ", tm->tm_hour, tm->tm_min, tm->tm_sec, (int)tv.tv_usec / 1000);
}

void puts_ts(char *s)
{
   time_stamp();
   puts(s);
}
