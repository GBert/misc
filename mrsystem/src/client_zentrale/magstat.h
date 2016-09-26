#ifndef MAGSTAT_H
#define MAGSTAT_H

#include "magnetartikel.h"

void MagStatusInsert(MagnetartikelStruct *Data,
                     MagnetartikelInfo *Magnetartikel);
void MagStatusParseMagStatusSr2(MagnetartikelStruct *Data, char *Buf, int Len);
void MagStatusLoadMagStatusSr2(MagnetartikelStruct *Data);
void MagStatusSaveMagStatusSr2(MagnetartikelStruct *Data);

#endif
