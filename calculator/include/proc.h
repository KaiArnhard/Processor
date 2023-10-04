#if !defined(PROC_H)
#define PROC_H

#include "stack.h"
#include <cmath>

int add(stack_t* stk);
int sub(stack_t* stk);
int div(stack_t* stk);
int mul(stack_t* stk);
int proc_sqrt(stack_t* stk);
int proc_sin(stack_t* stk);
int proc_cos(stack_t* stk);
int out(stack_t* stk);

int VirtualMachine(FILE* PtrToCm, stack_t* stk);
void ProcComparator(FILE* PtrToCm, const int command, stack_t* stk);

#endif // PROC_H
