#include "../include/proc.h"

int add(stack_t *stk) {
    int tmp1 = 0;
    int tmp2 = 0;

    StackPop(stk, &tmp1);
    StackPop(stk, &tmp2);

    return tmp1 + tmp2;
}