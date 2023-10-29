#define MAKE_COND_JUMP(name, count, cond)                           \
    if (tmp == -1) {                                                \
        MyAssert(tmp != -1 && "You entered wrong label", "pampam"); \
        Error = -1;                                                 \
    } else if(CMD_##name == CMD_JMP) {                              \
        Jump(proc, tmp);                                            \
    } else if(count cond proc->regis[1]) {                          \
        Jump(proc, tmp);                                            \
    }    
     

DEF_CMD(HLT, -1, 0, 
                    printf(BLUE "\nProgramm ended succesfully\n" WHITE);
                    return Error;
            )
DEF_CMD(PUSH, 0, 1,
                    tmp = proc->command[++proc->currentCommand];
                    switch (proc->command[proc->currentCommand - 1] & IDENTIF) {
                    case RamIdentif:
                        sleep(0.5);
                        StackPush(&proc->userStack, proc->ram[tmp]);
                        PrintOfRam(proc);
                        break;
                    case regis:
                        StackPush(&proc->userStack, proc->regis[tmp]);
                        break;
                    case immed:
                        StackPush(&proc->userStack, tmp * SIGNS);
                        break;
                    })
DEF_CMD(POP,  1, 1, 
                    tmp = proc->command[++proc->currentCommand];
                    switch (proc->command[proc->currentCommand - 1] & IDENTIF) {
                    case RamIdentif:
                        sleep(0.5);
                        StackPop(&proc->userStack, proc->ram + tmp);
                        PrintOfRam(proc);
                        break;
                    case regis:
                        StackPop(&proc->userStack, proc->regis + tmp);
                        break;
                    case immed:
                        MyAssert(0 && "You are trying to pop a constant! Are you idiot!?", (char*) &(proc->command[proc->currentCommand]));
                        Error = -1;
                        break;
                    })
DEF_CMD(IN,   2, 0, 
                    In(&proc->userStack);)
DEF_CMD(ADD,  3, 0,
                    Add(&proc->userStack);)
DEF_CMD(SUB,  4, 0,
                    Sub(&proc->userStack);)
DEF_CMD(MUL,  5, 0,
                    Mul(&proc->userStack);)
DEF_CMD(DIV,  6, 0,
                    Div(&proc->userStack);)
DEF_CMD(SQRT, 7, 0,
                    ProcSqrt(&proc->userStack);)
DEF_CMD(SIN,  8, 0,
                    ProcSin(&proc->userStack);)
DEF_CMD(COS,  9, 0,
                    ProcCos(&proc->userStack);)
DEF_CMD(OUT, 10, 0,
                    printf(GREEN "RESULT:\n" WHITE);
                    Out(&proc->userStack);)
DEF_CMD(JMP, 11, 1, 
                    tmp = proc->command[++proc->currentCommand];
                    MAKE_COND_JUMP(JMP, 0, ==)
                    )
DEF_CMD(JA, 12, 1, 
                    tmp = proc->command[++proc->currentCommand];
                    StackPop(&proc->userStack, &tmp1);
                    MAKE_COND_JUMP(JA, tmp1, >)
                    )
DEF_CMD(JAE, 13, 1, 
                    tmp = proc->command[++proc->currentCommand];
                    StackPop(&proc->userStack, &tmp1);
                    MAKE_COND_JUMP(JAE, tmp1, >=)
                    )
DEF_CMD(JB, 14, 1, 
                    tmp = proc->command[++proc->currentCommand];
                    StackPop(&proc->userStack, &tmp1);
                    MAKE_COND_JUMP(JB, tmp1, <)
                    )
DEF_CMD(JBE, 15, 1, 
                    tmp = proc->command[++proc->currentCommand];
                    StackPop(&proc->userStack, &tmp1);
                    MAKE_COND_JUMP(JBE, tmp1, <=)
                    )
DEF_CMD(JE, 16, 1, 
                    tmp = proc->command[++proc->currentCommand];
                    StackPop(&proc->userStack, &tmp1);
                    MAKE_COND_JUMP(JE, tmp1, ==)
                    )

DEF_CMD(JNE, 17, 1, 
                    tmp = proc->command[++proc->currentCommand];
                    StackPop(&proc->userStack, &tmp1);
                    MAKE_COND_JUMP(JNE, tmp1, !=)
                    )
DEF_CMD(CALL, 18, 1,
                    tmp = proc->command[++proc->currentCommand];
                    StackPush(&proc->callStack, proc->currentCommand + 1);
                    Jump(proc, tmp);
                    )
DEF_CMD(RET, 19, 0,
                    StackPop(&proc->callStack, &tmp);
                    Jump(proc, tmp);)

#include "DslUndef.h"