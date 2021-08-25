#ifndef LOGMS2_H
#define LOGMS2_H

#include <boolean.h>
#include "can_io.h"

typedef struct {
   BOOL Verbosity;
   IoFktStruct *IoFunctions;
} Logms2Struct;

#define Logms2SetVerbose(Data, Verbose)  (Data)->Verbosity=Verbose
#define Logms2SetIoFunctions(Data, Fkts) (Data)->IoFunctions=Fkts

#define Logms2GetVerbose(Data)     (Data)->Verbosity
#define Logms2GetIoFunctions(Data) (Data)->IoFunctions

Logms2Struct *Logms2Create(void);
void Logms2Destroy(Logms2Struct *Data);
void Logms2Init(Logms2Struct *Data, BOOL Verbose, IoFktStruct *IoFunctions);
void Logms2Run(Logms2Struct *Data);

#endif
