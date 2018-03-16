#include <string.h>
#include <boolean.h>
#include "scanner.h"

BOOL ScanSuche(Scanner *Handle, char *Wen)
{  BOOL Ok;
   ScanSymbol sy;

   sy = Handle->Sym;
   while ((sy != MengeaufSy) && (sy != EofSy))
      sy = Scan(Handle);
   if (sy != EofSy)
   {
      if (Scan(Handle) == BezeichnerSy)
         ScanString(Handle, Wen);
      Ok = (Scan(Handle) == MengezuSy);
      Ok = (Ok && (Scan(Handle) == EolnSy));
   }
   else
      Ok = FALSE;
   return(Ok);
}
