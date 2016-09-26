#include <stddef.h>
#include <stdlib.h>
#include "scanner.h"

void ScanDestroy(Scanner *Handle)
{
   if (Handle!=NULL)
   {
      if (Handle->ScanFile != (FILE *)NULL)
      {
         fclose(Handle->ScanFile);
      }
      if (Handle->Keywords != (ScanKeyword *)NULL)
         free(Handle->Keywords);
      free(Handle);
   }
}
