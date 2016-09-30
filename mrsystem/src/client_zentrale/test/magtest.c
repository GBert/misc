#include <stdlib.h>
#include <stdio.h>
#if 0
#include <string.h>
#include <errno.h>
#include <boolean.h>
#include <config.h>
#include <cs2parse.h>
#include <write_cs2.h>
#endif
#include "../magnetartikel.h"

#define CS2_PATH "."

static void WalkMagnetartikelOfMagnetartikelCs2(void *PrivData,
                                                MapKeyType Key,
                                                MapDataType Daten)
{  MagnetartikelInfo *MagnetartikelPage;

   puts("Element");
   MagnetartikelPage = (MagnetartikelInfo *)Daten;
   printf("id %d\n", MagnetartikelInfoGetId(MagnetartikelPage));
   printf("name %s\n", MagnetartikelInfoGetName(MagnetartikelPage));
   printf("typ %s\n", MagnetartikelInfoGetTyp(MagnetartikelPage));
   printf("stellung %d\n", MagnetartikelInfoGetStellung(MagnetartikelPage));
   printf("schaltzeit %d\n", MagnetartikelInfoGetSchaltzeit(MagnetartikelPage));
   if (MagnetartikelInfoGetUngerade(MagnetartikelPage) != -1)
      printf("ungerade %d\n", MagnetartikelInfoGetUngerade(MagnetartikelPage));
   printf("decoder %s\n", MagnetartikelInfoGetDecoder(MagnetartikelPage));
   printf("dectyp %s\n", MagnetartikelInfoGetDectyp(MagnetartikelPage));
}

int main(int argc, char *argv[])
{  MagnetartikelStruct *Magnetartikel;

   puts("Create Magnetartikel");
   Magnetartikel = MagnetartikelCreate();
   if (Magnetartikel != (MagnetartikelStruct *)NULL)
   {
      puts("Init Magnetartikel");
      MagnetartikelInit(Magnetartikel, CS2_PATH);
      puts("Load magnetartikel.cs2");
      MagnetartikelLoadMagnetartikelCs2(Magnetartikel);
      puts("Magnetartikel:");
      MapWalkAscend(MagnetartikelGetMagnetartikelDb(Magnetartikel),
                    (MapWalkCbFkt)WalkMagnetartikelOfMagnetartikelCs2,
                    (void *)NULL);
      puts("Exit Magnetartikel");
      MagnetartikelExit(Magnetartikel);
      puts("Destroy Magnetartikel");
      MagnetartikelDestroy(Magnetartikel);
   }
   else
   {
      puts("Create Magnetartikel failed");
   }
}
