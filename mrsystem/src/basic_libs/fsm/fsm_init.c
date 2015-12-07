#include "fsm.h"

void FsmInit(FsmStruct *Data, void *PrivatePtr, int StartState, int NumSignals, int NumStates, StateFunctionsType Functions)
{
   FsmSetState(Data, StartState);
   FsmSetPriv(Data, PrivatePtr);
   FsmSetNumSignals(Data, NumSignals);
   FsmSetNumStates(Data, NumStates);
   FsmSetFunctions(Data, Functions);
}
