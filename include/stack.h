#ifndef STACK_H
#define STACK_H

#include <cstdio>
#include <cstdlib>
#include <limits.h>
#include "debug.h"

typedef int elem_t;

static const int SIGNS = 100;
static const int ResizeConst = 2;

enum Resize_t {
    UP   = 1,
    DOWN = 0
};

const size_t DefaultSize = 10;
const elem_t POISON = *((elem_t*) "POIS");

struct DebugVariables
{
    const char*  name;
    size_t       line;
    const char*  file;
    const char*  function;
};
struct stack_t {
    #if defined(CANARY_PROT)
        canary_t LeftCanary = 0xDED320BED;
    #endif //LEFT_CANARY_PROT 
    
    elem_t*        data;
    size_t         size;
    size_t         capacity;
    DebugVariables var;  
    size_t         OldCapacity;

    #if defined(HASH_PROT)
        hash_t hash;
    #endif // HASH_PROT
    

    #if defined(CANARY_PROT)
        int offset = 0;
        canary_t RightCanary = 0xBADC0FFEE;
    #endif //RIGHT_CANARY_PROT
};

#if defined(HASH_PROT)

    #if defined(CANARY_PROT)
        const size_t StackSize = 2 * sizeof(canary_t) + sizeof(elem_t*) + 3 * sizeof(size_t) + sizeof(DebugVariables);
    #else
        const size_t StackSize = sizeof(elem_t*) + 3 * sizeof(size_t) + sizeof(DebugVariables);
    
    #endif // CANARY_PROT
    

#endif // HASH_PROT

#define STACK_CTOR(stk) {                                                          \
    StackCtor(stk, DefaultSize, #stk, __LINE__, __FILE__, __PRETTY_FUNCTION__);    \
}

void StackCtor (stack_t* stk, size_t capacity, const char* name, const size_t line, const char* file, const char* function);
void StackDtor (stack_t* stk);
void PoisStack(stack_t* stk);
void PrintOfData(stack_t* stk, FILE* fp);

void StackPush(stack_t* stk, const elem_t variable);
void StackPop(stack_t* stk, elem_t* ptr);
size_t StackResize(stack_t* stk, Resize_t CodeOfResize);

FILE* StackDump(stack_t* stk, const char* file, const char* function, size_t line);
size_t StackVerify(stack_t* stk);
void ErrorDecoder(size_t* Error);
void ErrToStr(size_t error);

#if defined(HASH_PROT)

hash_t StackHash(char* ptr, size_t lenght);
bool StackHashCheck(stack_t* stk);

#endif // HASH_PROT

#endif
