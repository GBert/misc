#include <boolean.h>
#include "scanner.h"

#define LF                10
#define CR                13
#define EOLN              13
#define EOFI              26

static char NextChar(Scanner *Handle)
{
   if (Handle->AktZeichen>=Handle->AnzZeichen)
   {
      Handle->AnzZeichen = (int)fread(Handle->Puffer, 1,
                                      SCAN_BLOCK_LAENGE,
                                      Handle->ScanFile);
      Handle->AktZeichen = 0;
   }
   if (Handle->AktZeichen < Handle->AnzZeichen)
   {
      Handle->AktZeichen = Handle->AktZeichen + 1;
      if (Handle->Puffer[Handle->AktZeichen - 1] == CR)
      {
         if (Handle->AktZeichen > Handle->AnzZeichen)
         {
            Handle->AnzZeichen = (int)fread(Handle->Puffer, 1,
                                            SCAN_BLOCK_LAENGE,
                                            Handle->ScanFile);
            Handle->AktZeichen = 0;
         }
         if ((Handle->AnzZeichen > 0) &&
             (Handle->AktZeichen < Handle->AnzZeichen) &&
             (Handle->Puffer[Handle->AktZeichen] == LF))
         {
            Handle->AktZeichen = Handle->AktZeichen + 1;
         }
         return(EOLN);
      }
      else if (Handle->Puffer[Handle->AktZeichen - 1] == LF)
      {
         return(EOLN);
      }
      else
      {
         return(Handle->Puffer[Handle->AktZeichen - 1]);
      }
   }
   else
   {
      return(EOFI);
   }
}

static void UngetChar(Scanner *Handle, char Zeichen)
{
   if (Handle->AktZeichen > 0)
   {
      Handle->AktZeichen = Handle->AktZeichen - 1;
   }
   else
   {
      memmove(&(Handle->Puffer[1]), &(Handle->Puffer[0]), SCAN_BLOCK_LAENGE);
      Handle->Puffer[0] = Zeichen;
      Handle->AnzZeichen = Handle->AnzZeichen + 1;
   }
}

ScanSymbol ScanEol(Scanner *Handle)
{  char c;
   int PosInToken;

   PosInToken = 0;
   c = NextChar(Handle);
   while ((c != EOLN) && (c != EOFI))
   {
      Handle->zk[PosInToken++] = c;
      c = NextChar(Handle);
   }
   Handle->zk[PosInToken++] = '\0';
   UngetChar(Handle, c);
   return(ZeichenkettenSy);
}
