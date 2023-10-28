#if !defined(PROC_FUNCTIONS_H)
#define PROC_FUNCTIONS_H

#include <cmath>
#include "StackFunctions.h"

static const size_t NumbOfRegs = 4;
static const size_t SizeOfRAM  = 100;

struct SPU_t {
    elem_t Register[NumbOfRegs];
    elem_t RAM[SizeOfRAM];

    stack_t UserStack;
    stack_t CallStack;
    
    size_t NumbOfComs;
    size_t CurrentCommand;
    elem_t* command;
};

#define SPU_DUMP(proc) {                                       \
    SPUDump(proc, __FILE__, __PRETTY_FUNCTION__, __LINE__);    \
}

#if defined(SHM)
    elem_t* SPUShmCtor();
#endif // SHM

void SPUCtor(SPU_t* proc, stack_t UserStack, stack_t CallStack, elem_t* commands, size_t NumbOfComms);
void SPUCtor(SPU_t* proc, const char* PathToCm);
void SPUDtor(SPU_t* proc);

void SPUDump(SPU_t* proc, const char* file, const char* function, size_t line);

void PrintfOfRAM(SPU_t* proc);
elem_t add(stack_t* UserStack);
elem_t sub(stack_t* UserStack);
elem_t div(stack_t* UserStack);
elem_t mul(stack_t* UserStack);
elem_t proc_sqrt(stack_t* UserStack);
elem_t proc_sin(stack_t* UserStack);
elem_t proc_cos(stack_t* UserStack);
elem_t out(stack_t* UserStack);
size_t jump(SPU_t* proc, size_t DestAddress);

int VirtualMachine(SPU_t* proc);
void ProcComparator(SPU_t* proc);

#endif // PROC_FUNCTIONS_H
