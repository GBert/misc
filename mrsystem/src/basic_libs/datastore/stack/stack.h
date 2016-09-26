#ifndef STACK_H
#define STACK_H

#include <liste.h>

typedef void *StackDataType;
typedef void (*StackDelCbFkt)(StackDataType Data);
typedef struct {
   Liste *StackDaten;
} Stack;

Stack *StackCreate(void);
void StackDestroy(Stack *Wurzel);
void StackInit(Stack *Wurzel, StackDelCbFkt Cb);
BOOL StackIsEmpty(Stack *Wurzel);
BOOL StackPush(Stack *MyStack, StackDataType Daten);
StackDataType StackPop(Stack *MyStack);

#endif
