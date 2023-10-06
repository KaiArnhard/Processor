#include "proc.h"

elem_t add(stack_t *stk) {
    elem_t tmp1 = 0;
    elem_t tmp2 = 0;

    StackPop(stk, &tmp1);
    StackPop(stk, &tmp2);
    StackPush(stk, (tmp1 + tmp2) * SIGNS);

    return (tmp1 + tmp2) * SIGNS;
}

elem_t sub(stack_t* stk) {
    elem_t deduct =  0;
    elem_t redused = 0;

    StackPop(stk, &deduct);
    StackPop(stk, &redused);
    StackPush(stk, (redused - deduct) * SIGNS);

    return (redused - deduct) * SIGNS;
}

elem_t div(stack_t* stk) {
    elem_t divider   = 0;
    elem_t divisible = 0;

    StackPop(stk, &divider);
    StackPop(stk, &divisible);
    StackPush(stk, (divisible * SIGNS / divider));

    return (divisible * SIGNS) / divider;
}

elem_t mul(stack_t* stk) {
    elem_t tmp1 = 0;
    elem_t tmp2 = 0;

    StackPop(stk, &tmp1);
    StackPop(stk, &tmp2);
    StackPush(stk, tmp1 * tmp2 / SIGNS);

    return tmp1 * tmp2;
}

elem_t proc_sqrt(stack_t* stk) {
    elem_t tmp = 0;
    StackPop(stk, &tmp);
    double tmp1 = sqrt(tmp * SIGNS);
    tmp = tmp1;
    StackPush(stk, tmp);
    return tmp;
}

elem_t proc_sin(stack_t* stk) {
    elem_t tmp = 0;
    StackPop(stk, &tmp);
    double tmp1 = sin(tmp / SIGNS) * SIGNS;
    tmp = tmp1;
    StackPush(stk, tmp);
    return tmp;
}

elem_t proc_cos(stack_t* stk) {
    elem_t tmp = 0;
    StackPop(stk, &tmp);
    double tmp1 = cos(tmp / SIGNS) * SIGNS;
    tmp = tmp1;
    StackPush(stk, tmp);
    return tmp;
}

elem_t in(stack_t *stk) {
    elem_t tmp = 0;
    scanf("%d", &tmp);
    return tmp;
}

elem_t out(stack_t* stk) {
    elem_t tmp = 0;
    StackPop(stk, &tmp);
    
    printf("\033[0;31m");
    printf("%d\n", tmp);
    
    return tmp;
}

int VirtualMachine(FILE* PtrToCm, stack_t* stk) {
    int NumbOfLines = 0;
    command_t command;

    fscanf(PtrToCm, "%d", &NumbOfLines);

    for (size_t counter = 0; counter < NumbOfLines; counter++) {
        fscanf(PtrToCm, "%d", &command);
        ProcComparator(PtrToCm, command, stk);
        STACK_DUMP(stk);
    }  
    return 0;
}

void ProcComparator(FILE* PtrToCm, const command_t command, stack_t* stk) {
    
    int tmp = 0;
    int poped = 0;
    char symbol = 0;

    switch (command) {
    case HLT:
        exit(0);
        break;
    case STACK_PUSH:
        fscanf(PtrToCm, "%d", &tmp);
        tmp *= SIGNS;
        StackPush(stk, tmp);
        break;
    case STACK_IN:
        printf("Enter number from keyboard\n");
        while (scanf("%d", &tmp) != 1) {
            printf("You entered not a number, try again\n");
            while ((symbol = getchar()) != EOF && symbol != '\n') { }
        }
        tmp *= SIGNS;
        StackPush(stk, tmp);
        break;
    case STACK_POP:
        StackPop(stk, &poped);
        break;
    case ADD:
        add(stk);
        break;
    case SUB:
        sub(stk);
        break;
    case DIV:
        div(stk);
        break;
    case MUL:
        mul(stk);
        break;
    case SQRT:
        proc_sqrt(stk);
        break;
    case SIN:
        proc_sin(stk);
        break;
    case COS:
        proc_cos(stk);
        break;
    case OUT:
        out(stk);
        break;
    default:
        abort();
    break;
    }
}