#ifndef FSSTAT_H
#define FSSTAT_H

#include "fahrstrasse.h"

void FsStatInsert(FahrstrasseStruct *Data, FahrstrasseInfo *Fahrstrasse);
void FsStatParseFsStatSr2(FahrstrasseStruct *Data, char *Buf, int Len);
void FsStatLoadFsStatSr2(FahrstrasseStruct *Data);
void FsStatSaveFsStatSr2(FahrstrasseStruct *Data);

#endif
