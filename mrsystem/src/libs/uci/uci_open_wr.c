#include <stdio.h>
#include <string.h>
#include <alloca.h>
#include <sys/stat.h>
#include "uci.h"


FILE *UciOpenWr(char *Pfad, char *BaseName, char *Extension)
{  char *FullName;
   FILE *FilePointer;

   FullName = (char *)alloca(strlen(Pfad) +
                             strlen(BaseName) +
                             strlen(Extension) +
                             2 /* / & EOS */ );
   if (FullName != (char *)NULL)
   {
      strcpy(FullName, Pfad);
      if (FullName[strlen(FullName)] != '/')
      {
         strcat(FullName, "/");
      }
      strcat(FullName, BaseName);
      strcat(FullName, Extension);
      FilePointer = fopen(FullName, "w");
      fchmod(fileno(FilePointer),
             S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
   }
   else
   {
      FilePointer = (FILE *)NULL;
   }
   return(FilePointer);
}
