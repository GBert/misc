#include <stdlib.h>
#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include "lx_debug.h"

#define MAXFRAMES 20

static void PmdTrace(int signo)
{  void *Stack[MAXFRAMES];
   char **Symbols;
   int i, Count;

   Count = backtrace(Stack, MAXFRAMES);
   printf("Caught signal %d, Stacktrace:", signo);
   Symbols = backtrace_symbols(Stack, Count);
   puts("Dump:");
   for (i = 0; i<Count; i++)
   {
      printf("Frame %d: 0x%p (%s)\n", i+1, Stack[i], Symbols[i]);
   }
   free(Symbols);
}


static void PmdSigHandler(int signo)
{
   PmdTrace(signo);
   exit(signo);
}

void PmdInit(void)
{
   signal(SIGSEGV, PmdSigHandler);
   signal(SIGFPE, PmdSigHandler);
}
