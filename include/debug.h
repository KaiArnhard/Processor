#ifndef DEBUG_H
#define DEBUG_H

#include <cstdio>
    
typedef unsigned long long canary_t;
typedef unsigned long long hash_t;

static FILE* pointerToDump = fopen("../ProcDump.txt", "w");

enum Errors_t {
    STACK_NO_ERRORS                        = 0,
    STACK_ERROR_STACK_OVERFLOW             = 1,
    STACK_ERROR_PTR_TO_STK_ZERO            = 1 << 1,
    STACK_ERROR_PTR_TO_DATA_ZERO           = 1 << 2,
    STACK_ERROR_SIZE_OVER_CAPACITY         = 1 << 3,
    STACK_ERROR_SIZE_LOWER_ZERO            = 1 << 4,
    STACK_ERROR_CAPACITY_LOWER_ZERO        = 1 << 5,
    STACK_ERROR_CAPACITY_EQUAL_ZERO        = 1 << 6,
    STACK_ERROR_CAPACITY_LOWER_DEFAULT     = 1 << 7,
    
    #if defined(CANARY_PROT)
        STACK_ERROR_LEFT_CANARY_DIED       = 1 << 8,
        STACK_ERROR_RIGHT_CANARY_DIED      = 1 << 9,
        STACK_ERROR_DATA_LEFT_CANARY_DIED  = 1 << 10,
        STACK_ERROR_DATA_RIGHT_CANARY_DIED = 1 << 11,
    #endif // CANARY_PROT
    
    #if defined(HASH_PROT)
        STACK_ERROR_WRONG_HASH             = 1 << 12
    #endif // HASH_PROT
    
};

static size_t MyErrorno = STACK_NO_ERRORS;

#define STACK_CHECK(stk)                                         \
    if (StackVerify(stk)) {                                      \
        StackDump(stk, __FILE__, __PRETTY_FUNCTION__, __LINE__); \
    }    

#define STACK_DUMP(stk)                                          \
    StackDump(stk, __FILE__, __PRETTY_FUNCTION__, __LINE__);     \

#if defined(HASH_PROT)

    #define STACK_HASH(stk)                                                                                             \
        stk->hash = 0;                                                                                                  \
        stk->hash = StackHash((char*) stk, StackSize) + StackHash((char*) stk->data, stk->capacity * sizeof(elem_t));   \

#endif // HASH_PROT


#define PTR_STACK_CHECK(condition)                                                                                                                                  \
if(!condition) {                                                                                                                                                    \
    MyErrorno |= STACK_ERROR_PTR_TO_STK_ZERO;                                                                                                                       \
    fprintf(pointerToDump, "Ptr %p, Error number %d, occurs in FILE %s, on line %d, function %s\n", condition, MyErrorno, __FILE__, __LINE__, __PRETTY_FUNCTION__); \
    exit(-1);                                                                                                                                                       \
}

#endif