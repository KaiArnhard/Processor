#include "proc.h"

int main() {
    stack_t stk;
    STACK_CTOR(&stk);
    FILE* PtrToCM = fopen("../../cm.txt", "r");
    VirtualMachine(PtrToCM, &stk);
    StackDtor(&stk);
}