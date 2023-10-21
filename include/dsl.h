#define MAKE_COND_JUMP(name, count, cond)                           \
    if (tmp == -1) {                                                \
        MyAssert(tmp != -1 && "You entered wrong label", "pampam"); \
        Error = -1;                                                 \
    } else if(CMD_##name == CMD_JMP) {                              \
        jump(proc, tmp);                                            \
    } else if(proc->Register[1] cond count) {                       \
        jump(proc, tmp);                                            \
    } else {                                                        \
        proc->CurrentCommand++;                                     \
    }
    
     

DEF_CMD(HLT, -1, 0, 
                    printf("\nProgramm ended succesfully\n");
                    return Error;
            )
DEF_CMD(PUSH, 0, 1,
                    tmp = proc->command[++proc->CurrentCommand];
                    switch (proc->command[proc->CurrentCommand - 1] & IDENTIF) {
                    case regis:
                        StackPush(&proc->stk, proc->Register[tmp]);
                        break;
                    case immed:
                        StackPush(&proc->stk, tmp * SIGNS);
                        break;
                    })
DEF_CMD(POP,  1, 1, 
                    tmp = proc->command[++proc->CurrentCommand];
                    switch (proc->command[proc->CurrentCommand - 1] & IDENTIF) {
                    case regis:
                        StackPop(&proc->stk, proc->Register + tmp);
                        break;
                    case immed:
                        MyAssert((proc->command[proc->CurrentCommand - 1] & IDENTIF == immed) && "You are trying to pop a constant! Are you idiot!?", (char*) &(proc->command[proc->CurrentCommand]));
                        Error = -1;
                        break;
                    })
DEF_CMD(IN,   2, 0, 
                    printf("Enter number from keyboard\n");
                    while (scanf("%d", &tmp) != 1) {
                        printf("You entered not a number, try again\n");
                        while ((symbol = getchar()) != EOF && symbol != '\n') { }
                    }
                    tmp *= SIGNS;
                    StackPush(&proc->stk, tmp);)
DEF_CMD(ADD,  3, 0,
                    add(&proc->stk);)
DEF_CMD(SUB,  4, 0,
                    sub(&proc->stk);)
DEF_CMD(MUL,  5, 0,
                    mul(&proc->stk);)
DEF_CMD(DIV,  6, 0,
                    div(&proc->stk);)
DEF_CMD(SQRT, 7, 0,
                    proc_sqrt(&proc->stk);)
DEF_CMD(SIN,  8, 0,
                    proc_sin(&proc->stk);)
DEF_CMD(COS,  9, 0,
                    proc_cos(&proc->stk);)
DEF_CMD(OUT, 10, 0,
                    printf(GREEN"RESULT:\n");
                    out(&proc->stk);)
DEF_CMD(JMP, 11, 1, 
                    tmp = proc->command[++proc->CurrentCommand];
                    MAKE_COND_JUMP(JMP, 0, ==)
                    )
DEF_CMD(JA, 12, 1, 
                    tmp = proc->command[++proc->CurrentCommand];
                    StackPop(&proc->stk, &tmp1);
                    MAKE_COND_JUMP(JA, tmp1, >)
                    )
DEF_CMD(JAE, 13, 1, 
                    tmp = proc->command[++proc->CurrentCommand];
                    StackPop(&proc->stk, &tmp1);
                    MAKE_COND_JUMP(JAE, tmp1, >=)
                    )
DEF_CMD(JB, 14, 1, 
                    tmp = proc->command[++proc->CurrentCommand];
                    StackPop(&proc->stk, &tmp1);
                    MAKE_COND_JUMP(JB, tmp1, <)
                    )
DEF_CMD(JBE, 15, 1, 
                    tmp = proc->command[++proc->CurrentCommand];
                    StackPop(&proc->stk, &tmp1);
                    MAKE_COND_JUMP(JBE, tmp1, <=)
                    )
DEF_CMD(JE, 16, 1, 
                    tmp = proc->command[++proc->CurrentCommand];
                    StackPop(&proc->stk, &tmp1);
                    MAKE_COND_JUMP(JE, tmp1, ==)
                    )

DEF_CMD(JNE, 17, 1, 
                    tmp = proc->command[++proc->CurrentCommand];
                    StackPop(&proc->stk, &tmp1);
                    MAKE_COND_JUMP(JNE, tmp1, !=)
                    )

#include "dsl_undef.h"