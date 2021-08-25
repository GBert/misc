#ifndef LOKSTATUS_H
#define LOKSTATUS_H

#include "lok.h"

void LokStatusInsert(LokStruct *Data, LokInfo *Lok);
void LokStatusParseLokomotiveSr2(LokStruct *Data, char *Buf, int Len);
void LokStatusLoadLokomotiveSr2(LokStruct *Data);
void LokStatusSaveLokomotiveSr2(LokStruct *Data);

#endif
