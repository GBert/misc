#include <stdlib.h>
#include "fsm.h"

FsmStruct *FsmCreate(void)
{  FsmStruct *Data;

   Data = (FsmStruct *)malloc(sizeof(FsmStruct));
   if (Data != (FsmStruct *)NULL)
   {
   }
   return(Data);
}
