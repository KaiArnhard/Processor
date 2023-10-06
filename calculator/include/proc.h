#if !defined(PROC_H)
#define PROC_H

#include "stack.h"
#include <cmath>
#include "command.h"

elem_t add(stack_t* stk);
elem_t sub(stack_t* stk);
elem_t div(stack_t* stk);
elem_t mul(stack_t* stk);
elem_t proc_sqrt(stack_t* stk);
elem_t proc_sin(stack_t* stk);
elem_t proc_cos(stack_t* stk);
elem_t out(stack_t* stk);

int VirtualMachine(FILE* PtrToCm, stack_t* stk);
void ProcComparator(FILE* PtrToCm, const command_t command, stack_t* stk);

#endif // PROC_H
