#include "fsm.h"

void FsmDo(FsmStruct *Data, int SignalNr, void *SignalData)
{  int NewState;
   SignalFunctionsType SignalFunctions;
   StateFktType SignalFunction;

   SignalFunctions = FsmGetFunctions(Data)[FsmGetState(Data)];
   SignalFunction = SignalFunctions[SignalNr];
   NewState = SignalFunction(FsmGetPriv(Data), SignalData);
   if (NewState != STATE_NO_CHANGE)
      FsmSetState(Data, NewState);
}
