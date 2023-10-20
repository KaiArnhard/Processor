#if !defined(PROC_H)
#define PROC_H

#include "stack.h"
#include <cmath>

struct SPU_t {
    elem_t Register[4];

    stack_t stk;
    
    size_t NumbOfComs;
    size_t CurrentCommand;
    elem_t* command;
};

#define SPU_DUMP(proc) {                                       \
    SPUDump(proc, __FILE__, __PRETTY_FUNCTION__, __LINE__);    \
}

void SPUCtor(SPU_t* proc, FILE* PtrToCm);
void SPUDtor(SPU_t* proc);
void SPUDump(SPU_t* proc, const char* file, const char* function, size_t line);

elem_t add(stack_t* stk);
elem_t sub(stack_t* stk);
elem_t div(stack_t* stk);
elem_t mul(stack_t* stk);
elem_t proc_sqrt(stack_t* stk);
elem_t proc_sin(stack_t* stk);
elem_t proc_cos(stack_t* stk);
elem_t out(stack_t* stk);
size_t jump(SPU_t* proc, size_t tmp);

int VirtualMachine(SPU_t* stk, FILE* PtrToCm);
void ProcComparator(SPU_t* proc);
int bitwise(char* BitComm, char* Identif);

#endif // PROC_H
