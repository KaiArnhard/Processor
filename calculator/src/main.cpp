#include "../include/proc.h"

int main() {
    stack_t stk;
    STACK_CTOR(&stk);
    
    for (size_t i = 0; i < 2; i++) {
        StackPush(&stk, 5);
    }

    printf("%d\n", add(&stk));
    StackDtor(&stk);
}