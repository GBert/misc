#include <stdlib.h>
#include "fsm.h"

void FsmDestroy(FsmStruct *Data)
{
   free(Data);
}
