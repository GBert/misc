#ifndef GBSSTAT_H
#define GBSSTAT_H

#include "gleisbild.h"

void GbsStatInsert(GleisbildStruct *Data, GleisbildInfo *Gleisbild);
void GbsStatParseGbsStatSr2(GleisbildStruct *Data, char *Buf, int Len);
void GbsStatLoadGbsStatSr2(GleisbildStruct *Data);
void GbsStatSaveGbsStatSr2(GleisbildStruct *Data);

#endif
