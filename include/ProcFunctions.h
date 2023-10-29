#if !defined(PROC_FUNCTIONS_H)
#define PROC_FUNCTIONS_H

#include <cmath>
#include "StackFunctions.h"

static const size_t numbOfRegs = 4;
static const size_t sizeOfRam  = 100;

struct SPU_t {
    elem_t regis[numbOfRegs];
    elem_t ram[sizeOfRam];

    stack_t userStack;
    stack_t callStack;
    
    size_t numbOfComs;
    size_t currentCommand;
    elem_t* command;
};

#define SPU_DUMP(proc) {                                       \
    SPUDump(proc, __FILE__, __PRETTY_FUNCTION__, __LINE__);    \
}

#if defined(SHM)
    elem_t* SPUShmCtor();
#endif // SHM

void SPUCtor(SPU_t* proc, stack_t userStack, stack_t callStack, elem_t* commands, size_t numbOfComms);
void SPUCtor(SPU_t* proc, FILE* ptrToCm);
void SPUCtor(SPU_t* proc, elem_t* commands, size_t numbOfComms);
void SPUDtor(SPU_t* proc);

void SPUDump(SPU_t* proc, const char* file, const char* function, size_t line);

void PrintOfRam(SPU_t* proc);

elem_t In(stack_t *userStack);

elem_t Add(stack_t* UserStack);
elem_t Sub(stack_t* UserStack);

elem_t Div(stack_t* UserStack);
elem_t Mul(stack_t* UserStack);

elem_t ProcSqrt(stack_t* UserStack);
elem_t ProcSin(stack_t* UserStack);
elem_t ProcCos(stack_t* UserStack);

elem_t Out(stack_t* UserStack);
size_t Jump(SPU_t* proc, size_t DestAddress);

int VirtualMachine(SPU_t* proc);
void ProcComparator(SPU_t* proc);

#endif // PROC_FUNCTIONS_H
