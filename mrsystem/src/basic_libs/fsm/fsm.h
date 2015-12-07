#ifndef FSM_H
#define FSM_H

typedef int (*StateFktType)(void *Priv, void *SignalData);
typedef StateFktType *SignalFunctionsType;
typedef SignalFunctionsType *StateFunctionsType;

typedef struct {
   int State;
   void *Priv;
   int NumSignals;
   int NumStates;
   StateFunctionsType Functions;
} FsmStruct;

#define FsmSetState(Data, NewState) (Data)->State=NewState
#define FsmSetPriv(Data, Private)   (Data)->Priv=Private
#define FsmSetNumSignals(Data, n)   (Data)->NumSignals=n
#define FsmSetNumStates(Data, n)    (Data)->NumStates=n
#define FsmSetFunctions(Data, Fkt)  (Data)->Functions=Fkt

#define FsmGetState(Data)      (Data)->State
#define FsmGetPriv(Data)       (Data)->Priv
#define FsmGetNumSignals(Data) (Data)->NumSignals
#define FsmGetNumStates(Data)  (Data)->NumStates
#define FsmGetFunctions(Data)  (Data)->Functions

#define STATE_NO_CHANGE         -1

FsmStruct *FsmCreate(void);
void FsmDestroy(FsmStruct *Data);
void FsmInit(FsmStruct *Data, void *PrivatePtr, int StartState, int NumSignals, int NumStates, StateFunctionsType Functions);
void FsmExit(FsmStruct *Data);
void FsmDo(FsmStruct *Data, int SignalNr, void *SignalData);

#endif
