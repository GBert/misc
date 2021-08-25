#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"

void ScanExit(Scanner *Handle)
{
   if (Handle->ScanFile != (FILE *)NULL)
   {
      fclose(Handle->ScanFile);
      Handle->AktZeichen = ' ';
      Handle->AnzZeichen = 0;
      Handle->ScanFile = (FILE *)NULL;
      Handle->Sym = NoSy;
      Handle->NumKeywords = 0;
      if (Handle->Keywords != (ScanKeyword *)NULL)
      {
         free(Handle->Keywords);
         Handle->Keywords = (ScanKeyword *)NULL;
      }
   }
}
