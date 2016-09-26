#include <stddef.h>
#include <stdlib.h>
#include "scanner.h"

Scanner *ScanCreate(void)
{  Scanner *NewScanner;

   NewScanner=malloc(sizeof(Scanner));
   if (NewScanner!=NULL)
   {
      NewScanner->AktZeichen = ' ';
      NewScanner->AnzZeichen = 0;
      NewScanner->ScanFile = (FILE *)NULL;
      NewScanner->Sym = NoSy;
      NewScanner->NumKeywords = 0;
      NewScanner->Keywords = (ScanKeyword *)NULL;
   }
   return(NewScanner);
}
