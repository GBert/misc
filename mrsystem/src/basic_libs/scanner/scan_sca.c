#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <boolean.h>
#include "scanner.h"

#define LF                10
#define CR                13
#define EOLN              13
#define EOFI              26

static char NextChar(Scanner *Handle)
{
   if ((Handle->AktZeichen >= Handle->AnzZeichen) &&
       (Handle->ScanFile != (FILE *)NULL))
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
         if ((Handle->AktZeichen > Handle->AnzZeichen) &&
             (Handle->ScanFile != (FILE *)NULL))
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

static ScanSymbol Bezeichner(Scanner *Handle, char StartZeichen)
{  short k;
   char AktZeichen, schreibweise[SCAN_BLOCK_LAENGE];

   AktZeichen = StartZeichen;
   k = 0;
   while (isalnum(AktZeichen) || (AktZeichen == '_'))
   {
      if (k < SCAN_BLOCK_LAENGE)
      {
         k++;
         schreibweise[k-1] = AktZeichen;
         schreibweise[k] = 0;
      }
      AktZeichen = NextChar(Handle);
   }
   UngetChar(Handle, AktZeichen);
   strcpy(Handle->zk, schreibweise);
   k = 0;
   while ((k < Handle->NumKeywords) &&
          (strcmp(Handle->zk, Handle->Keywords[k].Keyword) != 0))
   {
      k++;
   }
   if (k < Handle->NumKeywords)
   {
      return(Handle->Keywords[k].Symbol);
   }
   else
   {
      return(BezeichnerSy);
   }
}

static ScanSymbol Zahl(Scanner *Handle, char StartZeichen)
{  char AktZeichen;

   AktZeichen = StartZeichen;
   Handle->i = 0;
   do {
      Handle->i = 10 * Handle->i + AktZeichen -'0';
      AktZeichen = NextChar(Handle);
   } while (isalnum(AktZeichen));
   UngetChar(Handle, AktZeichen);
   return(GzahlSy);
}

static ScanSymbol Zeichenkette(Scanner *Handle)
{  BOOL ende;
   short k;
   char AktZeichen;

   Handle->zk[0] = 0;
   k = 0;
   ende = FALSE;
   do {
      AktZeichen = NextChar(Handle);
      if (AktZeichen == '"')
      {
         AktZeichen = NextChar(Handle);
         if (AktZeichen != '"')
         {
            ende = TRUE;
            UngetChar(Handle, AktZeichen);
         }
         else
         {
            Handle->zk[k] = AktZeichen;
            Handle->zk[k+1] = 0;
            k++;
         }
      }
      else
      {
         Handle->zk[k] = AktZeichen;
         Handle->zk[k+1] = 0;
         k++;
      }
   } while (!ende);
   return(ZeichenkettenSy);
}

ScanSymbol Scan(Scanner *Handle)
{  ScanSymbol sy;
   char AktZeichen;

   sy = NoSy;
   do {
      AktZeichen = NextChar(Handle);
   } while (AktZeichen == ' ');
   if (isalpha(AktZeichen))
   {
      sy = Bezeichner(Handle, AktZeichen);
   }
   else if (isdigit(AktZeichen))
   {
      sy = Zahl(Handle, AktZeichen);
   }
   else
   {
      switch (AktZeichen)
      {
         case '<':
            AktZeichen = NextChar(Handle);
            if (AktZeichen == '>')
            {
               sy = UngleichSy;
            }
            else if (AktZeichen == '=')
            {
               sy = KleinergleichSy;
            }
            else
            {
               sy = KleinerSy;
               UngetChar(Handle, AktZeichen);
            }
            break;
         case '+':
            sy = PlusSy;
            break;
         case '*':
            sy = SternSy;
            break;
         case '-':
            sy = MinusSy;
            break;
         case '=':
            sy = GleichSy;
            break;
         case ',':
            sy = KommaSy;
            break;
         case '^':
            sy = PfeilSy;
            break;
         case '[':
            sy = MengeaufSy;
            break;
         case ']':
            sy = MengezuSy;
            break;
         case '"':
            sy = Zeichenkette(Handle);
            break;
         case '>':
            AktZeichen = NextChar(Handle);
            if (AktZeichen == '=')
            {
               sy = GroessergleichSy;
            }
            else
            {
               sy = GroesserSy;
               UngetChar(Handle, AktZeichen);
            }
            break;
         case '#':
            sy = HashSy;
            break;
         case ':':
            sy = DoppelpunktSy;
            break;
         case ';':
            sy = SemicolonSy;
            break;
         case '/':
            sy = TeilSy;
            break;
         case '.':
            sy = PunktSy;
            break;
         case '(':
            sy = KlammeraufSy;
            break;
         case ')':
            sy = KlammerzuSy;
            break;
         default:
            if (AktZeichen == EOLN)
               sy = EolnSy;
            else
            {
               if (AktZeichen == EOFI)
                  sy = EofSy;
               else
                  sy = NoSy;
            }
            break;
      }
   }
   Handle->Sym = sy;
   return(sy);
}
